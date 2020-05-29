#include "Sounds.h"
#include <Windows.h>
#include "resource.h"

#define SOUND_MOVE IDR_WAVE1
#define SOUND_TAKING IDR_WAVE2
#define SOUND_TAKING_KING IDR_WAVE3
#define SOUND_BEGIN_GAME IDR_WAVE4
#define SOUND_INACCESSIBLE_CH IDR_WAVE5
#define SOUND_PRESS_MOUSE IDR_WAVE6
#define SOUND_ROTATE_PLAYER IDR_WAVE7
#define SOUND_SELECT_BUTTON IDR_WAVE8
#define SOUND_WIN_GAME IDR_WAVE9
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

DWORD WINAPI sTakingKing(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_TAKING_KING), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sBeginGame(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_BEGIN_GAME), GetModuleHandle(NULL), SND_RESOURCE);
	return 0;
}

DWORD WINAPI sInaccessibleCh(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_INACCESSIBLE_CH), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sPressMouse(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_PRESS_MOUSE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sRotatePlayer(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_ROTATE_PLAYER), GetModuleHandle(NULL), SND_RESOURCE);
	return 0;
}

DWORD WINAPI sSelectButton(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_SELECT_BUTTON), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sWinGame(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_WIN_GAME), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	return 0;
}

DWORD WINAPI sPressButton(void* p)
{
	PlaySound(MAKEINTRESOURCE(SOUND_PRESS_BUTTON), GetModuleHandle(NULL), SND_RESOURCE);
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

void sTakingKingThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sTakingKing, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sBeginGameThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sBeginGame, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sInaccessibleChThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sInaccessibleCh, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sPressMouseThread()
{
	//HANDLE hThread = CreateThread(nullptr, 0, sPressMouse, nullptr, 0, nullptr);
	//CloseHandle(hThread);
}

void sRotatePlayerThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sRotatePlayer, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sSelectButtonThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sSelectButton, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sWinGameThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sWinGame, nullptr, 0, nullptr);
	CloseHandle(hThread);
}

void sPressButtonThread()
{
	HANDLE hThread = CreateThread(nullptr, 0, sPressButton, nullptr, 0, nullptr);
	CloseHandle(hThread);
}