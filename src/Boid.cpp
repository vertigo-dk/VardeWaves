//
//  classes.cpp
//  StatesOfMatter
//
//  Created by Jonas Fehr on 26.04.14.
//
//

#include "Boid.h"

// CLASS Boid
Boid::Boid(){
    
}
void Boid::setup(float mass_, ofVec2f position_, ofVec2f velocity_, float worldSize_, float _maxSpeed, float _maxForce) {
    mass = mass_;
    radius = mass;
    position = ofVec2f(0);
    velocity = ofVec2f(0);
    acceleration = ofVec2f(0);
    position = position_;
    velocity = velocity_;
    worldSize = worldSize_;
    maxSpeed = _maxSpeed;
    maxForce = _maxForce;
    
}
void Boid::passEnvironment(vector <Boid> *boids_, vector <Predator> *predators_, int ownIndx_) {
    boidsREF = boids_;
    ownIndx = ownIndx_;
    predatorsREF = predators_;
}

void Boid::update() {
    flock();
    //attraction();
    checkCollisionsAndAttraction();
    // checkCollisions();
    predatorCharacter();
    checkPredator();
    checkWalls();
    //noWalls();
    velocity = velocity.operator+(acceleration);
    acceleration = ofVec2f(0,0);
    //  velocity.operator*=(damping_);
    if(velocity.length()>maxSpeed){
        velocity = velocity.getNormalized().operator*(maxSpeed);
    }
    
    
    position.operator+=(velocity);
}


void Boid::draw() {
    
    ofFill();
    ofSetColor(255);
    // if(collidePredator && !predatorsREF->empty()){
    //    ofSetColor(255, 0, 0 , 100);
    //  vector<ofPoint> vertices = predatorsREF->at(0).shape.getVertices();
    //        ofTriangle(vertices[indxNearest], vertices[indx2ndNearest], position);
    
    //  }
    ofDrawCircle(position.x, position.y, radius);
    
}

// Private
void Boid::attraction(){
    //AttractorForce A = strength *((m1*m2)/|∆P|^2)*∆P/|∆P|
    
    ofVec2f actualPos1 = position.operator+(acceleration); // Actual Position of this Boid
    
    for(int i=0; i < boidsREF->size(); i++){
        
        ofVec2f actualPos2 = boidsREF->at(i).position.operator+(boidsREF->at(i).acceleration);
        ofVec2f attractionVector = actualPos1.operator-(actualPos2);
        
        float distance = attractionVector.length();
        float collideDistance = radius+boidsREF->at(i).radius;
        
        if(distance < collideDistance){
            return;
        }
        
        ofVec2f attractionForce = attractionVector.normalize().operator*=(attractionStrength*(mass*boidsREF->at(i).mass)/(pow(distance,2)));
        
        acceleration.operator+=(-attractionForce.operator/(2));
        //boidsREF->at(i).acceleration.operator+=(attractionForce.operator/(2));
    }
    
}

void Boid::predatorCharacter(){
    if(!predatorsREF->empty()){
        for(int i=0; i < predatorsREF->size(); i++){
            Predator predator = predatorsREF->at(i);

            if(predator.character>0){ // attract boids
                
                
                float distSquared = position.squareDistance(predator.head);
                //    if(distSquared < predator.reactionDist * predator.reactionDist){ // see if within range
                ofVec2f directionVec = position.operator-(predator.head).normalize();
                
                ofVec2f attractForce = directionVec.operator*=(-predator.attraction*predator.character *sqrt(distSquared)/predator.reactionDist);//sqrt(distSquared)/predator.reactionDist);
                acceleration.operator+=(attractForce);
                
                //      }
                
                
                
            }else if(predator.character < 0){
                float distSquared = position.squareDistance(predator.head);
                if(distSquared < predator.reactionDist * predator.reactionDist){ // see if within range
                    ofVec2f directionVec = position.operator-(predator.head).normalize();
                    
                    ofVec2f repulseForce = directionVec.operator*=(predator.repulsion * -predator.character);
                    acceleration.operator+=(repulseForce);
                }
            }
            
        }
        
        // before stevens idea
        /*
         for(int i=0; i < predatorsREF->size(); i++){
         Predator predator = predatorsREF->at(i);
         float distSquared = position.distanceSquared(predator.head);
         if(distSquared < predator.reactionDist * predator.reactionDist){ // see if within range
         ofVec2f directionVec = position.operator-(predator.head).normalize();
         if(predator.character>0){ // attract boids
         ofVec2f attractForce = directionVec.operator*=(-predator.attraction * predator.character*sqrt(distSquared)/predator.reactionDist);
         acceleration.operator+=(attractForce);
         
         }else if(predator.character == 0){
         // do not attract
         }else{ // fear boids
         ofVec2f repulseForce = directionVec.operator*=(predator.repulsion * -predator.character);
         acceleration.operator+=(repulseForce);
         }
         }
         }*/
    }
    
}
void Boid::noWalls(){
    if(position.x > worldSize) position.x = 0;
    if(position.y > worldSize) position.y = 0;
    
    if(position.x < 0) position.x = worldSize;
    if(position.y < 0) position.y = worldSize;
    
}

