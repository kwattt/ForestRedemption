#pragma once

#include <iostream>
#include <vector>
#include "keyboard.h"

using namespace std;

bool Key(DWORD key);

enum menuComponents { // menu components
	Button,
	Slider,
	Header
};

class Menu;
class MenuComponent;

// selector de componentes
// actualizar valores de usuario.
// https://github.com/femga/rdr3_discoveries/blob/b56a3f1200e7761bfa187ac97764929acb94c068/useful_info_from_rpfs/textures/menu_textures/README.md

class Menu {
	public:
		Menu(string n, string s, float x, float y, float sx);
		~Menu();
		void Draw();
		void addComponent(menuComponents t, string n);
		void Change(int up);
		string Current();
	private:
		float pos_x;
		float pos_y;
		float size_x;
		string cName;
		string sub;
		string name;
		vector <MenuComponent> components;
		int current;
		int size;
};

class MenuComponent {
	public:
		MenuComponent(menuComponents t, string n);
		float Draw(float cpos, float x, float y, float size_x);
		menuComponents t();
		string n();
	protected:
		menuComponents type;
		string name;
		float var;
};