#include "script.h"
#include "keyboard.cpp"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) // inject in the DLL
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	}
	return TRUE;
}