void Boid::checkWalls(){
    
    
    // Rectangle Walls
    
    if( position.x+radius >= worldSize && -(velocity.x+acceleration.x) < 0){
      //  velocity.x = -velocity.x;
      //  acceleration.x = -acceleration.x;
        collideWall = true;
        position.x = 0;

    }
    else if( position.y+radius >= worldSize && -(velocity.y+acceleration.y) < 0 ){
        velocity.y = -velocity.y;
        acceleration.y = -acceleration.y;
        collideWall = true;
        
    }
    else if( position.x-radius <= 0 && -(velocity.x+acceleration.x) > 0) {
      //  velocity.x = -velocity.x;
      //  acceleration.x = -acceleration.x;
        collideWall = true;
        
        position.x = worldSize;

    }
    else if( position.y-radius <= 0 && -(velocity.y+acceleration.y) > 0){
        velocity.y = -velocity.y;
        acceleration.y = -acceleration.y;
        collideWall = true;
        
        
    } else {
        collideWall = false;
    }
    
}

void Boid::checkPredator(){
    
    // check collision with Predator
    if(!predatorsREF->empty()){
        for(int i=0; i < predatorsREF->size(); i++){
            Predator predator = predatorsREF->at(i);
            ofVec2f posAfterUpdate = position.operator+(velocity).operator+(acceleration);
            ofRectangle boundingBox = predator.shape.getBoundingBox();
            ofVec2f point;
            if(boundingBox.inside(posAfterUpdate)){
                // is inside boundingbox
                if(predator.shape.inside(posAfterUpdate) ){
                    // is inside shape
                    if(flagCollidePredator){
                        collidePredator = false;
                    }else{
                        collidePredator = true;
                        flagCollidePredator = true;
                    }
                    
                    /*
                     //Find closest two Points
                     vector<ofPoint> vertices = predator.shape.getVertices();
                     float nearestSquared = posAfterUpdate.distanceSquared(vertices[0]);
                     indxNearest = 0;
                     for(int j = 0; j < vertices.size(); j++){
                     float distSquared = posAfterUpdate.distanceSquared(vertices[j]);
                     if(nearestSquared > distSquared){
                     nearestSquared = distSquared;
                     indxNearest = j;
                     }
                     }
                     float distSquaredNext = posAfterUpdate.distanceSquared(vertices[ (indxNearest+1) % vertices.size() ]);
                     float distSquaredPrev = posAfterUpdate.distanceSquared(vertices[ (indxNearest-1) % vertices.size() ]);
                     
                     indx2ndNearest;
                     if(distSquaredNext < distSquaredPrev ) indx2ndNearest = (indxNearest+1) % vertices.size();
                     if(distSquaredNext > distSquaredPrev ){
                     indx2ndNearest = (indxNearest-1) % vertices.size();
                     if(indxNearest == 0) indx2ndNearest = vertices.size()-1;
                     }
                     
                     // calculate perpendicular (normal)
                     ofVec2f normal;
                     normal = vertices[indxNearest].operator-(vertices[indx2ndNearest]);
                     normal.normalize();
                     normal.perpendicular();
                     */
                    
                    ofPoint pointOnPoly = predator.shape.getClosestPoint(posAfterUpdate);
                    ofVec2f normal;
                    normal = posAfterUpdate.operator-(pointOnPoly);
                    normal.normalize();
                    
                    
                    //calculate the speed that will be transfered in a collision
                    ofVec2f currentSpeed1 = velocity.operator+(acceleration);
                    //  ofVec2f currentSpeed2 = predator.velocity;
                    float transferVelocity_1 = currentSpeed1.dot(normal);
                    // float transferVelocity_2 = currentSpeed2.dot(normal);
                    
                    float factor = 1.5; // artificial push of the collision
                    acceleration.operator+=(normal.operator*(-transferVelocity_1*factor));//-transferVelocity_2));
                    
                    
                    
                    
                    // displace those who are allready inside
                    if(boundingBox.inside(position)){
                        // distance to Nearest edge
                        /*ofVec2f A = vertices[indx2ndNearest].operator-(vertices[indxNearest]);
                         ofVec2f B = position.operator-(vertices[indxNearest]);
                         float amtOnA = A.normalized().dot(B.normalized());
                         A.operator*=(amtOnA);
                         ofVec2f displacement = B.operator-(A);
                         position.operator+=(displacement);
                         */
                        position = predator.shape.getClosestPoint(position);
                    }
                }
                
            }else{
                collidePredator = false;
                flagCollidePredator = false;
            }
        }
        
    }
}


