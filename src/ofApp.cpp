#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync( true );
    ofSetBackgroundAuto(false);
    
    wave_poles.setup();
    wave_ramp.setup();
    wave_poles.updateResponse(attack, damping);
    wave_ramp.updateResponse(attack, damping);
    
    
    grafic_RGB_Pole.allocate(NUM_ARRAY, RENDER_HEIGHT_POLE, GL_RGBA32F);
    grafic_RGB_Ramp.allocate(NUM_ARRAY, RENDER_HEIGHT_RAMP, GL_RGBA32F);

    grafic_W_Pole.allocate(NUM_ARRAY, RENDER_HEIGHT_POLE, GL_RGBA32F);
    grafic_W_Ramp.allocate(NUM_ARRAY, RENDER_HEIGHT_RAMP,GL_RGBA32F);
    
    render.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE*2);
    syphonRenderOut.setName("Waves");
    syphonTex.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE*2, GL_RGBA);
    
    
    syphonTex = render.getTexture();
    
    grafic_RGB_Pole.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_RGB_Pole.end();
    grafic_RGB_Ramp.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_RGB_Ramp.end();
    
    grafic_W_Pole.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_W_Pole.end();
    grafic_W_Ramp.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_W_Ramp.end();
    
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render.end();
    
    visualControl.setName("visualControl");
    visualControl.add(inLeft.set("inLeft", 0., -1., 1.));
    visualControl.add(inRight.set("inRight", 0., -1., 1.));
    visualControl.add(damping.set("damping", 0.96, 0.8, 1.));
    visualControl.add(attack.set("attack", 0.04, 0., .2));
    visualControl.add(noiseAmt.set("noiseAmt", 0.15, 0., 1));
    
    visualControl.add(posHWavePoles.set("posHWavePoles", 0.5, 0., 1));
    visualControl.add(posHWaveRamp.set("posHWaveRamp", 0.5, 0., 1));
    visualControl.add(posHLinePoles.set("posHLinePoles", 0.5, 0, 1));
    visualControl.add(posHLineRamp.set("posHLineRamp", 0.5, 0, 1));
    
    visualControl.add(colorLine.set("colorLine", ofColor(255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGTop.set("colorBGTop", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGBot.set("colorBGBot", ofColor(255,179,66,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorWaveTop.set("colorWaveTop", ofColor(0,105,255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorWaveBot.set("colorWaveBot", ofColor(0,0,55), ofColor(0,0),ofColor(255)));
    
    
    
    
    
    // Blinkende Lygter
    graficBlinkendeLygter.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE);
    
    // Control
    paramBlinkendeLyter.setName("BlinkendeLygter");
    paramBlinkendeLyter.add(intensityBlink.set("intensityBlink", 0.0, 0.0, 1.0));
    paramBlinkendeLyter.add(tempo.set("tempo", 0.02, 0.0001, 0.1));
    paramBlinkendeLyter.add(hard_soft.set("hard/soft", true));
    //paramBlinkendeLyter.add(colorLygter.set("colorLygter", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    visualControl.add(paramBlinkendeLyter);
    
    // Boubbles
    graficBoubbles.allocate(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE);
    
    // Control
    paramBoubbles.setName("Boubbles");
    paramBoubbles.add(intensityBoubbles.set("intensityBoubbles", 0.0, 0.0, 1.0));
    paramBoubbles.add(velMin.set("velMin", 0.5, 0.0, 5.0));
    paramBoubbles.add(velMax.set("velMax", 2., 0.0, 5.0));
    //paramBoubbles.add(colorBoubbles.set("colorBoubbles", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    visualControl.add(paramBoubbles);

    
    gui.setup(visualControl);

    
    // SETUP FOR OSC
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    
    colorLygter = ofColor(255);
    colorBoubbles = ofColor(255);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    syncOSC.update();
    
    float noiseLeft = noiseAmt*(ofNoise(ofGetElapsedTimef()*4)-0.5);
    float noiseRight = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+300.)-0.5);
    float noiseRight_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+600.)-0.5);
    float noiseLeft_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+900.)-0.5);
    
    wave_poles.update(inLeft+noiseLeft, inRight+noiseRight);
    wave_ramp.update( inRight+noiseRight_ramp, inLeft+noiseLeft_ramp);
    wave_poles.updateResponse(attack, damping);
    wave_ramp.updateResponse(attack, damping);
    
    
    // moved to Update, to be run when window no tactive
    
    grafic_RGB_Pole.begin();
    ofRectGradient(0,0,grafic_RGB_Pole.getWidth(), grafic_RGB_Pole.getHeight(), colorBGTop, colorBGBot, OF_GRADIENT_LINEAR);
    wave_poles.drawGradient(0,0,grafic_RGB_Pole.getWidth(), grafic_RGB_Pole.getHeight(), colorWaveTop, colorWaveBot, posHWavePoles);
    grafic_RGB_Pole.end();
    
    grafic_RGB_Ramp.begin();
    ofRectGradient(0,0,grafic_RGB_Ramp.getWidth(), grafic_RGB_Ramp.getHeight(), colorBGTop, colorBGBot, OF_GRADIENT_LINEAR);
    wave_ramp.drawGradient(0,0,grafic_RGB_Ramp.getWidth(), grafic_RGB_Ramp.getHeight(), colorWaveTop, colorWaveBot, posHWaveRamp);
    grafic_RGB_Ramp.end();
    
    grafic_W_Pole.begin();
    ofBackground(0,255);
    wave_poles.drawLine(0,0,grafic_W_Pole.getWidth(), grafic_W_Pole.getHeight(), colorLine, posHLinePoles, LINE_WIDTH);
    grafic_W_Pole.end();
    
    grafic_W_Ramp.begin();
    ofBackground(0,255);
    wave_ramp.drawLine(0,0,grafic_W_Ramp.getWidth(), grafic_W_Ramp.getHeight(),colorLine, posHLineRamp, LINE_WIDTH);
    grafic_W_Ramp.end();
    

    
    // Blinkedne Lygter
    if(ofRandom(100000)/100000 < intensityBlink){
        Lygte lygte;
        lygte.lygteColor = colorLygter;
        lygte.location = ofVec2f(((int)ofRandom(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP)),(int)ofRandom(RENDER_HEIGHT_POLE));
        lygte.tempo = tempo;
        lygte.hard_soft = hard_soft;
        
        lygter.push_back(lygte);
    }
    
    for (vector<Lygte>::iterator it=lygter.begin(); it!=lygter.end();)    {
        it->update();
        if(it->isDead())
            it = lygter.erase(it);
        else
            ++it;
    }
    
    graficBlinkendeLygter.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < lygter.size(); i++){
        lygter[i].draw();
    }
    graficBlinkendeLygter.end();
    
    
    // Boubbles
    if(ofRandom(100000)/100000 < intensityBoubbles){
        Boubble boubble;
        boubble.boubbleColor = colorBoubbles;
        boubble.location = ofVec2f(((int)ofRandom(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP)),RENDER_HEIGHT_POLE);
        boubble.velocity = ofVec2f(0., - ofRandom(velMin   , velMax));
        
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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < boubbles.size(); i++){
        boubbles[i].draw();
    }
    graficBoubbles.end();
    
    
    
    
    
    // Draw the content into the renderer for syphon publishing.
    
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ofSetColor(255,255);
    
    grafic_RGB_Pole.draw(0,0, RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE);
    grafic_RGB_Ramp.draw(RENDER_WIDTH_POLE,0, RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP);
    
    grafic_W_Pole.draw(0,RENDER_HEIGHT_POLE, RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE);
    grafic_W_Ramp.draw(RENDER_WIDTH_POLE,RENDER_HEIGHT_POLE, RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP);
    
    graficBlinkendeLygter.draw(0,RENDER_HEIGHT_POLE);
    graficBoubbles.draw(0,RENDER_HEIGHT_POLE);

    render.end();

    
    
    

    syphonRenderOut.publishTexture(&syphonTex);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //ofSetColor(255);
    //render.draw(0,0, RENDER_WIDTH, RENDER_HEIGHT);
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
        ofVec2f center(w / 2, h / 2);
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
        gradientMesh.addVertex(ofVec2f(w, py));
        gradientMesh.addVertex(ofVec2f(w, h));
        gradientMesh.addVertex(ofVec2f(px, h));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
    } else if(mode == OF_GRADIENT_BAR) {
        gradientMesh.addVertex(ofVec2f(w / 2, h / 2));
        gradientMesh.addVertex(ofVec2f(px, h / 2));
        gradientMesh.addVertex(ofVec2f(px, py));
        gradientMesh.addVertex(ofVec2f(w, py));
        gradientMesh.addVertex(ofVec2f(w, h / 2));
        gradientMesh.addVertex(ofVec2f(w, h));
        gradientMesh.addVertex(ofVec2f(px, h));
        gradientMesh.addVertex(ofVec2f(px, h / 2));
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
//------------------------SPRING ARRAY--------------------------
void WaveParticleSystem::setup(){
    
    for(int i = 0; i<NUM_ARRAY; i++){
        WaveParticle wP;
        wP.p = 0.;
        
        waveParticles.push_back(wP);
        
    }
}

void WaveParticleSystem::update(float _inLeft, float _inRight){
    
    waveParticles[0].p = _inLeft;
    
    
    //cout << ofToString(waveParticles[30].a)+ "\n";
    
    waveParticles.back().p = _inRight;
    
    
    
    for(int i = 1; i<NUM_ARRAY-1; i++){
        
        waveParticles[i].update(waveParticles[i-1].p, waveParticles[i+1].p);
        int i2 = NUM_ARRAY-1-i;
        waveParticles[i2].update(waveParticles[i2-1].p, waveParticles[i2+1].p);
        
    }
    
}

void WaveParticleSystem::updateResponse(float _attack, float _damping){
    
    for(int i = 0; i<NUM_ARRAY; i++){
        waveParticles[i].damping = _damping;
        waveParticles[i].attack  = _attack;
        
    }
}

void WaveParticleSystem::drawLine(int _x, int _y, int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    
    
    if(false){ // BEIZER
        ofSetLineWidth(_lineWidth);
        ofSetColor(colorLine);
        ofBeginShape();
        ofNoFill();
        float dist = _w / (NUM_ARRAY-2);
        vector<ofPoint> points;
        for(int i = 0; i < NUM_ARRAY; i++){
            ofPoint point = ofVec2f(dist*i , (waveParticles[i].p/2+1-_posHLine)*_h);
            points.push_back(point);
        }
        
        ofCurveVertices(points);
        ofEndShape();
    }
    
    if(true){
        ofSetLineWidth(_lineWidth);
        ofSetColor(colorLine);
        float dist = _w / (NUM_ARRAY-2);
        
        ofPolyline line;
        


        for(int i = 0; i < NUM_ARRAY; i++){

            float x = dist*i;
            float y = (waveParticles[i].p/2+1-_posHLine)*_h;
            line.addVertex(ofVec2f(x,y));
        }
        line.draw();
    }
    ofPopMatrix();
    
    
}

void WaveParticleSystem::drawGradient(int _x, int _y, int _w, int _h, ofColor colorWaveTop, ofColor colorWaveBot, float _posHWave ){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    float dist = _w/(NUM_ARRAY);
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofFloatColor colorWaveTop_f = colorWaveTop;
    ofFloatColor colorWaveBot_f = colorWaveBot;
    
    
    
    for(int i = 0; i <= NUM_ARRAY; i++){
        
        mesh.addVertex(ofVec2f(dist*i, (waveParticles[i].p/2+1-_posHWave)*_h));
        mesh.addColor(colorWaveTop_f);
        
        mesh.addVertex(ofPoint(dist*i, _h));
        mesh.addColor(colorWaveBot_f);
        
    }
    for(int i = 0; i < NUM_ARRAY-1; i++){
        mesh.addIndex(i*2+1);
        mesh.addIndex(i*2+2);
        mesh.addIndex(i*2+0);
        
        mesh.addIndex(i*2+1);
        mesh.addIndex(i*2+3);
        mesh.addIndex(i*2+2);
        
    }
    mesh.draw();
    ofPopMatrix();
    
}

