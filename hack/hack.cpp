#include "hack.h"


long GetProcessIdFromName(const char* name)
{
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		cout << "Create TlHelp32 Error!" << endl;
		return -1;
	}

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &processer);
	while (flag != 0)
	{
		_bstr_t processName(processer.szExeFile);  //WCHAR�ַ���ת����CHAR�ַ���
		if (strcmp(processName, name) == 0)
		{
			return processer.th32ProcessID;        //���ؽ���ID
		}
		flag = Process32Next(hsnapshot, &processer);
	}

	CloseHandle(hsnapshot);
	return -2;
}

DWORD_PTR getModuleBaseAddr(HANDLE processHandle, char targetFileName[]) {
	
	HMODULE lphModule[1024];
	DWORD lpcbNeeded;
	DWORD   dwFilterFla = 0x03;// �г�����ģ��
	// https://learn.microsoft.com/zh-cn/windows/win32/api/psapi/nf-psapi-enumprocessmodulesex
	if (EnumProcessModulesEx(processHandle, lphModule, sizeof(lphModule), &lpcbNeeded, 0x03)) {
		// ��������module
		for (UINT i = 0; i < (lpcbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szFileName[MAX_PATH];
			if (GetModuleFileNameEx(processHandle, lphModule[i], szFileName, _countof(szFileName) - 1)) {
				//wcout << szFileName << endl;
			}
			else {
				cout << "Error in GetModuleFileNameEx()" << endl;
				continue;
			}
		
			// ��дƥ���㷨����java��һ��contains�͸㶨�ˣ�������cܳ�� ���ܳ
			for (int j = 0; j < sizeof(szFileName) - sizeof(targetFileName); j++) {
				bool finded = true;
				bool findEnd = false;
				for (size_t k = 0; k < sizeof(targetFileName); k++)
				{
					// TCHAR szFileName[MAX_PATH]; ��Ȼÿ�ֶ���һ���µ�����������ʵ������ͬһ����ַ���㲻��ʲôԭ��
					// ��һ��ģ����ǰһ��ģ���ֵ���ǵ���
					// ���ǰһ��ģ���·�����ں�һ��������Щ�͸��ǲ�����
					// ��ô�ж��Ƿ�����أ�����ĩβ��һ��'\0', ܳ
					if (szFileName[j] == '\0') {
						findEnd = true;
						finded = false;
						break;
					}
					if (szFileName[j + k] != targetFileName[k]) {
						finded = false;
						break;
					}
				}
				if (finded) {
					wcout << "Nice! I find the target module" << szFileName << endl;
					MODULEINFO lpmodinfo;
					bool b = GetModuleInformation(processHandle, lphModule[i], &lpmodinfo, sizeof(lpmodinfo));
					if (!b) {
						return NULL;
					}
					//cout << lpmodinfo.EntryPoint << endl; //ģ����ڵ�
					cout << "Find module base��" << lpmodinfo.lpBaseOfDll << endl; // ģ�����ַ
					return (DWORD_PTR)lpmodinfo.lpBaseOfDll;
				}
				if (findEnd) {
					break;
				}
			}
		}
	}
	else {
		cout << "Error in scan modules" << endl;
	}
	return NULL;
}

DWORD_PTR getMultiLevelAddressFromBase(HANDLE handle, DWORD_PTR baseAddr, DWORD multiLevelAddress[], int multiLevelLen) {
	DWORD_PTR res = baseAddr;
	for (size_t i = 0; i < multiLevelLen - 1; i++)
	{
		DWORD_PTR tmp = res + multiLevelAddress[i];
		//https://learn.microsoft.com/zh-cn/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory
		if (!ReadProcessMemory(handle, (void*)tmp, &res, 8, 0))
		{
			cout << "Error in getMultiLevelAddressFromBase()!" << endl;
			return NULL;
		}
		//cout << res << endl;
	}
	DWORD_PTR tmp = res + multiLevelAddress[multiLevelLen - 1];
	cout << "Find target address��" << tmp << endl;
	return tmp;
}

bool hackBallexBlood(HANDLE processHandle, DWORD_PTR targetAddr) {
	float targetNum = 100.0;
	// https://learn.microsoft.com/zh-cn/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
	if (!WriteProcessMemory(processHandle, (LPVOID)targetAddr, &targetNum, sizeof(float), NULL))
	{
		cout << "Error in writing to targetAddr" << endl;
		return false;
	}
	return true;
}





//int main()
//{
//	// get handle from name
//	const char* processName = "Ballex.exe";
//	long pid = GetProcessIdFromName(processName);
//	cout << "��ȡ Ballex.exe pid : " << pid << endl;
//	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//
//	// find module base
//	char targetModuleName[] = "UnityPlayer.dll";
//	DWORD_PTR baseAddr = getModuleBaseAddr(processHandle, targetModuleName);
//
//	// find target addr
//	DWORD multiLevelAddress[6] = { 0x014D9540, 0x68, 0x10, 0x28, 0x50, 0x118 };
//	DWORD_PTR targetAddr = getMultiLevelAddressFromBase(processHandle, baseAddr, multiLevelAddress, 6);
//	
//	// hack
//	hackBallexBlood(processHandle, targetAddr);
//	
//	system("pause");
//	return 0;
//}