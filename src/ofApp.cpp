#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync( true );
    ofSetBackgroundAuto(false);
    
    wave_poles.setup(NUM_ARRAY);
    wave_ramp.setup(NUM_ARRAY);
    wave_poles.updateResponse(ATTACK, DAMPING);
    wave_ramp.updateResponse(ATTACK, DAMPING);
    
    fboTexPoles.allocate(RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE);
    fboTexRamp.allocate(RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP);
    
    render.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE);
    syphonRenderOut.setName("Waves");
    syphonTex.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE, GL_RGBA);
    
    
    syphonTex = render.getTexture();
    
    
    render.begin();
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    render.end();
    
    WaveControl.setName("WaveControl");
    WaveControl.add(inLeft.set("inLeft", 0., -1., 1.));
    WaveControl.add(inRight.set("inRight", 0., -1., 1.));
    WaveControl.add(noiseAmt.set("noiseAmt", 0.15, 0., 1));
    
    WaveControl.add(posHWavePoles.set("posHWavePoles", 0.5, 0., 2));
    WaveControl.add(posHWaveRamp.set("posHWaveRamp", 0.5, 0., 2));
    WaveControl.add(posHLinePoles.set("posHLinePoles", 0.5, 0, 2));
    WaveControl.add(posHLineRamp.set("posHLineRamp", 0.5, 0, 2));
    
    WaveControl.add(colorTopTop.set("colorTopTop", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));
    WaveControl.add(colorTopBot.set("colorTopBot", ofColor(255,179,66,255), ofColor(0,0),ofColor(255)));
    WaveControl.add(colorBotTop.set("colorBotTop", ofColor(0,105,255,255), ofColor(0,0),ofColor(255)));
    WaveControl.add(colorBotBot.set("colorBotBot", ofColor(0,0,55), ofColor(0,0),ofColor(255)));
    WaveControl.add(colorLine.set("colorLine", ofColor(255,255), ofColor(0,0),ofColor(255)));
    
    
    
    
    
    // Blinkende blinks
    graficBlinkendeLygter.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE);
    
    // Control
    WaveControl.add(blinkIntensity.set("blinkIntensity", 0.0, 0.0, 1.0));
    WaveControl.add(blinkTempo.set("blinkTempo", 0.02, 0.0001, 0.1));
    WaveControl.add(hard_soft.set("hard/soft", true));
    WaveControl.add(colorBlink.set("colorBlink", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    
    // Boubbles
    graficBoubbles.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE);
    
    // Control
    WaveControl.add(boubblesIntensity.set("boubblesIntensity", 0.0, 0.0, 1.0));
    WaveControl.add(boubblesVelMin.set("boubblesVelMin", 0.5, 0.0, 5.0));
    WaveControl.add(boubblesVelMax.set("boubblesVelMax", 2., 0.0, 5.0));
    WaveControl.add(colorBoubbles.set("colorBoubbles", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    
    // Dancing Lines
    
    counterDLines = 0;
    
    // Control
    WaveControl.add(dLinesSpeed.set("dLinesSpeed", .2, 0.0, 1));
    WaveControl.add(dLinesSync.set("dLinesSync", .9, 0.5, 1));
    
    WaveControl.add(colorDLines.set("colorDLines", ofColor(0,255), ofColor(0,0), ofColor(255,255)));
    
    for(int i = 0; i < 10; i++){
        DancingLine newLine;
        newLine.location1 = ofVec2f(0, fboTexPoles.getHeight()/2);
        newLine.location2 = ofVec2f(fboTexPoles.getWidth(), fboTexPoles.getHeight()/2);
        newLine.lineColor = colorDLines;
        newLine.freedom = fboTexPoles.getHeight()/2;
        
        dancingLinesPole.push_back(newLine);
    }
    for(int i = 0; i < 10; i++){
        DancingLine newLine;
        newLine.location1 = ofVec2f(0, fboTexRamp.getHeight()/2);
        newLine.location2 = ofVec2f(fboTexRamp.getWidth(), fboTexRamp.getHeight()/2);
        newLine.lineColor = colorDLines;
        newLine.freedom = fboTexRamp.getHeight()/2;
        
        dancingLinesRamp.push_back(newLine);
    }
    
    
    // Drops
    WaveControl.add(dropsVelMin.set("dropsVelMin", 0.001, 0.0, .2));
    WaveControl.add(dropsVelMax.set("dropsVelMax", 0.1, 0.0, .5));
    WaveControl.add(dropsDragLimit.set("dropsDragLimit", 0.1, 0.0, .5));
    WaveControl.add(dropsExtraMass.set("dropsExtraMass", 0.01, 0.0, .1));
    
    
    WaveControl.add(colorDrops.set("colorDrops", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    
    
    
    // setup the control panel
    Flock.setName("Flock");
    Flock.add(flockIntensity.set("flockIntensity", 0., 0. , 1.));
    Flock.add(flockIntensityDay.set("flockIntensityDay", 0., 0. , 1.));
    
    Flock.add(maxForce.set("maxForce", 0.5, 0.1 , 10));
    Flock.add(maxSpeed.set("maxSpeed", 2, 1 , 10));
    Flock.add(desiredSeparation.set("desiredSeparation", 20, 0 , 100));
    Flock.add(neighbourDist.set("neighbourDist", 27, 0 , 100));
    Flock.add(weightSeparation.set("weightSeparation", 1.5, 0. , 2.));
    Flock.add(weightAlign.set("weightAlign", 1, 0. , 2.));
    Flock.add(weightCohesion.set("weightCohesion", 1, 0. , 2.));
    Flock.add(numOfBoids.set("numOfBoids", 35, 1, 500));
    Flock.add(colorFlock.set("colorFlock", ofColor(0,255), ofColor(0,0), ofColor(255,255)));
    
    
    
    
    
    WaveControl.add(Flock);
    
    // Setup GUI
    WaveControl.add(updateRequest.set("updateRequest",0, 0, 1));
    
    gui.setup(WaveControl);
    
    gui.minimizeAll();
    
    
    // Setup Flock
    flockSystem.setup(numOfBoids, ofVec2f(0,0), RENDER_HEIGHT_POLE);
    flockSystem.setAllDesiredsep(desiredSeparation);
    // flockSystem.setAllMax(maxForce, maxSpeed);
    flockSystem.setAllNeighbourdist(neighbourDist);
    flockSystem.setAllWheigts(weightSeparation, weightSeparation, weightCohesion);
    
    // flockSystem.startThread();
    
    
    // SETUP FOR OSC
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    
    // colorBlink = ofColor(255);
    // colorBoubbles = ofColor(255);
    
    updateRequest = 1;
    syncOSC.update();
    updateRequest = 0;
    
    
    
    
    
    // osc receiver to make soundWaves
    
    receiveOscTriggers.setup(TRIGGER_OSCRECEIVEPORT);
    
    sWSystem.setup(RENDER_WIDTH_SW, RENDER_HEIGHT_SW);
    
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    syncOSC.update();
    
    // waveSystem
    
    // Blinkedne blinks
    for(int i = 0; i < 10 ; i++){
        if(ofRandom(100000)/100000 < blinkIntensity){
            Blink blink;
            blink.blinkColor = colorBlink;
            blink.location = ofVec2f(((int)ofRandom(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP)),(int)ofRandom(RENDER_HEIGHT_POLE));
            blink.tempo = blinkTempo;
            blink.hard_soft = hard_soft;
            
            blinks.push_back(blink);
        }
    }
    
    for (vector<Blink>::iterator it=blinks.begin(); it!=blinks.end();)    {
        it->update();
        if(it->isDead())
            it = blinks.erase(it);
        else
            ++it;
    }
    
    graficBlinkendeLygter.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < blinks.size(); i++){
        blinks[i].draw();
    }
    graficBlinkendeLygter.end();
    
    
    // Boubbles
    if(ofRandom(100000)/100000 < boubblesIntensity){
        Boubble boubble;
        boubble.boubbleColor = colorBoubbles;
        boubble.location = ofVec2f(((int)ofRandom(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP)),RENDER_HEIGHT_POLE);
        boubble.velocity = ofVec2f(0., - ofRandom(boubblesVelMin   , boubblesVelMax));
        
        boubbles.push_back(boubble);
    }
    
    for (vector<Boubble>::iterator it=boubbles.begin(); it!=boubbles.end();)    {
        it->update();
        
        if(it->isDead())
            it = boubbles.erase(it);
        else
            ++it;
    }
    
    graficBoubbles.begin();
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i < boubbles.size(); i++){
            boubbles[i].draw();
        }
    }
    graficBoubbles.end();
    
    
    // DANCING LINES
    
    counterDLines += dLinesSpeed/100;
    
    
    for (vector<DancingLine>::iterator it=dancingLinesPole.begin(); it!=dancingLinesPole.end();)    {
        it->lineColor = colorDLines;
        it->update(counterDLines);
        it->sync = dLinesSync;
        ++it;
    }
    
    for (vector<DancingLine>::iterator it=dancingLinesRamp.begin(); it!=dancingLinesRamp.end();)    {
        it->lineColor = colorDLines;
        it->update(counterDLines);
        it->sync = dLinesSync;
        ++it;
    }
    
    
    // OSC soundWaveReceive
    // check for waiting messages
    while(receiveOscTriggers.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiveOscTriggers.getNextMessage(m);
        cout << "oscReceive: "<< m.getAddress() << " " << ofToString(m.getArgAsFloat(0)) << "\n";
        
        // check for mouse moved message
        if(m.getAddress() == "/soundWaves/impulseR/"){
            float  strength = m.getArgAsFloat(0);
            
            sWSystem.addSoundWave(ofVec2f(RENDER_WIDTH_SW, RENDER_HEIGHT_SW/2), strength);
        }
        // check for mouse button message
        else if(m.getAddress() == "/soundWaves/impulseL/"){
            float  strength = m.getArgAsFloat(0);
            
            sWSystem.addSoundWave(ofVec2f(0, RENDER_HEIGHT_SW/2), strength);
        }
        
        // check for mouse button message
        else if(m.getAddress() == "/waveSystem/addDrop/"){
            float  strength = m.getArgAsFloat(0);
            wave_poles.addDrop(ofMap(strength, 0., 1., dropsVelMin, dropsVelMax));
        }
        
    }
    // Update soundWaves
    sWSystem.update();
    
    
    
    // update Flock;
    // update the control Values
    flockSystem.maxSpeed = maxSpeed;
    flockSystem.maxForce = maxForce;
    flockSystem.neighbourdist = neighbourDist;
    flockSystem.desiredseparation = desiredSeparation;
    flockSystem.weightSep = weightSeparation;
    flockSystem.weightAli = weightAlign;
    flockSystem.weightCoh = weightCohesion;
    flockSystem.minimumDistance = desiredSeparation;
    flockSystem.numOfBoids = (int)numOfBoids;
    flockSystem.update();
    
    
    
    
    fboTexPoles.begin();
    {
        ofRectGradient(0, fboTexPoles.getHeight()/2, fboTexPoles.getWidth(), fboTexPoles.getHeight()/2, colorBotTop, colorBotBot, OF_GRADIENT_LINEAR);
        for(int i = 0; i < dancingLinesPole.size(); i++){
            dancingLinesPole[i].draw();
        }
        ofRectGradient(0,0, fboTexPoles.getWidth(), fboTexPoles.getHeight()/2, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
        
        if(flockIntensity > 0.){
            
            flockSystem.draw(0, fboTexPoles.getHeight()/2, fboTexPoles.getWidth(), fboTexPoles.getHeight()/2, ofColor(colorFlock->r, colorFlock->g, colorFlock->b,int(255*flockIntensity)));
        }
    }
    fboTexPoles.end();
    
    
    fboTexRamp.begin();
    {
        ofRectGradient(0, fboTexRamp.getHeight()/2, fboTexRamp.getWidth(), fboTexRamp.getHeight()/2, colorBotTop, colorBotBot, OF_GRADIENT_LINEAR);
        for(int i = 0; i < dancingLinesRamp.size(); i++){
            dancingLinesRamp[i].draw();
        }
        ofRectGradient(0,0, fboTexRamp.getWidth(), fboTexRamp.getHeight()/2, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    }
    fboTexRamp.end();
    
    //
    //    wave_poles.dropsVelMin = dropsVelMin;
    //    wave_poles.dropsVelMax = dropsVelMax;
    wave_poles.dropsDragLimit = dropsDragLimit;
    wave_poles.dropsExtraMass = dropsExtraMass;
    
    // draw the Waves
    float noiseLeft = noiseAmt*(ofNoise(ofGetElapsedTimef()*4)-0.5);
    float noiseRight = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+300.)-0.5);
    float noiseRight_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+600.)-0.5);
    float noiseLeft_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+900.)-0.5);
    
    wave_poles.update(inLeft+noiseLeft, inRight+noiseRight);
    wave_ramp.update( inRight+noiseRight_ramp, inLeft+noiseLeft_ramp);
    
    
    
    //    wave_poles.updateResponse(ATTACK, DAMPING);
    //    wave_ramp.updateResponse(ATTACK, DAMPING);
    
    
    // Draw the content into the renderer for syphon publishing.
    
    render.begin();
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ofSetColor(255,255);
        
        wave_poles.drawGradient(0,0,                RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, posHWavePoles, fboTexPoles.getTexture());
        wave_ramp.drawGradient( RENDER_WIDTH_POLE,0,RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP, posHWaveRamp, fboTexRamp.getTexture());
        float posWave = posHWavePoles;
        if(posHWavePoles < posHLinePoles) posWave = posHLinePoles;
        wave_poles.drawDrops(0,0,                RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, ofColor::white, posWave);
        
        
        
        // Line
        wave_poles.drawLine(0,0,                RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, colorLine, posHLinePoles, LINE_WIDTH);
        wave_ramp.drawLine( RENDER_WIDTH_POLE,0,RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP, colorLine, posHLineRamp, LINE_WIDTH);
        
        if(flockIntensityDay > 0.){
            // Poles
            flockSystem.draw(0,RENDER_HEIGHT_POLE*(1.-posHLinePoles),RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE-(RENDER_HEIGHT_POLE*(1.-posHLinePoles)), ofColor(colorFlock->r, colorFlock->g, colorFlock->b,int(255*flockIntensityDay)));
            // Poles
            flockSystem.draw(RENDER_WIDTH_POLE,RENDER_HEIGHT_RAMP*(1.-posHLineRamp),RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP-(RENDER_HEIGHT_RAMP*(1.-posHLineRamp)), ofColor(colorFlock->r, colorFlock->g, colorFlock->b,int(255*flockIntensityDay)));
        }
        
        graficBlinkendeLygter.draw(0,0);//RENDER_HEIGHT_POLE);
        graficBoubbles.draw(0,0);//RENDER_HEIGHT_POLE);
        
        sWSystem.draw(0,0, RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, true);
        sWSystem.draw(RENDER_WIDTH_POLE,0,RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP, false);
        
        ofSetColor(0,255);
        ofFill();
        ofDrawRectangle(RENDER_WIDTH_POLE,RENDER_HEIGHT_RAMP, RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE-RENDER_HEIGHT_RAMP);
        ofSetColor(255,255);
        ofRectGradient(RENDER_WIDTH_POLE,RENDER_HEIGHT_RAMP, RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE-RENDER_HEIGHT_RAMP, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    }
    render.end();
    
    syphonRenderOut.publishTexture(&syphonTex);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(150);
    gui.draw();
    ofSetWindowTitle("Varde WaveArray V1 FPS: "+ofToString((int)ofGetFrameRate()));
}

