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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    
    
    
    
    // Blinkende Lygter
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
    
    
    WaveControl.add(updateRequest.set("updateRequest",0, 0, 1));
    
    gui.setup(WaveControl);
    
    
    // SETUP FOR OSC
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    
   // colorBlink = ofColor(255);
   // colorBoubbles = ofColor(255);
    
    updateRequest = 1;
    syncOSC.update();
    updateRequest = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    syncOSC.update();
    
    
    // Blinkedne Lygter
    for(int i = 0; i < 10 ; i++){
        if(ofRandom(100000)/100000 < blinkIntensity){
            Lygte lygte;
            lygte.lygteColor = colorBlink;
            lygte.location = ofVec2f(((int)ofRandom(RENDER_WIDTH_POLE+RENDER_WIDTH_RAMP)),(int)ofRandom(RENDER_HEIGHT_POLE));
            lygte.tempo = blinkTempo;
            lygte.hard_soft = hard_soft;
            
            lygter.push_back(lygte);
        }
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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < boubbles.size(); i++){
        boubbles[i].draw();
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
    
    
    
    
    fboTexPoles.begin();
    ofRectGradient(0, fboTexPoles.getHeight()/2, fboTexPoles.getWidth(), fboTexPoles.getHeight()/2, colorBotTop, colorBotBot, OF_GRADIENT_LINEAR);
    for(int i = 0; i < dancingLinesPole.size(); i++){
        dancingLinesPole[i].draw();
    }
    ofRectGradient(0,0, fboTexPoles.getWidth(), fboTexPoles.getHeight()/2, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    fboTexPoles.end();
    
    
    fboTexRamp.begin();
    ofRectGradient(0, fboTexRamp.getHeight()/2, fboTexRamp.getWidth(), fboTexRamp.getHeight()/2, colorBotTop, colorBotBot, OF_GRADIENT_LINEAR);
    for(int i = 0; i < dancingLinesRamp.size(); i++){
        dancingLinesRamp[i].draw();
    }
    ofRectGradient(0,0, fboTexRamp.getWidth(), fboTexRamp.getHeight()/2, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    fboTexRamp.end();
    
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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ofSetColor(255,255);
    
    wave_poles.drawGradient(0,0,                RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, posHWavePoles, fboTexPoles.getTexture());
    wave_ramp.drawGradient( RENDER_WIDTH_POLE,0,RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP, posHWaveRamp, fboTexRamp.getTexture());
   
    // Line
    wave_poles.drawLine(0,0,                RENDER_WIDTH_POLE, RENDER_HEIGHT_POLE, colorLine, posHLinePoles, LINE_WIDTH);
    wave_ramp.drawLine( RENDER_WIDTH_POLE,0,RENDER_WIDTH_RAMP, RENDER_HEIGHT_RAMP, colorLine, posHLineRamp, LINE_WIDTH);
    
    graficBlinkendeLygter.draw(0,0);//RENDER_HEIGHT_POLE);
    graficBoubbles.draw(0,0);//RENDER_HEIGHT_POLE);
    
    //ofRectGradient(,RENDER_HEIGHT_RAMP, RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE-RENDER_HEIGHT_RAMP, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    
    ofRectGradient(RENDER_WIDTH_POLE,RENDER_HEIGHT_RAMP, RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE-RENDER_HEIGHT_RAMP, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    
    render.end();
    
    
    
    
    
    syphonRenderOut.publishTexture(&syphonTex);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(150);
    
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
void WaveParticleSystem::setup(int _numArray){
    
    for(int i = 0; i<_numArray; i++){
        WaveParticle wP;
        wP.p = 0.;
        
        waveParticles.push_back(wP);
        
    }
}

void WaveParticleSystem::update(float _inLeft, float _inRight){
    
    waveParticles[0].p = _inLeft;
    
    
    //cout << ofToString(waveParticles[30].a)+ "\n";
    
    waveParticles.back().p = _inRight;
    
    
    
    for(int i = 1; i<waveParticles.size()-1; i++){
        
        waveParticles[i].update(waveParticles[i-1].p, waveParticles[i+1].p);
        int i2 = waveParticles.size()-1-i;
        waveParticles[i2].update(waveParticles[i2-1].p, waveParticles[i2+1].p);
        
    }
    
}

void WaveParticleSystem::updateResponse(float _attack, float _damping){
    
    for(int i = 0; i<waveParticles.size(); i++){
        waveParticles[i].damping = _damping;
        waveParticles[i].attack  = _attack;
        
    }
}

void WaveParticleSystem::drawLine(int _x, int _y, int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    ofSetLineWidth(_lineWidth);
    ofSetColor(colorLine);
    
    float inc = waveParticles.size()/_w;
    
    ofPolyline line;
    
    
    
    for(int i = 0; i <= _w; i++){
        
        float x = i;
        float y = (waveParticles[i*inc].p/2+1-_posHLine)*_h;
        line.addVertex(ofVec2f(x,y));
    }
    line.draw();
    
    ofPopMatrix();
    
    
}

void WaveParticleSystem::drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    
    float inc = waveParticles.size()/_w;
    
    float incTex = _texture.getWidth() / _w;
    float texHeight = _texture.getHeight();
    float texHalfHeight = texHeight/2;
    
    ofMesh mesh;
    mesh.clear();
    mesh.enableTextures();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    float min = 0;
    float max = 0;
    
    for(int i = 0; i <= _w; i++){
        float pValue = waveParticles[i*inc].p*-1;
        if(pValue < min) min = pValue;
        if(pValue > max) max = pValue;
        
    }

    
    
    for(int i = 0; i <= _w; i++){
        
        float x = i;
        float y = (waveParticles[i*inc].p/2+1-_posHWave)*_h;
        ofFloatColor pColor = ofFloatColor(1., ofMap( waveParticles[i*inc].p*-1, min, max, 0.8, 1, true));
        
        
        mesh.addVertex(ofVec3f(x, 0, 0));
        mesh.addTexCoord(ofVec2f(x,0));
        mesh.addColor(pColor);
        
        mesh.addVertex(ofVec3f(x, y, 0));
        mesh.addTexCoord(ofVec2f(x,texHalfHeight));
        mesh.addColor(pColor);

        
        mesh.addVertex(ofVec3f(x, _h, 0));
        mesh.addTexCoord(ofVec2f(x,texHeight));
        mesh.addColor(pColor);



    }
    
    for(int i = 0; i < mesh.getNumVertices()/3-1; i++){
        
        
        mesh.addIndex(i*3+1);
        mesh.addIndex(i*3+3);
        mesh.addIndex(i*3+0);
        
        mesh.addIndex(i*3+1);
        mesh.addIndex(i*3+4);
        mesh.addIndex(i*3+3);
        
        mesh.addIndex(i*3+2);
        mesh.addIndex(i*3+4);
        mesh.addIndex(i*3+1);
        
        mesh.addIndex(i*3+2);
        mesh.addIndex(i*3+5);
        mesh.addIndex(i*3+4);
        
    }
    
    ofSetColor( 255, 255, 255 );  //Set white color
    _texture.bind();
    mesh.draw();
    _texture.unbind();
    
    ofPopMatrix();
}

