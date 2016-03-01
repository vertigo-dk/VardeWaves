//
//  FlockSystem.cpp
//  StatesOfMatter
//
//  Created by Jonas Fehr on 28.04.14.
//
//

#include "FlockSystem.h"

//------------------------------------------------------------------------
FlockSystem::FlockSystem(){
    
}

void FlockSystem::setup(float numOfBoids_, ofVec2f gravity_, int diameter_) {
    
    renderSize = diameter_;
    numOfBoids = numOfBoids_;
    
    imageFbo.allocate(renderSize, renderSize, GL_RGBA);//A32F_ARB);
    imageFbo.begin();
    ofClear(255,255,255, 0);
    ofEnableSmoothing();
    imageFbo.end();
    
    
    
    
    
    for(int i=0; i<numOfBoids_; i++){
        addNewBoid();
    }
    numOfBoidsLast = numOfBoids;
    
    for(int i=0; i<boidsThread.size(); i++){
        boidsThread[i].passEnvironment(&boidsThread, &predators, i);
    }
    boids = boidsThread;
    gravity = gravity_;
    
    
    colorBoids = ofColor(255, 255, 255);
    
}

void FlockSystem::update() {
   //  lock();
    attractionU = attraction;
    numOfBoidsU = numOfBoids;
    
    maxSpeedU = maxSpeed;
    maxForceU = maxForce;
    
    desiredseparationU = desiredseparation;
    neighbourdistU = neighbourdist;
    
    weightSepU = weightSep;
    weightAliU = weightAli;
    weightCohU = weightCoh;
    
    predatorsThread = predators;
    boidsThread = boids;
    
   // unlock(); 
    
    if(attractionU != attractionLast){
        setAllAttractors(attractionU);
        attractionLast = attractionU;
    }
    if(maxForceU != maxForceLast || maxSpeedU != maxSpeedLast){
        setAllMax(maxForceU, maxSpeedU);
        maxForceLast = maxForceU;
        maxSpeedLast = maxSpeedU;
    }
    
    if(desiredseparationU != desiredseparationLast){
        setAllDesiredsep(desiredseparationU);
        desiredseparationLast = desiredseparationU;
    }
    
    if(neighbourdistU != neighbourdistLast){
        setAllNeighbourdist(neighbourdistU);
        neighbourdistLast = neighbourdistU;
    }
    
    if(weightSepU != weightSepLast ||weightAliU != weightAliLast || weightCohU != weightCohLast){
        setAllWheigts(weightSepU, weightAliU, weightCohU);
        weightSepLast = weightSepU;
        weightAliLast = weightAliU;
        weightCohLast = weightCohU;
    }
    
    
    if(numOfBoidsU > numOfBoidsLast){
        for (int n = 0; n<numOfBoidsU-numOfBoidsLast; n++){
            addNewBoid();
            
        }
        
    }
    else if(numOfBoidsU < numOfBoidsLast){
        for (int n = 0; n<numOfBoidsLast-numOfBoids; n++){
            deleteLastBoid();
            
        }
        
    }
    numOfBoidsLast = boidsThread.size();
    
    for(int i=0; i<boidsThread.size(); i++){
        boidsThread[i].passEnvironment(&boidsThread, &predatorsThread, i);
    }
    
     /*if(addedNewboids==true){
     addedNewboids = false;
     for(int i=0; i<boids.size(); i++){
     boids[i].passEnvironment(&boids, i, center);
     }
     }*/
    
    // update the boids
    boidsPosUpdate.clear();
    boidsVelUpdate.clear();
    
    for(int i=0; i<boidsThread.size(); i++){
        boidsThread[i].update();
        boidsPosUpdate.push_back(boidsThread[i].position);
        boidsVelUpdate.push_back(boidsThread[i].velocity);

    }
    
    // BubbleSort
    
    /* int n = boids.size();
     Boid dummy;
     for (int r = 1; r < n; r++){
     for (int i = 0; i < n - r; i++){
     if(flagDimensions){
     float dist1 = getDistancePow2(camera.getPosition(), boids[i].position);
     float dist2 = getDistancePow2(camera.getPosition(), boids[i+1].position);
     if (dist1 < dist2)
     {
     dummy = boids[i];
     boids[i] = boids[i+1];
     boids[i + 1] = dummy;
     }
     }else{
     if (boids[i].position.z < boids[i+1].position.z)
     {
     dummy = boids[i];
     boids[i] = boids[i+1];
     boids[i + 1] = dummy;
     }
     }
     
     }
     }*/
    
    
    // Create Mesh for Lines
    if(flagDrawConnections){
        meshLinesUpdate.clear();
        distDetectUpdate.clear();
        
        for(int i=0; i<boidsThread.size(); i++){
            meshLinesUpdate.addVertex(boidsThread[i].position);
        }
        
        
        
        
        for(int i1=0; i1 < boidsThread.size(); i1++){
            ofVec2f pos1 = meshLinesUpdate.getVertex(i1);
            for(int i2=i1+1; i2 < boidsThread.size(); i2++){
                ofVec2f pos2 = meshLinesUpdate.getVertex(i2);
                if(abs(pos1.x-pos2.x) < minimumDistance){
                    if(abs(pos1.y-pos2.y) < minimumDistance){
                        float dist2 = getDistancePow2(pos1, pos2);
                        float minDist2 = minimumDistance*minimumDistance;
                        if( dist2 < minDist2){
                            meshLinesUpdate.addIndex(i1);
                            meshLinesUpdate.addIndex(i2);
                            distDetectUpdate.push_back(dist2);

                            //  meshLinesUpdate.setColorForIndices(i1, i2, ofColor(255,255,255, 100));//(int)(255*dist2/minDist2)));
                            
                        }
                    }
                }
            }
        }
    }
    tempU=getTemp();
    
//    mutex.lock();
    boidsBuffer.clear();
    boidsBuffer = boidsThread;
    meshLinesBuffer.clear();
    meshLinesBuffer = meshLinesUpdate;
    distDetectBuffer.clear();
    distDetectBuffer = distDetectUpdate;
    
    boidsPosBuffer.clear();
    boidsPosBuffer = boidsPosUpdate;
    
    boidsVelBuffer.clear();
    boidsVelBuffer = boidsVelUpdate;
    
    temp = tempU;
 //   condition.wait(mutex);
 //   mutex.unlock();
    
}
void FlockSystem::draw(int _x, int _y, int width_, int height_, ofColor drawColor) {
    
 //   mutex.lock();
   // boids.clear();
    boids = boidsBuffer;
    meshLinesDraw.clear();
    meshLinesDraw = meshLinesBuffer;
    
    
    
    distDetect.clear();
    distDetect = distDetectBuffer;
    
    boidsPos.clear();
    boidsPos = boidsPosBuffer;
    
    boidsVel.clear();
    boidsVel = boidsVelBuffer;
    
    
 //   condition.signal();
 //   mutex.unlock();
    
    
    imageFbo.begin();
    ofBackground(0, 100);
    // vector to store values for shader communication
    int total = boids.size();
    for(int i = 0; i < total; i++){
        boids[i].draw();
    }
    

    flagDrawConnections = false;
    if(flagDrawConnections && meshLinesDraw.hasIndices()) {
        
        meshLinesDraw.setMode(OF_PRIMITIVE_LINES);
        ofSetColor(0,0,255);
        //ofFill();
        ofSetLineWidth(lineWidth);
        ofEnableSmoothing();
        //ofEnableAlphaBlending();
        meshLinesDraw.draw();
        // ofDisableAlphaBlending();
        
    }
    
    /*for(int i = 0; i < predators.size(); i++){
        predators[i].draw();
    }*/
    
    imageFbo.end();
    
    ofSetColor(drawColor);
    
    imageFbo.draw(_x, _y, width_, height_);
}


