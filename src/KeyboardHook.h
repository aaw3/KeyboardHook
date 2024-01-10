#ifdef KEYBOARDHOOK_EXPORTS
#define KEYBOARDHOOK_API extern "C" __declspec(dllexport)
#else
#define KEYBOARDHOOK_API extern "C" __declspec(dllimport)
#endif

KEYBOARDHOOK_API INT32 InstallHook(HWND hwndParent);
KEYBOARDHOOK_API INT32 UninstallHook();