#include "testApp.h"

#include "Gwen/UnitTest/UnitTest.h"

#include "ofxGwen.h"

ofxGwen gwen;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	ofBackground(255);
	
	gwen.setup();
	
	UnitTest* unit = new UnitTest(gwen.getCanvas());
	unit->SetPos(0, 0);
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}