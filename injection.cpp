#include "injection.h"
char szModulePath[MAX_PATH]; //={0};
WNDPROC oldWNDPROC;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DefineOffsets();

		oldWNDPROC = NULL;
		if (D2GFX_GetHwnd())																			  //only set windowhook when we can find d2's hwnd
			oldWNDPROC = (WNDPROC)SetWindowLongPtr(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)GameEventHandler); //set hook (for detecting keypresses)

		GetModuleFileName(hDll, szPath, MAX_PATH); //module path for finding ini
		PathRemoveFileSpecA(szPath);			   //remove dll from path
		strcat_s(szPath, sizeof(szPath), "\\");
		readConfig(); //call readconfig

		InstallPatchs();

		PrintText(FONTCOLOR_BROWN, "AutoTele 2.0 by WhiteEvil, improved by rsprudencio"); //print text that module is loaded

		return 1;
	};
	if (dwReason == DLL_PROCESS_DETACH)
	{
		SetWindowLongPtr(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)oldWNDPROC); //remove the hook we set at the start of the program
		RemovePatchs();

		if (TeleThread)
		{
			TerminateThread(TeleThread, 0);
			CloseHandle(TeleThread);
		}

		PrintText(FONTCOLOR_BROWN, "AutoTele: Goodbye");
	}
	return true;
};