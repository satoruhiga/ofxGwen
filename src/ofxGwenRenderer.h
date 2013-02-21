#pragma once

#include "ofMain.h"

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Texture.h"

//#define OFXGWEN_USE_OFX_FONTSTASH

namespace Gwen
{
	namespace Renderer
	{
		class ofBitmapFontRenderer;
		class ofTrueTypeFontRenderer;
		class ofxFontStashFontRenderer;
		
		template <typename FontRenderer = ofTrueTypeFontRenderer>
		class OpenFrameworks;
	}
}

struct Gwen::Renderer::ofTrueTypeFontRenderer
{
	static void LoadFont(Gwen::Renderer::Base *renderer, Gwen::Font* font)
	{
		font->realsize = font->size * renderer->Scale();
		
		ofTrueTypeFont *pFont = new ofTrueTypeFont;
		pFont->loadFont(Utility::UnicodeToString(font->facename), font->realsize, true, true, false);
		
		font->data = pFont;
	}
	
	static void FreeFont(Gwen::Renderer::Base *renderer, Gwen::Font* pFont)
	{
		if (!pFont->data) return;
		
		ofTrueTypeFont* font = ((ofTrueTypeFont*)pFont->data);
		delete font;
		
		pFont->data = NULL;
	}
	
	static void RenderText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text)
	{
		if (!pFont->data || fabs(pFont->realsize - pFont->size * renderer->Scale()) > 2)
		{
			FreeFont(renderer, pFont);
			LoadFont(renderer, pFont);
		}
		
		ofTrueTypeFont* font = (ofTrueTypeFont*)(pFont->data);
		ofRectangle rect = font->getStringBoundingBox(Utility::UnicodeToString(text), 0, 0);
		
		pos.x -= rect.x;
		pos.y -= rect.y;
		
		renderer->Translate(pos.x, pos.y);
		font->drawString(Utility::UnicodeToString(text), pos.x, pos.y);
	}
	
	static Gwen::Point MeasureText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, const Gwen::UnicodeString& text)
	{
		if (!pFont->data || fabs(pFont->realsize - pFont->size * renderer->Scale()) > 2)
		{
			FreeFont(renderer, pFont);
			LoadFont(renderer, pFont);
		}
		
		ofTrueTypeFont* font = (ofTrueTypeFont*)(pFont->data);
		
		ofRectangle rect = font->getStringBoundingBox(Utility::UnicodeToString(text), 0, 0);
		return Gwen::Point(rect.width + fabs(rect.x), rect.height);
	}
};

struct Gwen::Renderer::ofBitmapFontRenderer
{
	enum
	{
		MARGIN = 1,
		BITMAP_CHAR_WIDTH = 8,
		BITMAP_CHAR_HEIGHT = 9,
		BITMAP_CHAR_NEWLINE_HEIGHT = 5
	};

	static void LoadFont(Gwen::Renderer::Base *renderer, Gwen::Font* font)
	{
		font->size = 11;
	}
	
	static void FreeFont(Gwen::Renderer::Base *renderer, Gwen::Font* pFont) {}
	
	static void RenderText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text)
	{
		pos.y += BITMAP_CHAR_HEIGHT;

		renderer->Translate(pos.x, pos.y);
		
		// FIXME: define custom skin
		ofSetColor(0, 255);
		ofDrawBitmapString(Utility::UnicodeToString(text), pos.x, pos.y);
	}
	
	static Gwen::Point MeasureText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, const Gwen::UnicodeString& text)
	{
		int w = 0;
		int h = 1;
		int x = 0;
		int y = 0;
		int max_w = 0;

		for (int i = 0; i < text.size(); i++)
		{
			char c = text[i];
			if (c == '\n')
			{
				max_w = max(max_w, w);
				w = 0;
				h++;
			}
			else
			{
				w++;
			}
		}

		max_w = max(max_w, w);

		x = max_w * BITMAP_CHAR_WIDTH;
		y = h * BITMAP_CHAR_HEIGHT + (h - 1) * BITMAP_CHAR_NEWLINE_HEIGHT;

		x += MARGIN * 2 + 1;
		y += MARGIN * 2 + 1;

		return Gwen::Point(x, y);
	}
};

#ifdef OFXGWEN_USE_OFX_FONTSTASH

#include "ofxFontStash.h"

struct Gwen::Renderer::ofxFontStashFontRenderer
{
public:
	
	static void LoadFont(Gwen::Renderer::Base *renderer, Gwen::Font* font)
	{
		font->realsize = font->size * renderer->Scale();
		
		ofxFontStash *pFont = new ofxFontStash;
		pFont->setup(Utility::UnicodeToString(font->facename));
		
		font->data = pFont;
	}
	
	static void FreeFont(Gwen::Renderer::Base *renderer, Gwen::Font* pFont)
	{
		if (!pFont->data) return;
		
		ofxFontStash* font = ((ofxFontStash*)pFont->data);
		delete font;
		
		pFont->data = NULL;
	}
	
