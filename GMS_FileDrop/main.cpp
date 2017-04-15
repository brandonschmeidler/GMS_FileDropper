#include "GMS_Win32Ext.hpp"
#include <vector>
#include <string>

//GLOBALS
HWND gmWindow;
HHOOK g_hook;
HINSTANCE g_DllInstance;

//DLL ENTRY
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID IpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		g_DllInstance = hInstance;
		return DisableThreadLibraryCalls(g_DllInstance);
	}

	return TRUE;
}

//INTERNAL FUNCTIONS
std::vector<std::wstring> ExtractPathList(HDROP hDrop){
	std::vector<std::wstring> pathList;
	UINT dropCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);
	
	for (UINT i = 0; i < dropCount; i++) {
		int bufferSize = DragQueryFile(hDrop, i, NULL, NULL) + 1;

		WCHAR* path = new WCHAR[bufferSize];
		DragQueryFile(hDrop, i, path, bufferSize); 

		pathList.push_back(std::wstring(path));
		delete [] path;
	}
	return pathList;
}

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	PMSG pmsg = (PMSG)lParam;

	if (0 > nCode || PM_NOREMOVE == wParam) {
		return CallNextHookEx(g_hook, nCode, wParam, lParam);
	}

	if (pmsg->message == WM_DROPFILES) {
		HDROP hDrop = (HDROP)pmsg->wParam;
		
		std::vector<std::wstring> pathList = ExtractPathList(hDrop);
		int map = CreateDsMap(0);
		UINT dropCount = pathList.size();
		DsMapAddDouble(map, "dropCount", dropCount);
		for (UINT i = 0; i < dropCount; i++) {
			DsMapAddString(map,(char*)std::to_string(i).c_str(), (char*)utf8_encode(pathList[i]).c_str());
		}
		CreateAsynEventWithDSMap(map, EVENT_OTHER_SOCIAL);

		DragFinish(hDrop);
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

//EXPORTED FUNCTIONS
GMS_DLL double gms_filedrop_init() {
	gmWindow = GetActiveWindow();
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)HookCallback, g_DllInstance, 0);
	DragAcceptFiles(gmWindow, true);
	std::cout << "File drop initialized" << std::endl;
	return 0;
}

GMS_DLL double gms_filedrop_free() {
	UnhookWindowsHookEx(g_hook);
	g_hook = NULL;
	DragAcceptFiles(gmWindow, false);
	std::cout << "File drop freed" << std::endl;
	return 0;
}