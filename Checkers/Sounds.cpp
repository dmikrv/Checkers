#include "Sounds.h"
#include <Windows.h>
#include "resource.h"

#define SOUND_MOVE IDR_WAVE1
#define SOUND_TAKING IDR_WAVE2
#define SOUND_INACCESSIBLE_CH IDR_WAVE5
#define SOUND_SELECT_CHECKER IDR_WAVE6
#define SOUND_SELECT_BUTTON IDR_WAVE8
#define SOUND_PRESS_BUTTON IDR_WAVE10

DWORD WINAPI sMove(void* p)
{
	//PlaySound(L"sounds/demo.wav", NULL, SND_NODEFAULT | SND_LOOP);
	PlaySound(MAKEINTRESOURCE(SOUND_MOVE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sTaking(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_TAKING), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sInaccessibleCh(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_INACCESSIBLE_CH), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sSelectChecker(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_SELECT_CHECKER), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sSelectButton(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_SELECT_BUTTON), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC 
		| SND_NOSTOP);
	return 0;
}

DWORD WINAPI sPressButton(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_PRESS_BUTTON), GetModuleHandle(NULL), SND_RESOURCE);
	return 0;
}

DWORD WINAPI sStop(void* p)
{
	PlaySound(NULL, 0, 0);
	return 0;
}

void sMoveThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sMove, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sTakingThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sTaking, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sInaccessibleChThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sInaccessibleCh, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sSelectCheckerThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sSelectChecker, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sSelectButtonThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sSelectButton, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sPressButtonThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sPressButton, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sStopThread()
{
	PlaySound(NULL, 0, 0);
	//CreateThread(nullptr, 0, sStop, nullptr, 0, nullptr);
	//HANDLE hThread = CreateThread(nullptr, 0, sStop, nullptr, 0, nullptr);
	//CloseHandle(hThread);
}

