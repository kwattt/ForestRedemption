#include "script.h"
#include "models.h"
#include "overlay.h"
#include "misc.h"
#include "menu.h"

#define max_wood 4

prompt_ TreeP;
prompt_ LogP;
prompt_ WoodP;
prompt_ DWoodP;
prompt_ StickP;

Object cabain_test = NULL;

Ped playerPed;

using namespace std;

void checkKeys() { // Main loop, we mostly check keys for actions
	size_t tWood = playerInv.wood.size();
	playerPed = PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX());

	if (tWood > 0) { // Player is carrying wood. We add the drop wood prompt.
		HUD::_UIPROMPT_DISABLE_PROMPT_TYPE_THIS_FRAME(7);
		DWoodP.show(true);
		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(DWoodP.p())) {

			DWoodP.show(false);

			ENTITY::DETACH_ENTITY(*playerInv.wood.begin(), true, true);
			playerInv.wood.erase(playerInv.wood.begin());

			WAIT(10); 

			size_t tWood = playerInv.wood.size();
			if(tWood == 0) // If the player threw all the wood remove the carry anim 
				stopAnim("mech_loco_m@generic@carry@animals@medium@idle_shoulder", "idle");
		}
	}
	else
		DWoodP.show(false);

	if (PED::IS_PED_FALLING(playerPed) || PED::IS_PED_SWIMMING(playerPed)) { // Player is falling

		for (auto & it : playerInv.wood) { // player is carrying wood
			ENTITY::DETACH_ENTITY(it, true, true);
			WAIT(100);
		}

		playerInv.wood.clear();
		stopAnim("mech_loco_m@generic@carry@animals@medium@idle_shoulder", "idle");
		return; 
	}

	Hash weap;
	WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weap, false, 0, false); // player has to be unarmed

	Object wood = isPlayerNearObject(woodMat, 2.2f);
	if (wood != NULL && weap == joaat("WEAPON_UNARMED")) { // Pick up planks.
		if (tWood > max_wood) return; // solo 4!

		WoodP.show(true);
		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(WoodP.p()))
		{
			float basez = -0.06f;
			basez += 0.03f * tWood;

			ENTITY::ATTACH_ENTITY_TO_ENTITY(wood, playerPed, 266, 0.10f, 0.11f, basez, 10.0f, 26.0f, 0.0f, false, false, false, false, 2, true, false, false);
			playAnim("mech_loco_m@generic@carry@animals@medium@idle_shoulder", "idle", true);
			playerInv.wood.insert(playerInv.wood.begin(), wood);
			WoodP.show(false);
			WAIT(30);
		}
	}
	else
		WoodP.show(false);

	Object stick = isPlayerNearObject(woodStick, 2.1f);
	if (stick != NULL && tWood < max_wood) { // Player has wood planks near

		StickP.show(true);

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(StickP.p()))
		{

			DWORD TOBJ = 0x02EFB3FA;
			if (!STREAMING::IS_MODEL_VALID(TOBJ))
				cout << "INVALID MODEL" << endl;
			else {
				STREAMING::REQUEST_MODEL(TOBJ, 0);
				while (!STREAMING::HAS_MODEL_LOADED(TOBJ))
				{
					WAIT(0);
				}
			}

			StickP.show(false);

			HUD::DISPLAY_HUD(false);
			HUD::_SHOW_PLAYER_CORES(false);

			CAM::DO_SCREEN_FADE_OUT(300);
			WAIT(500);

			Object tmp;
			Vector3 a = ENTITY::GET_ENTITY_COORDS(playerPed, 1, 1);

			ENTITY::SET_ENTITY_COORDS(stick, a.x, a.y, a.z - 90.0f, false, false, false, false);
			OBJECT::DELETE_OBJECT(&stick);

			int negative = rand() % 2;
			int negative2 = rand() % 2;

			float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5));
			float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5));

			if (negative) r1 *= -1.0f;
			if (negative) r2 *= -1.0f;

			tmp = OBJECT::CREATE_OBJECT(TOBJ, a.x + r1, a.y + r2, a.z + 0.04f, 0, 1, 0, 0, 0);
			ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(tmp, 1);
			PHYSICS::ACTIVATE_PHYSICS(tmp);

			SCRIPTS::SET_NO_LOADING_SCREEN(true);

			HUD::DISPLAY_HUD(true);
			HUD::_SHOW_PLAYER_CORES(true);

			WAIT(100);
			CAM::DO_SCREEN_FADE_IN(200);
		}
	}
	else
		StickP.show(false);

	if (tWood > 0) return; // If player is carrying wood.

	Object log = isPlayerNearObject(woodLog, 2.2f); // Chop tree
		if (log != NULL) {

			LogP.show(true);

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(LogP.p()))
			{

				// PERFECT 622277464
				// SAVED 				(Local_1157.f_690[(16 + iVar7) /*12*/])->f_7 = 622277464;

				DWORD TOBJ = 0x02EFB3FA;
				if (!STREAMING::IS_MODEL_VALID(TOBJ))
					cout << "INVALID MODEL" << endl;
				else {
					STREAMING::REQUEST_MODEL(TOBJ, 0);
					while (!STREAMING::HAS_MODEL_LOADED(TOBJ))
					{
						WAIT(0);
					}
				}

				LogP.show(false);

				HUD::DISPLAY_HUD(false);
				HUD::_SHOW_PLAYER_CORES(false);

				CAM::DO_SCREEN_FADE_OUT(1500);
				WAIT(1700);

				Object tmp;
				Vector3 a = ENTITY::GET_ENTITY_COORDS(playerPed, 1, 1);

				ENTITY::SET_ENTITY_COORDS(log, a.x, a.y, a.z - 90.0f, false, false, false, false);
				OBJECT::DELETE_OBJECT(&log);

				int negative = rand() % 2;
				int negative2 = rand() % 2;

				for (int i = 0; i < 3; i++) {
					HUD::DISPLAY_HUD(false);
					HUD::_SHOW_PLAYER_CORES(false);
					float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5));
					float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5));

					if (negative) r1 *= -1.0f;
					if (negative) r2 *= -1.0f;

					tmp = OBJECT::CREATE_OBJECT(TOBJ, a.x + r1, a.y + r2, a.z + 0.25f, 0, 1, 0, 0, 0);
					ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(tmp, 1);
					PHYSICS::ACTIVATE_PHYSICS(tmp);
				}

				SCRIPTS::SET_NO_LOADING_SCREEN(true);

				HUD::DISPLAY_HUD(true);
				HUD::_SHOW_PLAYER_CORES(true);

				WAIT(600);
				CAM::DO_SCREEN_FADE_IN(300);
			}
		}
		else
			LogP.show(false);


		Object tree = isPlayerNearObject(snowTree, 2.2f); // Chop snow tree.
		if (tree != NULL) {
			TreeP.show(true);

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(TreeP.p()))
			{

				DWORD TOBJ = 622277464;
				DWORD TOBASE = 0x5BD08710;
				if (STREAMING::IS_MODEL_VALID(TOBJ) && STREAMING::IS_MODEL_VALID(TOBASE))
				{
					STREAMING::REQUEST_MODEL(TOBJ, 0);
					STREAMING::REQUEST_MODEL(TOBASE, 0);
					while (!STREAMING::HAS_MODEL_LOADED(TOBJ) || !STREAMING::HAS_MODEL_LOADED(TOBASE))
					{
						WAIT(0);
					}
				}
				TreeP.show(false);

				Vector3 s = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 13.0f, -4.0f, 4.7f);
				Vector3 e = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 13.0f, 4.0f, 4.4f);
				Vector3 a = ENTITY::GET_ENTITY_COORDS(playerPed, 1, 1);

				Cam Start = CAM::CREATE_CAMERA_WITH_PARAMS(joaat("DEFAULT_SCRIPTED_CAMERA"), s.x, s.y, s.z, 0.0, 0.0, 0.0, CAM::GET_GAMEPLAY_CAM_FOV(), 1, 0);
				Cam End = CAM::CREATE_CAMERA_WITH_PARAMS(joaat("DEFAULT_SCRIPTED_CAMERA"), e.x, e.y, e.z, 0.0, 0.0, 0.0, CAM::GET_GAMEPLAY_CAM_FOV(), 1, 0);
				CAM::POINT_CAM_AT_COORD(Start, a.x, a.y, a.z);
				CAM::POINT_CAM_AT_COORD(End, a.x, a.y, a.z);
				CAM::SET_CAM_ACTIVE_WITH_INTERP(Start, End, 7200, 1, 1);

				if (CAM::IS_GAMEPLAY_HINT_ACTIVE()) CAM::STOP_GAMEPLAY_HINT(false);
				CAM::SET_CINEMATIC_BUTTON_ACTIVE(false);

				CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 1, 0, 1);
				CAM::SET_CAM_AFFECTS_AIMING(Start, false);
				CAM::SET_CAM_AFFECTS_AIMING(End, false);

				for (int i = 0; i < 4; i++) {
					if (i == 3) {}
					else
						playAnim("amb_work@world_human_tree_chop@male_a@wip_base", "wip_base");
					HUD::DISPLAY_HUD(false);
					HUD::_SHOW_PLAYER_CORES(false);
					WAIT(1700);
					AUDIO::_PLAY_SOUND_FROM_POSITION("Wood_Tear", a.x, a.y, a.z, "APPLESEED_SOUNDSET", 0, 0, 1, 0);
					if (i == 2) CAM::DO_SCREEN_FADE_OUT(1500);  
				}
				Object tmp;
				Vector3 log = ENTITY::GET_ENTITY_COORDS(tree, 1, 1);
				ENTITY::SET_ENTITY_COORDS(tree, a.x, a.y, a.z - 90.0f, false, false, false, false);

				tmp = OBJECT::CREATE_OBJECT(TOBASE, log.x, log.y, log.z, 0, 1, 0, 0, 0);

				int negative = rand() % 2;
				int negative2 = rand() % 2;

				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.6));
				float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.6));

				if (negative) r1 *= -1.0f;
				if (negative) r2 *= -1.0f;

				tmp = OBJECT::CREATE_OBJECT(TOBJ, log.x + r1, log.y + r2, log.z + 3.4f, 0, 1, 0, 0, 0);
				
				ENTITY::SET_ENTITY_ROTATION(tmp, 90.0f, 0.0, 0.0, 1, 0);
				ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(tmp, 1);
				PHYSICS::ACTIVATE_PHYSICS(tmp);

				SCRIPTS::SET_NO_LOADING_SCREEN(true);

				HUD::DISPLAY_HUD(true);
				HUD::_SHOW_PLAYER_CORES(true);
				CAM::SET_CINEMATIC_BUTTON_ACTIVE(true);
				CAM::SET_CINEMATIC_MODE_ACTIVE(false);

				CAM::RENDER_SCRIPT_CAMS(false, 1, 50, 1, 0, 0);
				CAM::DESTROY_CAM(Start, 1);
				CAM::DESTROY_CAM(End, 1);

				WAIT(600);
				CAM::DO_SCREEN_FADE_IN(300);
			}
		}
		else
			TreeP.show(false);

	if (cabain_test != NULL) { // Move cabain position, testing.
		PLAYER::SET_PLAYER_CONTROL(0, false, 0, 0);
			
		Vector3 a = ENTITY::GET_ENTITY_COORDS(cabain_test, true, true);

		float x = 0.0f;
		float y = 0.0f;

		if (Key(VK_LEFT)) {
			y += 0.6f;
		}
		else if (Key(VK_RIGHT)) {
			y -= 0.6f;
		}
		else if (Key(VK_UP)) {
			x += 0.6f;
		}
		else if (Key(VK_DOWN)) {
			x -= 0.6f;
		}

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(cabain_test, a.x + x, a.y + y, a.z, 0, 0, 0);
		OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(cabain_test, true);

		if (Key(VK_BACK)) {
			cabain_test = NULL;
			PLAYER::SET_PLAYER_CONTROL(0, true, 0, 0);
			CAM::RENDER_SCRIPT_CAMS(false, 1, 50, 1, 0, 0);
		}

	}
}

