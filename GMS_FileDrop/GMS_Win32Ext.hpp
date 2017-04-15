#pragma once
#ifndef _GMS_Win32Ext_HPP
#define _GMS_Win32Ext_HPP
#define GMS_DLL extern "C" __declspec (dllexport)
#include <Windows.h>
#include <iostream>

#pragma region GMS
/*
	The following functions are used to construct a ds_map with C++
	and then pass them into GMS with the async social event
	Source: https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-
*/
const int EVENT_OTHER_SOCIAL = 70;
void(*CreateAsynEventWithDSMap)(int, int) = NULL;
int(*CreateDsMap)(int _num, ...) = NULL;
bool(*DsMapAddDouble)(int _index, char *_pKey, double value) = NULL;
bool(*DsMapAddString)(int _index, char *_pKey, char *pVal) = NULL;

GMS_DLL void RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4) {
	void(*CreateAsynEventWithDSMapPtr)(int, int) = (void(*)(int, int))(arg1);
	int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...))(arg2);
	bool(*DsMapAddDoublePtr)(int _index, char* _pKey, double value) = (bool(*)(int, char*, double))(arg3);
	bool(*DsMapAddStringPtr)(int _index, char* _pKey, char* value) = (bool(*)(int, char*, char*))(arg4);
	CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
	CreateDsMap = CreateDsMapPtr;
	DsMapAddDouble = DsMapAddDoublePtr;
	DsMapAddString = DsMapAddStringPtr;
}
#pragma endregion

#pragma region Win32
void ErrorCheck() {
	std::cout << "Error: " << GetLastError() << std::endl;
}

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
#pragma endregion

#endif