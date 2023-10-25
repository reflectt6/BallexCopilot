#pragma once
#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include <comdef.h>  
#include <psapi.h>
using namespace std;


long GetProcessIdFromName(const char* name);

DWORD_PTR getModuleBaseAddr(HANDLE processHandle, char targetFileName[]);

DWORD_PTR getMultiLevelAddressFromBase(HANDLE handle, DWORD_PTR baseAddr, DWORD multiLevelAddress[], int multiLevelLen);

bool hackBallexBlood(HANDLE processHandle, DWORD_PTR targetAddr);
