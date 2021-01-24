#include "menu.h"
#include "nativeCaller.h"
#include "natives.h"

DWORD Time = 0;

bool Key(DWORD key) {
	if (GetTickCount() < Time) return false;
	if (IsKeyDown(key)) {
		Time = GetTickCount() + 125;
		return true;
	}
	return false;
}

void draw_sprite(const char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a)
{
	float fX = x + scalex / 2;
	float fY = y + scaley / 2;
	if (!TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(sprite))
		TXD::REQUEST_STREAMED_TEXTURE_DICT(sprite, 0);
	GRAPHICS::DRAW_SPRITE(category, sprite, fX, fY, scalex, scaley, rotation, r, g, b, a, 1);
	TXD::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(category);
}

void draw_rect(float x, float y, float width, float height, int r, int g, int b, int a)
{
	float fX = x + width / 2;
	float fY = y + height / 2;
	GRAPHICS::DRAW_RECT(fX, fY, width, height, r, g, b, a, true, true);
}

void draw_text(const char* text, float x, float y, int r, int g, int b, int a, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
	HUD::_SET_TEXT_COLOR(r, g, b, a);
	HUD::SET_TEXT_SCALE(sx, sy);
	HUD::SET_TEXT_CENTRE(centered);
	const char* literalString = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text);
	HUD::_DISPLAY_TEXT(literalString, x, y);
}


Menu::Menu(string n, string s, float x, float y, float sx) : name(n), sub(s), pos_x(x), pos_y(y), size_x(sx) {
	current = 0;
}

Menu::~Menu()
{
	components.clear();
}

void Menu::Draw()
{
	// draw title

	// pos y 0.074f
	// pos x 0.053

	draw_text(name.c_str(), pos_x*2.66f, 0.086f, 255, 255, 255, 255, true, 0.5f, 0.5f);
	draw_rect(pos_x, pos_y, size_x, 0.074f, 0, 0, 0, 190);

	draw_sprite("feeds", "help_text_bg", pos_x, pos_y, size_x, 0.074f, 0, 255, 255, 255, 45);
	draw_sprite("generic_textures", "menu_header_1a", pos_x, pos_y, size_x, 0.074f, 0, 255, 255, 255, 255);
	//draw_sprite("generic_textures", "hud_menu_4a", 0.053f, 0.131f + 0.027f, 0.260f, 0.002f, 0, 255, 255, 255, 255);
	float cpos = pos_y + 0.074f;
	int number = -1;
	for (auto & it : components) {
		if (it.t() != Header) ++number;

		if (number == current && it.t() != Header) {
			draw_sprite("generic_textures", "hud_menu_5a", pos_x, cpos, size_x, 0.038f, 0, 255, 20, 20, 150);
			cName = it.n();
		}
		cpos += it.Draw(cpos, pos_x, pos_y, size_x);
	}
}

void Menu::addComponent(menuComponents t, string n)
{
	if (t != Header) ++size;
	components.insert(
		components.end(),
		MenuComponent(t, n)
	);
}

void Menu::Change(int up)
{
	if (current + up >= size) current = 0;
	else if (current + up < 0) current = size - 1;
	else current += up;
}

string Menu::Current() // Current active item
{
	return cName;
}

MenuComponent::MenuComponent(menuComponents t, string n) : type(t), name(n) //
{

}

float MenuComponent::Draw(float cpos, float x, float y, float size_x) {
	float epos = 0.0;
	switch (type) {
		case Header:
			epos = 0.033f;
			draw_rect(x, cpos, size_x, 0.033f, 0, 0, 0, 190);
			draw_text(name.c_str(), x + 0.009f, cpos + 0.005f, 255, 255, 255, 255, false, 0.3f, 0.3f);
			break;
		case Button:
			epos = 0.038f;
			draw_rect(x, cpos, size_x, 0.038f, 0, 0, 0, 190);
			draw_text(name.c_str(), x + 0.015f, cpos + 0.005f, 255, 255, 255, 255, false, 0.3f, 0.3f);
			draw_sprite("generic_textures", "hud_menu_5a", 0.053f, cpos, size_x, 0.038f, 0, 255, 255, 255, 20);
			break;
	}
	return epos;
} 

menuComponents MenuComponent::t()
{
	return type;
}

string MenuComponent::n()
{
	return name;
}

