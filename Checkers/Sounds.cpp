#include "Sounds.h"
#include <Windows.h>

DWORD WINAPI sMario(LPVOID param)
{
	Beep(659, 200);
	Beep(659, 200);
	Sleep(200);
	Beep(659, 200);
	Sleep(100);
	Beep(523, 200);
	Beep(659, 200);
	Sleep(200);
	Beep(783, 200);
	Sleep(400);
	Beep(391, 200);
	return 0;
}

DWORD WINAPI sMoveChecker(void* p)
{
	PlaySound(L"sounds/move_checker.wav", NULL, SND_NODEFAULT);
	return 0;
}

DWORD WINAPI sMoveKing(void* p)
{
	PlaySound(L"sounds/move_checker_king.wav", NULL, SND_NODEFAULT);
	return 0;
}

//DWORD WINAPI sDemo(void* p)
//{
//	PlaySound(L"sounds/demo.wav", NULL, SND_NODEFAULT | SND_LOOP);
//	return 0;
//}

void sMarioThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sMario, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sMoveCheckerThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sMoveChecker, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sMoveKingThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sMoveKing, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

//void sDemoThread()
//{
//	CreateThread(nullptr, 0, sDemo, nullptr, 0, nullptr);
//}

//
//struct BeepData
//{
//	DWORD freq;
//	DWORD dur;
//};
//
//// запуск BeepFunc отдельным потоком
//void beep(DWORD freq, DWORD dur)
//{
//	BeepData* bdata = new BeepData();
//	bdata->dur = dur;
//	bdata->freq = freq;
//
//	HANDLE hThread = CreateThread(nullptr, 0, BeepFunc, bdata, 0, nullptr);
//	CloseHandle(hThread);
//}
//
//DWORD WINAPI BeepFunc(LPVOID param)
//{
//	BeepData bdata = *((BeepData*)param);
//	Beep(bdata.freq, bdata.dur);
//	return 0;
//}