void Boid::checkCollisions(){
    bool collide;
    bool noCollisions = false;
    int rounds = 0;
    while(noCollisions == false){
        collide = false;
        for(int i=0; i < boidsREF->size(); i++){
            rounds++;
            if(i!=ownIndx){
                if(detectCollision(i)){
                    collide = true;
                    
                }
            }
        }
        if(collide == false || rounds >= 5) noCollisions = true;
    }
    
}

bool Boid::detectCollision(int i){
    ofVec2f currentPos = position.operator+(acceleration);
    ofVec2f currentPos2 = boidsREF->at(i).position.operator+(boidsREF->at(i).acceleration);
    
    // very rough measure if they could collide to reduce calculations
    if(abs(currentPos.x-currentPos2.x)>(2*maxSpeed+radius*2)) return false;
    if(abs(currentPos.y-currentPos2.y)>(2*maxSpeed+radius*2)) return false;
    
    float minimumDistance = radius+boidsREF->at(i).radius;
    
    if(getDistancePow2(currentPos, currentPos2) > minimumDistance*minimumDistance) return false; // check if near enough to collide
    
    ofVec2f collisionVector = currentPos.operator-(currentPos2);
    float distance = collisionVector.length();
    
    collisionVector.normalize();
    
    
    // failure correction, if two Boids are overlapping, they are added a acceleration which will replace them to not overlap
    if(distance < minimumDistance){
        acceleration.operator+=(collisionVector.operator*((minimumDistance-distance)/2));
        boidsREF->at(i).acceleration.operator+=(collisionVector.operator*(-(minimumDistance-distance)/2));
        return true;
    }
    
    
    // if the Boids are way to far away to collide, interrupt
    if(distance > (2 * maxSpeed) + minimumDistance){
        return false;
        
    }
    
    // calculate the speed that will be transfered in a collision
    ofVec2f currentSpeed1 = velocity.operator+(acceleration);
    ofVec2f currentSpeed2 = boidsREF->at(i).velocity.operator+(boidsREF->at(i).acceleration);
    float transferVelocity_1 = currentSpeed1.dot(collisionVector);
    float transferVelocity_2 = currentSpeed2.dot(collisionVector);
    
    // if the Boids are moveng away from each other, interrupt
    if (transferVelocity_1 - transferVelocity_2 > 0){
        return false;
    }
    // if the distance including the movement towards each other will be smaller than the minimum distance, they collide
    if ( distance - transferVelocity_1 - transferVelocity_2 < minimumDistance){
        acceleration.operator+=(collisionVector.operator*(transferVelocity_2-transferVelocity_1));
        boidsREF->at(i).acceleration.operator+=(collisionVector.operator*(transferVelocity_1-transferVelocity_2));
        boidsREF->at(i).checkCollisions(); // check for chain reactions
        return true;
    }
    
    return false;
    
}