void FlockSystem::addNewBoid(){
    
    float mass = 5;
    
    ofVec2f initialVelocity = ofVec2f(ofRandom(-10,10),ofRandom(-10,10));
    ofVec2f origin = ofVec2f( ofRandom(renderSize), ofRandom(renderSize));

    
    Boid boid = *new Boid();
    
    boid.setup(mass, origin, initialVelocity, renderSize, maxSpeed, maxForce);
    boidsThread.push_back(boid);
    
    addedNewBoids = true;
    
}

void FlockSystem::deleteLastBoid(){
    boidsThread.pop_back();
    
}

void FlockSystem::setAllAttractors(float strength_){
    for(int i = 0; i < boidsThread.size(); i++){
        boidsThread[i].attractionStrength = strength_;
    }
}
void FlockSystem::setAllMax(float maxForce_, float maxSpeed_){
    for(int i = 0; i < boidsThread.size(); i++){
        boidsThread[i].maxForce = maxForce;
        boidsThread[i].maxSpeed = maxSpeed;
    }
}

void FlockSystem::setAllWheigts(float weightSep_, float weightAli_, float weightCoh_){
    for(int i = 0; i < boidsThread.size(); i++){
        boidsThread[i].weightSep = weightSep_;
        boidsThread[i].weightAli = weightAli_;
        boidsThread[i].weightCoh = weightCoh_;
    }
}

void FlockSystem::setAllNeighbourdist(float neighbourdist_){
    for(int i = 0; i < boidsThread.size(); i++){
        boidsThread[i].neighbourdist = neighbourdist_;
    }
}

void FlockSystem::setAllDesiredsep(float desiredsep_){
    for(int i = 0; i < boidsThread.size(); i++){
        boidsThread[i].desiredseparation = desiredsep_;
    }
}

float FlockSystem::getTemp(){
    float tempX;
    for(int i=0; i<boidsThread.size(); i++){
        tempX += boidsThread[i].velocity.length()/boidsThread.size();
    }
    
    return tempX;
}

float FlockSystem::getDistancePow2(ofVec2f pos1, ofVec2f pos2){
    return (pos1.x-pos2.x)*(pos1.x-pos2.x) + (pos1.y-pos2.y)*(pos1.y-pos2.y);
    
}


