#pragma once

#include "ofMain.h"

#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"
#include "Gwen/Controls/Properties.h"

#include "ofxGwenRenderer.h"

class ofxGwen
{
public:
	
	ofxGwen();
	virtual ~ofxGwen();
	
	void setup();
	
	Gwen::Controls::Canvas* getCanvas() const { return canvas; }

	// events

	void enableAllEvents();
	void disableAllEvents();
	bool isEventEnabled();

	void update(ofEventArgs& e);
	void draw(ofEventArgs& e);
	void mousePressed(ofMouseEventArgs& e);
	void mouseReleased(ofMouseEventArgs& e);
	void mouseDragged(ofMouseEventArgs& e);
	void mouseMoved(ofMouseEventArgs& e);
	void keyPressed(ofKeyEventArgs& e);
	void keyReleased(ofKeyEventArgs& e);
	void windowResized(ofResizeEventArgs &e);

	// visible
	
	void show();
	void hide();
	void toggleView() { if (isVisible()) hide(); else show(); }
	
	inline bool isVisible() { return visible; }

protected:

	bool visible;
	bool event_enable;
	
	Gwen::Renderer::OpenGL *renderer;
	Gwen::Skin::Base *skin;
	Gwen::Controls::Canvas* canvas;
	
	virtual Gwen::Renderer::OpenGL *createRenderer();
	virtual Gwen::Skin::Base *createSkin(Gwen::Renderer::OpenGL *renderer);
	virtual Gwen::Controls::Canvas *createCanvas();
	
private:
	
	ofMouseEventArgs last_mouse_event;
};
