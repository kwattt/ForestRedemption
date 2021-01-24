#pragma once
#include "files.h"

// Overlay for testing purposes 

Object *saved = nullptr;
#define psize 1024
using namespace std;

struct text_box_t
{
	std::string text;
	float x, y;
	byte r, g, b, a;
};


// This snippet is originated from the pools sample by Alexander Blade

void entity_draw_info_add(std::vector<text_box_t> &textOnScreen, Entity entity, float mindist, float maxdist, std::string type, byte r, byte g, byte b, byte a)
{
	Vector3 v = ENTITY::GET_ENTITY_COORDS(entity, TRUE, FALSE);
	float x, y;

	if (GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(v.x, v.y, v.z, &x, &y))
	{

		// draw bounds
		if (x < 0.01 || y < 0.01 || x > 0.93 || y > 0.93)
			return;
		// get coords
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE);

		// draw text if entity isn't close to the player
		if (dist > mindist && dist < maxdist)
		{
			// check if the text fits on screen
			bool bFitsOnscreen = true;
			for each (auto &iter in textOnScreen)
			{
				float textDist = sqrtf((iter.x - x)*(iter.x - x) + (iter.y - y)*(iter.y - y));
				if (textDist < 0.05)
				{
					bFitsOnscreen = false;
					break;
				}
			}
			// if text doesn't fit then skip draw
			if (!bFitsOnscreen) {
				return;
			}
			// add info to the vector
			int health = ENTITY::GET_ENTITY_HEALTH(entity);
			Hash model = ENTITY::GET_ENTITY_MODEL(entity);
			char text[256];
			//cout << "refit" << endl;
			sprintf_s(text, "\n%s %08X\nDistancia %.02f\nVida %i", type.c_str(), model, dist, health);
			if (std::find(std::begin(snowTree), std::end(snowTree), model) != snowTree.end()) {
				textOnScreen.push_back({text, x, y, 103, 48, 0, a });
			}
			else if (std::find(std::begin(woodLog), std::end(woodLog), model) != woodLog.end()) {
				textOnScreen.push_back({ text, x, y, 209, 176, 27, a });
			}
			else if (std::find(std::begin(woodMat), std::end(woodMat), model) != woodMat.end()) {
				textOnScreen.push_back({ text, x, y, 27, 140, 209, a });
			}
			else if (std::find(std::begin(woodStick), std::end(woodStick), model) != woodStick.end()) {
				textOnScreen.push_back({ text, x, y, 75, 192, 20, a });
			}
			else textOnScreen.push_back({text, x, y, r, g, b, a });

		}
	}
}

void overlay() {

	int count;
	Object pool[psize];
	count = worldGetAllObjects(pool, psize);
	float mindist = 9999.9f;
	Object *temp = nullptr;
	/*
	if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_RT")) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_RT"))) {
		for (int i = 0; i < count; i++) {
			if (ENTITY::DOES_ENTITY_EXIST(pool[i]))
			{
				if (ENTITY::IS_ENTITY_AN_OBJECT(pool[i])) {
					Vector3 v = ENTITY::GET_ENTITY_COORDS(pool[i], TRUE, FALSE);
					Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);

					float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE);
					if (dist > 2.0) {
						if (dist < mindist) {
							mindist = dist;
							temp = &pool[i];
						}
					}
				}
			}
		}
	}

	if (temp != nullptr) {
		Vector3 v = ENTITY::GET_ENTITY_COORDS(*temp, TRUE, FALSE);
		cout << "killed object with dword " << std::hex << (DWORD)ENTITY::GET_ENTITY_MODEL(*temp) << endl;
		ENTITY::SET_ENTITY_COORDS(*temp, v.x, v.y, v.z - 90.0f, false, false, false, false);
		saved = temp;
	}

	if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT")) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT"))) {

		if (saved != nullptr) {
			saveModel(ENTITY::GET_ENTITY_MODEL(*saved));
			cout << "Saved model > " << std::hex << ENTITY::GET_ENTITY_MODEL(*saved);
		}
	} */

	count = worldGetAllObjects(pool, psize);

	std::vector<text_box_t> textOnScreen;

	for (int i = 0; i < count; i++) {
		if (ENTITY::DOES_ENTITY_EXIST(pool[i]))
		{
			if (ENTITY::IS_ENTITY_AN_OBJECT(pool[i])) {
				entity_draw_info_add(textOnScreen, pool[i], 0, 10, "DWORD", 75, 75, 110, 75);
			}
		}
	}

	for each (auto &iter in textOnScreen)
	{
		HUD::SET_TEXT_SCALE(0.2f, 0.2f);
		HUD::_SET_TEXT_COLOR(255, 255, 255, 255);
		HUD::SET_TEXT_CENTRE(0);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::_DISPLAY_TEXT(MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", const_cast<char *>(iter.text.c_str())), iter.x, iter.y);
		GRAPHICS::DRAW_RECT(iter.x + 0.028f, iter.y + 0.033f, 0.058f, 0.041f, iter.r, iter.g, iter.b, iter.a, 0, 0);
	}

}