void Boid::checkCollisionsAndAttraction(){
    bool collide;
    bool noCollisions = false;
    bool attractionFlag = true;
    int rounds = 0;
    collided = false;
    while(noCollisions == false){
        collide = false;
        for(int i=0; i < boidsREF->size(); i++){
            rounds++;
            if(i!=ownIndx){
                if(detectCollisionAddAttraction(i, attractionFlag)){
                    collide = true;
                    collided = true;
                }
            }
            if(rounds > 1) attractionFlag = false;
        }
        if(collide == false || rounds >= 5) noCollisions = true;
    }
    
}

bool Boid::detectCollisionAddAttraction(int i, bool _attractionFlag){
    ofVec2f currentPos = position.operator+(acceleration);
    ofVec2f currentPos2 = boidsREF->at(i).position.operator+(boidsREF->at(i).acceleration);
    
    int minDistanceAttraction = 40;
    
    // very rough measure if they could collide to reduce calculations
    if(minDistanceAttraction > (2*maxSpeed+radius*2)){
        if(abs(currentPos.x-currentPos2.x)>minDistanceAttraction) return false;
        if(abs(currentPos.y-currentPos2.y)>minDistanceAttraction) return false;
    }else{
        if(abs(currentPos.x-currentPos2.x)>(2*maxSpeed+radius*2)) return false;
        if(abs(currentPos.y-currentPos2.y)>(2*maxSpeed+radius*2)) return false;
    }
    
    
    if(getDistancePow2(currentPos, currentPos2) > minDistanceAttraction*minDistanceAttraction) return false; // check if near enough to at least beeing attracted;
    
    ofVec2f collisionVector = currentPos.operator-(currentPos2);
    float distance = collisionVector.length();
    
    collisionVector.normalize();
    float minimumDistance = radius+boidsREF->at(i).radius;
    
    
    // failure correction, if two Boids are overlapping, they are added a acceleration which will replace them to not overlap
    if(distance < minimumDistance){
        acceleration.operator+=(collisionVector.operator*((minimumDistance-distance)/2));
        boidsREF->at(i).acceleration.operator+=(collisionVector.operator*(-(minimumDistance-distance)/2));
        return true;
    }
    
    // Attraction
    
    if(distance < minDistanceAttraction){// && _attractionFlag){ // Optional minimaldistance to calculate
        ofVec2f attractionForce = collisionVector.operator*=(attractionStrength*(mass*boidsREF->at(i).mass)/(pow(distance,2)));
        
        acceleration.operator+=(-attractionForce.operator/(2));
    }
    
    // if the Boids are way to far away to collide, interrupt
    if(distance > (2 * maxSpeed) + minimumDistance){
        return false;
        
    }
    
    // calculate the speed that will be transfered in a collision
    ofVec2f currentSpeed1 = velocity.operator+(acceleration);
    ofVec2f currentSpeed2 = boidsREF->at(i).velocity.operator+(boidsREF->at(i).acceleration);
    float transferVelocity_1 = currentSpeed1.dot(collisionVector);
    float transferVelocity_2 = currentSpeed2.dot(collisionVector);
    
    // if the Boids are moveng away from each other, interrupt
    if (transferVelocity_1 - transferVelocity_2 > 0){
        return false;
    }
    // if the distance including the movement towards each other will be smaller than the minimum distance, they collide
    if ( distance - transferVelocity_1 - transferVelocity_2 < minimumDistance){
        acceleration.operator+=(collisionVector.operator*(transferVelocity_2-transferVelocity_1));
        boidsREF->at(i).acceleration.operator+=(collisionVector.operator*(transferVelocity_1-transferVelocity_2));
        boidsREF->at(i).checkCollisions(); // check for chain reactions
        return true;
    }
    
    return false;
    
}

float Boid::getDistancePow2(ofVec2f pos1, ofVec2f pos2){
    return (pos1.x-pos2.x)*(pos1.x-pos2.x) + (pos1.y-pos2.y)*(pos1.y-pos2.y);
    
}

void Boid::flock()
{
    
    ofVec2f sep = separate();   // Separation
    ofVec2f ali = align();      // Alignment
    ofVec2f coh = cohesion();   // Cohesion
    // Arbitrarily weight these forces
    sep.operator*=(weightSep);
    ali.operator*=(weightAli);
    coh.operator*=(weightCoh);
    // Add the force vectors to acceleration
    
    acceleration.operator+=(sep);
    acceleration.operator+=(ali);
    acceleration.operator+=(coh);
    
}

ofVec2f Boid::getVelocity()
{
    return velocity;
}