void ofApp::ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode){
    ofVboMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifndef TARGET_EMSCRIPTEN
#ifdef TARGET_OPENGLES
    if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
#else
    gradientMesh.setUsage(GL_STREAM_DRAW);
#endif
#endif
    if(mode == OF_GRADIENT_CIRCULAR) {
        // this could be optimized by building a single mesh once, then copying
        // it and just adding the colors whenever the function is called.
        ofVec2f center(w / 2 + px, h / 2 + py);
        gradientMesh.addVertex(center);
        gradientMesh.addColor(start);
        int n = 32; // circular gradient resolution
        float angleBisector = TWO_PI / (n * 2);
        float smallRadius = ofDist(0, 0, w / 2, h / 2);
        float bigRadius = smallRadius / cos(angleBisector);
        for(int i = 0; i <= n; i++) {
            float theta = i * TWO_PI / n;
            gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
            gradientMesh.addColor(end);
        }
    } else if(mode == OF_GRADIENT_LINEAR) {
        gradientMesh.addVertex(ofVec2f(px, py));
        gradientMesh.addVertex(ofVec2f(px+w, py));
        gradientMesh.addVertex(ofVec2f(px+w, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
    } else if(mode == OF_GRADIENT_BAR) {
        gradientMesh.addVertex(ofVec2f(px+w / 2, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px, py));
        gradientMesh.addVertex(ofVec2f(px+w, py));
        gradientMesh.addVertex(ofVec2f(px+w, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px+w, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h / 2));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
    }
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'd') wave_poles.addDrop(ofRandom(dropsVelMin, dropsVelMax));
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------


