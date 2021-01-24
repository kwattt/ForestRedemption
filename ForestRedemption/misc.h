#pragma once
#include <iostream>
#include "main.h"
#include "nativeCaller.h"
#include "natives.h"
#include <vector>

#define psize 1024

Hash joaat(const char* string)
{
	return MISC::GET_HASH_KEY(string);
}

struct Carry {
	vector<Object> wood;
	bool log;
	//int box;
};

Carry playerInv;

void stopAnim(const char* dict, const char*  name) { // stop the anim, we need the anim dic and name so we dont stop the other anims.
	TASK::STOP_ANIM_TASK(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), dict, name, 0);
}

void playAnim(const char* dict, const char*  name, bool loop = false) { // play anim
	STREAMING::REQUEST_ANIM_DICT(dict);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
		WAIT(0);
	if (!loop)
		TASK::TASK_PLAY_ANIM(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), dict, name, 1.0, 1.0, 30000, 0, 0, 1, 0, 0, 0, 0);
	else
		TASK::TASK_PLAY_ANIM(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), dict, name, 8.0, -8.0, -1, 31, 0, false, false, false, 0, 0);
}

Object isPlayerNearObject(vector<DWORD> type, float distance) { // near object in a vector of dwords.
	int count;
	Object pool[psize];
	count = worldGetAllObjects(pool, psize);
	float mindist = 999.9f;
	int ent = -1;
	for (int i = 0; i < count; i++) {
		if (ENTITY::DOES_ENTITY_EXIST(pool[i]))
		{
			if (ENTITY::IS_ENTITY_AN_OBJECT(pool[i])) {
				Hash model = ENTITY::GET_ENTITY_MODEL(pool[i]);

				if (ENTITY::IS_ENTITY_ATTACHED(pool[i])) {
					if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT")) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT"))) {
						ENTITY::DETACH_ENTITY(pool[i], true, false);
					}
				}

				if (std::find(type.begin(), type.end(), model) != type.end()) {
					Vector3 v = ENTITY::GET_ENTITY_COORDS(pool[i], TRUE, FALSE);
					Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
					float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE);
					if (dist < distance && dist < mindist && !ENTITY::IS_ENTITY_ATTACHED(pool[i]) && ENTITY::GET_ENTITY_HEALTH(pool[i]) >= 800)
					{
						mindist = dist;
						ent = i;
					}
				}
			}
		}
	}
	if (ent != -1) return pool[ent];
	return NULL;
}

Vector3 getNewMouse() { // -- unused
	float mx = 0.0f;
	float my = 0.0f;
	
}
