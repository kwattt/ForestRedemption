#include "prompt.h"
#include "types.h"

prompt_::prompt_() {
}

void prompt_::enable(string name, string key) { // Enabled the prompt
	var = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_UIPROMPT_SET_CONTROL_ACTION(var, MISC::GET_HASH_KEY(key.c_str()));
	HUD::_UIPROMPT_SET_TEXT(var, MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", name.c_str()));
	HUD::_UIPROMPT_SET_GROUP(var, 0, 0);
	HUD::_UIPROMPT_SET_HOLD_MODE(var, 1);
	HUD::_UIPROMPT_REGISTER_END(var);
	HUD::_UIPROMPT_SET_ENABLED(var, 0);
	HUD::_UIPROMPT_SET_VISIBLE(var, 0);
}
void prompt_::show(bool status) // show or hide the prompt
{
	HUD::_UIPROMPT_SET_ENABLED(var, status);
	HUD::_UIPROMPT_SET_VISIBLE(var, status);
}

Prompt prompt_::p() { // return the prompt
	return var;
}