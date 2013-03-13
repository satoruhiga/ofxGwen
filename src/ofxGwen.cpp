#include "ofxGwen.h"

#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

ofxGwen::ofxGwen() : event_enable(false), visible(true), renderer(NULL), skin(NULL), canvas(NULL)
{
	
}

ofxGwen::~ofxGwen()
{
	
}

void ofxGwen::setup(const string& default_font, const string& skin_texture_path)
{
	show();
	enableAllEvents();
	
	if (default_font == "")
		renderer = createRenderer(false);
	else
		renderer = createRenderer(true);
		
	skin = createSkin(renderer, default_font, skin_texture_path);
	canvas = createCanvas();
}

//

Gwen::Renderer::OpenGL* ofxGwen::createRenderer(bool use_truetype_font)
{
	Gwen::Renderer::OpenGL* renderer = NULL;
	
	if (use_truetype_font)
	{
#ifdef OFXGWEN_USE_OFX_FONTSTASH
		renderer = new Gwen::Renderer::OpenFrameworks<Gwen::Renderer::ofxFontStashFontRenderer>;
#else
		renderer = new Gwen::Renderer::OpenFrameworks<Gwen::Renderer::ofTrueTypeFontRenderer>;
#endif
	}
	else
	{
		renderer = new Gwen::Renderer::OpenFrameworks<Gwen::Renderer::ofBitmapFontRenderer>;
	}

	renderer->Init();
	return renderer;
}

Gwen::Skin::Base* ofxGwen::createSkin(Gwen::Renderer::OpenGL *renderer, const string& default_font, const string& skin_texture_path)
{
	Gwen::Skin::Base *skin = NULL;
	
	if (skin_texture_path.empty())
	{
		skin = new Gwen::Skin::Simple(renderer);
	}
	else
	{
		Gwen::Skin::TexturedBase *textured = new Gwen::Skin::TexturedBase(renderer);
		textured->Init(skin_texture_path);
		textured->SetDefaultFont(Gwen::Utility::StringToUnicode(default_font), 11);
		
		skin = textured;
	}
	
	skin->SetRender(renderer);
	return skin;
}

Gwen::Controls::Canvas* ofxGwen::createCanvas()
{
	Gwen::Controls::Canvas *canvas = new Gwen::Controls::Canvas(skin);
	canvas->SetSize(ofGetWidth(), ofGetHeight());
	return canvas;
}

//

void ofxGwen::enableAllEvents()
{
	if (event_enable) return;
	event_enable = true;
	
	ofAddListener(ofEvents().update, this, &ofxGwen::update);
	ofAddListener(ofEvents().draw, this, &ofxGwen::draw);
	ofAddListener(ofEvents().mousePressed, this, &ofxGwen::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxGwen::mouseReleased);
	ofAddListener(ofEvents().mouseDragged, this, &ofxGwen::mouseDragged);
	ofAddListener(ofEvents().mouseMoved, this, &ofxGwen::mouseMoved);
	ofAddListener(ofEvents().keyPressed, this, &ofxGwen::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxGwen::keyReleased);
	ofAddListener(ofEvents().windowResized, this, &ofxGwen::windowResized);
}

void ofxGwen::disableAllEvents()
{
	if (!event_enable) return;
	event_enable = false;
	
	ofRemoveListener(ofEvents().update, this, &ofxGwen::update);
	ofRemoveListener(ofEvents().draw, this, &ofxGwen::draw);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxGwen::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxGwen::mouseReleased);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxGwen::mouseDragged);
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxGwen::mouseMoved);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxGwen::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxGwen::keyReleased);
	ofRemoveListener(ofEvents().windowResized, this, &ofxGwen::windowResized);
}

void ofxGwen::show()
{
	visible = true;
	enableAllEvents();
}

void ofxGwen::hide()
{
	visible = false;
	disableAllEvents();
}

// events

void ofxGwen::update(ofEventArgs& e)
{
	assert(canvas);
	canvas->DoThink();
}

void ofxGwen::draw(ofEventArgs& e)
{
	assert(canvas);
	
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	canvas->RenderCanvas();
	glPopAttrib();
	glPopMatrix();
}

void ofxGwen::mousePressed(ofMouseEventArgs& e)
{
	assert(canvas);
	canvas->InputMouseButton(e.button, true);
	
	last_mouse_event = e;
}

void ofxGwen::mouseReleased(ofMouseEventArgs& e)
{
	assert(canvas);
	canvas->InputMouseButton(e.button, false);
	
	last_mouse_event = e;
}

void ofxGwen::mouseDragged(ofMouseEventArgs& e)
{
	assert(canvas);
	canvas->InputMouseMoved(e.x, e.y, last_mouse_event.x - e.x, last_mouse_event.y - e.y);
	
	last_mouse_event = e;
}

void ofxGwen::mouseMoved(ofMouseEventArgs& e)
{
	assert(canvas);
	canvas->InputMouseMoved(e.x, e.y, last_mouse_event.x - e.x, last_mouse_event.y - e.y);
	
	last_mouse_event = e;
}

void ofxGwen::keyPressed(ofKeyEventArgs& e)
{
	assert(canvas);
	
	unsigned char k = 0;
	
#define KEY_MAP(ofKey, gwenKey) case ofKey: { k = gwenKey; break; }
	
	switch (e.key) {
			KEY_MAP(OF_KEY_LEFT, Gwen::Key::Left);
			KEY_MAP(OF_KEY_RIGHT, Gwen::Key::Right);
			KEY_MAP(OF_KEY_UP, Gwen::Key::Up);
			KEY_MAP(OF_KEY_DOWN, Gwen::Key::Down);
			
			KEY_MAP(OF_KEY_RETURN, Gwen::Key::Return);
			KEY_MAP(OF_KEY_BACKSPACE, Gwen::Key::Backspace);
			KEY_MAP(OF_KEY_DEL, Gwen::Key::Delete);
			
			KEY_MAP('\t', Gwen::Key::Tab);
			KEY_MAP(' ', Gwen::Key::Space);
			
			KEY_MAP(OF_KEY_HOME, Gwen::Key::Home);
			KEY_MAP(OF_KEY_END, Gwen::Key::End);
			
//			// FIXME: Currently doesn't work
			KEY_MAP(OF_KEY_CTRL, 0); // Gwen::Key::Control
			KEY_MAP(OF_KEY_ALT, 0); // Gwen::Key::Alt
		default:
			break;
	}
	
#undef KEY_MAP
	
	if (k == 0 || e.key == ' ')
	{
		canvas->InputCharacter(e.key);
	}
	
	if (k != 0)
	{
		canvas->InputKey(k, true);
		canvas->InputKey(k, false);
	}
}

void ofxGwen::keyReleased(ofKeyEventArgs& e)
{
}

void ofxGwen::windowResized(ofResizeEventArgs &e)
{
	assert(canvas);
	canvas->SetSize(e.width, e.height);
}
