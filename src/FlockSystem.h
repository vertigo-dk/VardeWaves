//
//  FlockSystem.h
//  StatesOfMatter
//
//  Created by Jonas Fehr on 28.04.14.
//
//

#pragma once


#include "ofMain.h"
#include "Boid.h"


//-----------------------------------------------------------
class FlockSystem { // : public ofThread {
protected:
    vector<Boid> boidsThread, boidsBuffer, boidsUpdate;
    vector<Predator> predatorsThread;

    ofMesh meshLinesDraw, meshLinesUpdate, meshLinesBuffer;

    vector<ofVec2f>  boidsPosUpdate, boidsPosBuffer;
    vector<ofVec2f>  boidsVelUpdate, boidsVelBuffer;
    
    
    void threadedFunction() {while(true) {update();}}
    
public:
    vector<Boid> boids;
    vector<Predator> predators;
    
    
    vector<ofVec2f> boidsPos;
    vector<ofVec2f> boidsVel;
    
    FlockSystem();
    void setup(float numOfBoids_, ofVec2f gravity_, int diameter_);
    void update();
    void draw(int _x, int _y, int width_, int height_, ofColor drawColor);

    
    // variables for the changing environement

    ofVec2f gravity;
    float attraction, attractionU, attractionLast;
    int numOfBoids, numOfBoidsU, numOfBoidsLast;
    float temp, tempU;
    
    // variable and constructor for Boids, variable length
    
    void addNewBoid();
    void deleteLastBoid();
    void setAllAttractors(float strength_); // function to set all strengths of the attractor
    void setAllMax(float maxForce_, float maxSpeed_);
    void setAllWheigts(float weightSep_, float weightAli_, float weightCoh_);
    void setAllNeighbourdist(float neightbourdist_);
    void setAllDesiredsep(float desiredsep_);


    int renderSize;

    
    void drawConnections();
    bool flagDrawConnections;
    float minimumDistance;
    float propRate = 0;
    
    void drawPlasma();
    bool flagDrawPlasma;
    


    bool flagDimensions = false;


    float getTemp();

    
    ofFbo imageFbo;
    
    ofVbo vbo;

    ofShader shader;
    ofTexture texture;
    ofColor colorBoids;
    bool blendMode;
    ofRectangle viewPort;

    
    //this holds all of our points
    vector<ofVec2f> meshPoints[1];
    
    bool addedNewBoids;
    
    ofColor colorLines;
    int lineWidth;
    
    ofColor colorBG;
    
    //BoidControl
    float maxSpeed, maxSpeedU, maxSpeedLast;
    float maxForce, maxForceU, maxForceLast;

    
    float desiredseparation,desiredseparationU,desiredseparationLast;
    float neighbourdist,neighbourdistU,neighbourdistLast;
    
    
    
    float weightSep,weightSepU,weightSepLast;
    float weightAli,weightAliU,weightAliLast;
    float weightCoh,weightCohU,weightCohLast;
    
    vector<float> distDetectUpdate, distDetectBuffer, distDetect;

    
private:
    // function to check if two Boids collide
    bool checkCollisionElastik(int indxBoid_1, int indxBoid_2);
    float getDistancePow2(ofVec2f pos1, ofVec2f pos2);
   // Poco::Condition condition;
};