Cam cCam;
Menu *cMenu;
Menu test("Building", "-", 0.053f, 0.078f, 0.2f);

void menuKeys() { // open menu
	if (cMenu == nullptr) {
		if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT")) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT"))) 
			cMenu = &test;
	}

	if (cMenu != nullptr) {
		if (Key(VK_BACK)) { // Close menu
			cMenu = nullptr;
		}

		if (Key(VK_NUMPAD2) || Key(VK_DOWN)) { // menu down
			test.Change(1);
		}
		else if (Key(VK_NUMPAD8) || Key(VK_UP)) { // menu up
			test.Change(-1);
		}

		if (cMenu == &test) // cheat menú.
		{
			if (Key(VK_RETURN)) {
				if (test.Current() == "Settled") { // Create construction, test with a cabain.

					cMenu = nullptr;
					
					Hash OBJID = 147427595;

					if (!STREAMING::IS_MODEL_VALID(OBJID))
						cout << "INVALID MODEL" << endl;
					else {
						STREAMING::REQUEST_MODEL(OBJID, 0);
						while (!STREAMING::HAS_MODEL_LOADED(OBJID))
						{
							WAIT(0);
						}
					}

					Vector3 a = ENTITY::GET_ENTITY_COORDS(playerPed, true, true);

					cout << "Done" << endl;

					cabain_test = OBJECT::CREATE_OBJECT(OBJID, a.x, a.y + 2.0f, a.z + 0.04f, 0, 1, 0, 0, 0);
					// ENTITY::SET_ENTITY_COLLISION(cabain_test, false, true);
					
					float startHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
					Vector3 camOffset;
					camOffset.x = (float)sin((startHeading * 3.1416f / 180.0f))*3.0f;
					camOffset.y = (float)cos((startHeading * 3.1416f / 180.0f))*3.0f;

					camOffset.z = 0.4f;

					cCam = CAM::GET_RENDERING_CAM();
					Vector3 camLocation = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(playerPed, camOffset.x, camOffset.y, camOffset.z);

					Cam cameraHandle = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", camLocation.x, camLocation.y, camLocation.z, 0.0, 0.0, 0.0, 40.0, 1, 2);
					CAM::ATTACH_CAM_TO_ENTITY(cameraHandle, cabain_test, camOffset.x, camOffset.y, camOffset.z+14.0f, true);
					CAM::POINT_CAM_AT_ENTITY(cameraHandle, cabain_test, 0.0f, 0.0f, 0.0f, true);
					CAM::RENDER_SCRIPT_CAMS(true, 0, 50, 1, 0, 0);
				}


				if (test.Current() == "Cores") { // Restore player cores cheat.
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), 0, 99);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), 1, 99);
					ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::GET_PLAYER_PED(PLAYER::GET_PLAYER_INDEX()), 2, 99);
				}
				if (test.Current() == "Exit") { // Close menu
					cMenu = nullptr;
				}
			}

			test.Draw();
		}

	}
}

void main()
{
	test.addComponent(Header, "Construction");
	test.addComponent(Button, "Settled");
	test.addComponent(Button, "Free");
	test.addComponent(Button, "Cores");
	test.addComponent(Button, "Exit");


	playerInv.log = false;

	TreeP.enable("Chop Tree", "INPUT_RELOAD");
	LogP.enable("Cut Log", "INPUT_ENTER");
	WoodP.enable("Pick Plank", "INPUT_ATTACK");
	DWoodP.enable("Drop Plank", "INPUT_AIM");
	StickP.enable("Collect Wood", "INPUT_RELOAD");

	while (true)
	{
		WAIT(0);
		//menuKeys();
		//checkKeys();
		//overlay();
	}
}

#pragma warning(disable : 4996)

void ScriptMain()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	srand(GetTickCount());
	main();
}