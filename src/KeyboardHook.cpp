// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "KeyboardHook.h"


#pragma data_seg (".SHARED")
// Windows message for communication between main executable and DLL module
UINT const WM_HOOK = WM_APP + 1;
// HWND of the main executable (managing application)
HWND hwndServer = NULL;
#pragma data_seg ()
#pragma comment (linker, "/section:.SHARED,RWS")

HINSTANCE instanceHandle = NULL;
HHOOK hookHandle;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        instanceHandle = hModule;
        hookHandle = NULL;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}




static LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(hookHandle, nCode, wParam, lParam);
    }

    // Report the event to the main window. If the return value is 1, block the input; otherwise pass it along the Hook Chain
	//In MacroBoard: Error is called for "Null object reference" on this line when program is still initializing and you are typing.
    if (SendMessage(hwndServer, WM_HOOK, wParam, lParam))
    {
        return 1;
    }

    return CallNextHookEx(hookHandle, nCode, wParam, lParam);
}


INT32 InstallHook(HWND hwndParent)
{
	if (hwndServer != NULL)
	{
		return -1;
		// Already hooked
	}

	// Register keyboard Hook
	hookHandle = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)HookCallback, instanceHandle, 0);
	if (hookHandle == NULL)
	{
		return 0;
	}
	hwndServer = hwndParent;
	return 1;
}

INT32 UninstallHook()
{
	if (hookHandle == NULL)
	{
		return 2;
	}
	// If unhook attempt fails, check whether it is because of invalid handle (in that case continue)
	if (!UnhookWindowsHookEx(hookHandle))
	{
		DWORD error = GetLastError();
		if (error != ERROR_INVALID_HOOK_HANDLE)
		{
			return 0;
		}
	}
	hwndServer = NULL;
	hookHandle = NULL;
	return 1;
}