	static void RenderText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text)
	{
		if (!pFont->data || fabs(pFont->realsize - pFont->size * renderer->Scale()) > 2)
		{
			FreeFont(renderer, pFont);
			LoadFont(renderer, pFont);
		}
		
		string content = Utility::UnicodeToString(text);
		
		ofxFontStash* font = (ofxFontStash*)(pFont->data);
		ofRectangle rect = font->getBoundingBoxSize(content, pFont->size * renderer->Scale(), 0, 0);
		
		pos.x -= rect.x;
		pos.y -= rect.y;
		
		renderer->Translate(pos.x, pos.y);
		font->drawMultiLine(content, pFont->size * renderer->Scale(), pos.x, pos.y);
	}
	
	static Gwen::Point MeasureText(Gwen::Renderer::Base *renderer, Gwen::Font* pFont, const Gwen::UnicodeString& text)
	{
		if (!pFont->data || fabs(pFont->realsize - pFont->size * renderer->Scale()) > 2)
		{
			FreeFont(renderer, pFont);
			LoadFont(renderer, pFont);
		}
		
		ofxFontStash* font = (ofxFontStash*)(pFont->data);
		
		ofRectangle rect = font->getBoundingBoxSizeMultiLine(Utility::UnicodeToString(text), pFont->size * renderer->Scale(), 0, 0);
		return Gwen::Point(rect.width + fabs(rect.x), rect.height);
	}
};

#endif

//

template <typename FontRenderer>
class Gwen::Renderer::OpenFrameworks : public FontRenderer, public Gwen::Renderer::OpenGL
{
public:
	
	OpenFrameworks() {}
	~OpenFrameworks() {}
	
	void Init() {}
	
	void SetDrawColor(Gwen::Color color)
	{
		ofSetColor(color.r, color.g, color.b, color.a);
	}
	
	void DrawFilledRect(Gwen::Rect rect)
	{
		GLboolean texturesOn;
		
		glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
		if (texturesOn)
		{
			Flush();
			glDisable(GL_TEXTURE_2D);
		}
		
		Translate(rect);
		
		glBegin(GL_TRIANGLES);
		AddVert(rect.x, rect.y);
		AddVert(rect.x + rect.w, rect.y);
		AddVert(rect.x, rect.y + rect.h);
		
		AddVert(rect.x + rect.w, rect.y);
		AddVert(rect.x + rect.w, rect.y + rect.h);
		AddVert(rect.x, rect.y + rect.h);
		glEnd();
	}
	
	void DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
	{
		ofTexture* tex = (ofTexture*)pTexture->data;
		
		if (!tex)
		{
			return DrawMissingImage(rect);
		}
		
		Translate(rect);
		
		ofPoint c1 = tex->getCoordFromPercent(u1, v1);
		ofPoint c2 = tex->getCoordFromPercent(u2, v2);
		
		tex->bind();
		
		glBegin(GL_TRIANGLES);
		AddVert(rect.x, rect.y, c1.x, c1.y);
		AddVert(rect.x + rect.w, rect.y, c2.x, c1.y);
		AddVert(rect.x, rect.y + rect.h, c1.x, c2.y);
		
		AddVert(rect.x + rect.w, rect.y, c2.x, c1.y);
		AddVert(rect.x + rect.w, rect.y + rect.h, c2.x, c2.y);
		AddVert(rect.x, rect.y + rect.h, c1.x, c2.y);
		glEnd();
		
		tex->unbind();
	}
	
	inline void AddVert(int x, int y, float u = 0, float v = 0)
	{
		glTexCoord2f(u, v);
		glVertex2i(x, y);
	}
	
	void StartClip()
	{
		Flush();
		Gwen::Rect rect = ClipRegion();
		
		{
			GLint view[4];
			glGetIntegerv(GL_VIEWPORT, &view[0]);
			rect.y = view[3] - (rect.y + rect.h);
		}
		
		glScissor(rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale());
		glEnable(GL_SCISSOR_TEST);
	};
	
	void EndClip()
	{
		Flush();
		glDisable(GL_SCISSOR_TEST);
	};
	
	inline void Flush()
	{
		glFlush();
	}
	
	//
	
	void LoadTexture(Gwen::Texture* pTexture)
	{
		ofPixels pix;
		ofLoadImage(pix, ofToDataPath(pTexture->name.Get()));
		
		ofTexture *texture = new ofTexture;
		texture->allocate(pix.getWidth(), pix.getHeight(), GL_RGBA, false);
		texture->loadData(pix);
		
		pTexture->data = texture;
		
		pTexture->width = texture->getWidth();
		pTexture->height = texture->getHeight();
	}
	
	void FreeTexture(Gwen::Texture* pTexture)
	{
		ofTexture* tex = (ofTexture*)pTexture->data;
		if (!tex) return;
		
		delete tex;
		pTexture->data = NULL;
	}
	
	//
	
	void LoadFont(Gwen::Font* font)
	{
		FontRenderer::LoadFont(this, font);
	}

	void FreeFont(Gwen::Font* pFont)
	{
		FontRenderer::FreeFont(this, pFont);
	}

	void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text)
	{
		FontRenderer::RenderText(this, pFont, pos, text);
	}

	Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString& text)
	{
		return FontRenderer::MeasureText(this, pFont, text);
	}

};