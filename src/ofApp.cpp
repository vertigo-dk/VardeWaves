#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFrameRate(60);
    ofSetVerticalSync( true );
    ofSetBackgroundAuto(false);
    
    wave_poles.setup();
    wave_ramp.setup();
    wave_poles.updateResponse(attack, damping);
    wave_ramp.updateResponse(attack, damping);
    
    
    grafic_RGB.allocate(RENDER_WIDTH*2, RENDER_HEIGHT, GL_RGBA32F);
    grafic_W.allocate(RENDER_WIDTH*2, RENDER_HEIGHT, GL_RGBA32F);
    
    render.allocate(RENDER_WIDTH*2, RENDER_HEIGHT*2);
    syphonRenderOut.setName("Waves");
    syphonTex.allocate(RENDER_WIDTH*2, RENDER_HEIGHT*2, GL_RGBA);
    
    
    syphonTex = render.getTexture();
    
    grafic_RGB.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_RGB.end();
    grafic_W.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    grafic_W.end();
    
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render.end();
    
    /*
     #ifdef TARGET_OPENGLES
     shaderBlurX.load("shadersES2/shaderBlurX");
     shaderBlurY.load("shadersES2/shaderBlurY");
     #else
     if(ofIsGLProgrammableRenderer()){
     shaderBlurX.load("shadersGL3/shaderBlurX");
     shaderBlurY.load("shadersGL3/shaderBlurY");
     }else{
     shaderBlurX.load("shadersGL2/shaderBlurX");
     shaderBlurY.load("shadersGL2/shaderBlurY");
     }
     #endif
     
     */
    visualControl.setName("visualControl");
    visualControl.add(inLeft.set("inLeft", 0., -1., 1.));
    visualControl.add(inRight.set("inRight", 0., -1., 1.));
    visualControl.add(damping.set("damping", 0.96, 0.8, 1.));
    visualControl.add(attack.set("attack", 0.04, 0., .2));
    visualControl.add(noiseAmt.set("noiseAmt", 0.15, 0., 1));
    
    visualControl.add(posHWave.set("posHWave", 0.5, 0., 1));
    visualControl.add(posHLine.set("posHLine", 0.5, 0, 1));
    
    
    
    
    
    visualControl.add(colorLine.set("colorLine", ofColor(255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGTop.set("colorBGTop", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGBot.set("colorBGBot", ofColor(255,179,66,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorWaveTop.set("colorWaveTop", ofColor(0,105,255,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorWaveBot.set("colorWaveBot", ofColor(0,0,55), ofColor(0,0),ofColor(255)));
    
    gui.setup(visualControl);
    
    
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    
    
    
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
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    grafic_RGB.begin();
    
    ofBackground(0,255);
    // ofBackground(0,10);
    /* ofSetColor(0, 255);
     ofFill();
     ofRect(0, 0, RENDER_WIDTH, RENDER_HEIGHT);*/
    
    ofRectGradient(0,0,RENDER_WIDTH*2, RENDER_HEIGHT, colorBGTop, colorBGBot, OF_GRADIENT_LINEAR);
    
    
    
    wave_poles.drawGradient(0,0,colorWaveTop, colorWaveBot, posHWave);
    wave_ramp.drawGradient(RENDER_WIDTH,0,colorWaveTop, colorWaveBot, posHWave);
    
    grafic_RGB.end();
    
    grafic_W.begin();
    
    ofBackground(0,255);
    wave_poles.drawLine(0,0,colorLine, posHLine);
    wave_ramp.drawLine(RENDER_WIDTH,0,colorLine, posHLine);
    
    grafic_W.end();
    
    
    render.begin();
    grafic_RGB.draw(0,0);
    grafic_W.draw(0,RENDER_HEIGHT);
    render.end();
    
    ofSetColor(255);
    render.draw(0,0, RENDER_WIDTH, RENDER_HEIGHT);
    gui.draw();
    
    
    ofSetWindowTitle("Varde WaveArray V1 FPS: "+ofToString((int)ofGetFrameRate()));
    
    syphonRenderOut.publishTexture(&syphonTex);
    
    
    
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
    float dist = RENDER_WIDTH/(NUM_ARRAY-2);
    
    for(int i = 0; i<NUM_ARRAY; i++){
        WaveParticle wP;
        wP.p = ofVec2f(dist*i,0.);
        
        waveParticles.push_back(wP);
        
    }
}

void WaveParticleSystem::update(float _inLeft, float _inRight){
    
    waveParticles[0].p.y = _inLeft;
    
    
    //cout << ofToString(waveParticles[30].a)+ "\n";
    
    waveParticles.back().p.y = _inRight;
    
    
    
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

void WaveParticleSystem::drawLine(int _x, int _y, ofColor colorLine, float _posHLine){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    vector <ofPoint> points;
    
    
    if(true){
        ofSetLineWidth(4);
        ofSetColor(colorLine);
        ofBeginShape();
        ofNoFill();
        vector<ofPoint> points;
        float dist = RENDER_WIDTH/(NUM_ARRAY-1);
        for(int i = 0; i < NUM_ARRAY; i++){
            ofPoint point = ofVec2f(waveParticles[i].p.x, (waveParticles[i].p.y/2+1-_posHLine)*RENDER_HEIGHT);
            points.push_back(point);
        }
        
        ofCurveVertices(points);
        ofEndShape();
    }
    ofPopMatrix();
    
    
}

void WaveParticleSystem::drawGradient(int _x, int _y, ofColor colorWaveTop, ofColor colorWaveBot, float _posHWave ){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    float dist = RENDER_WIDTH/(NUM_ARRAY-1);
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofFloatColor colorWaveTop_f = colorWaveTop;
    ofFloatColor colorWaveBot_f = colorWaveBot;
    
    
    
    for(int i = 0; i < NUM_ARRAY; i++){
        
        mesh.addVertex(ofVec2f(waveParticles[i].p.x, (waveParticles[i].p.y/2+1-_posHWave)*RENDER_HEIGHT));
        mesh.addColor(colorWaveTop_f);
        
        mesh.addVertex(ofPoint(waveParticles[i].p.x, RENDER_HEIGHT));
        mesh.addColor(colorWaveBot_f);
        
    }
    for(int i = 0; i < NUM_ARRAY-1; i++){
        mesh.addIndex(i*2);
        mesh.addIndex(i*2+1);
        mesh.addIndex(i*2+2);
        
        mesh.addIndex(i*2+1);
        mesh.addIndex(i*2+3);
        mesh.addIndex(i*2+2);
        
    }
    mesh.draw();
    ofPopMatrix();
    
}