ofVec2f Boid::getPosition()
{
    return position;
}
ofVec2f Boid::getAcceleration()
{
    return acceleration;
}
void Boid::addAcceleration(ofVec2f acceleration_) {
    acceleration.operator+=(acceleration_);
}



// A method that calculates and applies a steering force towards a target
// STEER = DESIRED MINUS VELOCITY
ofVec2f Boid::seek(ofVec2f target)
{
    // ofVec2f loc = getWorldCenter();
    ofVec2f desired = target.operator-(position);//target.sub(loc);  // A vector pointing from the location to the target
    
    // If the magnitude of desired equals 0, skip out of here
    if (desired.x == 0 && desired.y == 0) return ofVec2f(0,0);
    
    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired.operator*=(maxSpeed);
    // Steering = Desired minus Velocity
    
    ofVec2f steer = desired.operator-(velocity);
    
    float len = steer.length();
    if (len > maxForce) {
        steer.normalize();
        steer.operator*=(maxForce);
    }
    return steer;
}

ofVec2f Boid::separate()
{
    ofVec2f steer = ofVec2f(0,0);
    int count = 0;
    // For every boid in the system, check if it's too close
    ofVec2f locA = position;
    for ( int i = 0; i< boidsREF->size(); i++)
    {
        
        ofVec2f locB = boidsREF->at(i).position;
        
        
        float d = locA.squareDistance(locB);
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation*desiredseparation)) {
            // Calculate vector pointing away from neighbor
            ofVec2f diff = locA.operator-(locB);
            diff.normalize();
            diff.operator*=(1.0/sqrt(d));        // Weight by distance
            steer.operator+=(diff); // addLocal
            count++;            // Keep track of how many
        }
        // }
    }
    // Average -- divide by how many
    if (count > 0) {
        steer.operator*=(1.0/count);//mulLocal(1.0/count);
    }
    
    // As long as the vector is greater than 0
    if (steer.lengthSquared() > 0) {
        // Implement Reynolds: Steering = Desired - Velocity
        steer.normalize();
        steer.operator*=(maxSpeed);//mulLocal(maxSpeed);
        steer.operator-=(velocity);
        float len = steer.length();
        if (len > maxForce) {
            steer.normalize();
            steer.operator*=(maxForce);
        }
    }
    
    return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
ofVec2f Boid::align () {
    
    ofVec2f steer = ofVec2f(0,0);
    int count = 0;
    
    ofVec2f locA = position; //body.getWorldCenter();
    for ( int i = 0; i< boidsREF->size(); i++)
    {
        
        ofVec2f locB = boidsREF->at(i).position;
        
        
        float d = locA.squareDistance(locB);
        if ((d > 0) && (d < neighbourdist*neighbourdist)) {
            steer.operator+=(boidsREF->at(i).velocity);
            count++;
        }
        //  }
    }
    // Average -- divide by how many
    if (count > 0) {
        steer.operator*=(1.0/count);//mulLocal(1.0/count);
    }
    
    // As long as the vector is greater than 0
    if (steer.lengthSquared() > 0) {
        // Implement Reynolds: Steering = Desired - Velocity
        steer.normalize();
        steer.operator*=(maxSpeed);//mulLocal(maxSpeed);
        steer.operator-=(velocity);
        float len = steer.length();
        if (len > maxForce) {
            steer = steer.getNormalized();
            steer.operator*=(maxForce);
        }
    }
    return steer;
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofVec2f Boid::cohesion () {
    ofVec2f sum = ofVec2f(0,0);   // Start with empty vector to accumulate all locations
    int count = 0;
    ofVec2f locA = position; //body.getWorldCenter();
    for ( int i = 0; i< boidsREF->size(); i++)
    {
        
        ofVec2f locB = boidsREF->at(i).position;
        
        
        float d = locA.squareDistance(locB);
        
        if ((d > 0) && (d < neighbourdist*neighbourdist)) {
            sum.operator+=(locB); // Add location
            count++;
        }
        //   }
    }
    if (count > 0) {
        sum.operator*=(1.0/count);
        return seek(sum);  // Steer towards the location
    }
    return sum;
}

void Predator::draw(){
    
    shape.draw();
    ofSetColor(255,0,0);
    ofDrawCircle(head, 10);
    
}
