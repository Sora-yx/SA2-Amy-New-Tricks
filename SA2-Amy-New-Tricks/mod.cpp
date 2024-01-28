#include "pch.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "WARNING Amy New Tricks is deprecated! Please download and install 'SA2 New Tricks Mod' for the best experience.\n\nIf you already have it installed, please uninstall Amy New Tricks.";

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;
		PrintDebug(error);
		MessageBoxA(MainWindowHandle, error, "Amy New Tricks", MB_ICONWARNING);
		return;
	}

	__declspec(dllexport) void __cdecl OnFrame() 
	{

	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}