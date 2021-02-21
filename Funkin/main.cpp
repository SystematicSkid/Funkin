#include <windows.h>
#include <iostream>
#include <vector>

// memory
#include "./memory.h"

// sdk
#include "./Game/game.h"
#include "./Game/note.h"
#include "./Game/playstate.h"
#include "./Game/playersettings.h"

// hooks
#include "hooks.h"

void init()
{
	FILE* pNewStdout = NULL;
	FILE* pNewStderr = NULL;
	FILE* pNewStdin = NULL;

	/* Initialize our output console */
	/* TODO: Move this into a debug macro */
	//AllocConsole();
	///* Assign `stdin`, `stout`, `stderr`  */
	//::freopen_s(&pNewStdin, "CONIN$", "r", stdin);
	//::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
	//::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);


	//printf("Hooking!\n");

	hooks::init();
}

DWORD WINAPI DllMain( _In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved )
{
	if (_Reason != DLL_PROCESS_ATTACH)
		return 0;
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, NULL, NULL, NULL);
	init();
	return 1;
}