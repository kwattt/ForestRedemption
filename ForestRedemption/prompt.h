#pragma once
#include <iostream>
#include "types.h"
#include "natives.h"

using namespace std;

class prompt_ { // Prompt class, basic stuff.
public:
	prompt_();
	void enable(string name, string key);
	void show(bool status);
	Prompt p();
private:
	Prompt var;
};