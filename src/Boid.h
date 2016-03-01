//
//  classes.h
//  StatesOfMatter
//
//  Created by Jonas Fehr on 26.04.14.
//
//

#pragma once

#include "ofMain.h"
//#include "BoidSystem.h"
class Predator{
public:
    ofPolyline shape;
    float character;
    float attraction;
    float repulsion;
    float reactionDist;
    ofVec2f velocity = ofVec2f(0,0.01);
    ofVec2f head;
    
    void draw();
};


// Boid
class Boid{
public:
    Boid();
    void setup(float mass_, ofVec2f position_, ofVec2f velocity_, float _worldSize, float _maxSpeed, float _maxForce);
    void passEnvironment(vector <Boid> *boids_, vector <Predator> *predators_, int ownIndx_);

    void update();

    void draw();
    
    void addAcceleration(ofVec2f acceleration_);
    ofVec2f getAcceleration();
    ofVec2f getVelocity();

    ofVec2f getPosition();
    
    float mass;
    float radius;           // Boid's radius = mass * massSizeFactor
    float attractionStrength;
    ofVec2f position;       // current position of the Boid
    ofVec2f velocity;       // current velocity of the Boid
    ofVec2f acceleration;   // container for acceleration during one frame

    float worldSize;

    float maxSpeed;
    
    vector <Boid> *boidsREF;
    int ownIndx;
    bool collided;
    
    vector <Predator> *predatorsREF;
    
    // We need to keep track of a Body and a width and height
    
    float maxForce;    // Maximum steering force
    // float maxspeed;    // Maximum speed
    ofColor color;
    
    float desiredseparation = 3;
    float neighbourdist = 5;
    
    float weightSep = 1.3;
    float weightAli = 1;
    float weightCoh = 1;
    
    bool    debug = false;
    
    bool collideWall = false;
    bool collidePredator = false;
    bool flagCollidePredator = false;

    
private:
    void attraction();
    void noWalls();

    void checkWalls();
    void checkPredator();
    void predatorCharacter();
    void checkCollisions();
    bool detectCollision(int index);
    
    
    bool detectCollisionAddAttraction(int i, bool _attractionFlag);
    void checkCollisionsAndAttraction();
    
    float getDistancePow2(ofVec2f pos1, ofVec2f pos2);

    
    void flock();
    
    ofVec2f seek(ofVec2f target);
    ofVec2f separate();
    ofVec2f align ();
    ofVec2f cohesion ();

    int indxNearest, indx2ndNearest;


};

