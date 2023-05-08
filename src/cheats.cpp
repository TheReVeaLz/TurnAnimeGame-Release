#pragma once
#include <windows.h>
#include "cheats.h"
#include "utils.h"
#include <vector>
#include "game.h"
#include "hook.h"
#include "dllmain.h"
#include <future>
#include <MinHook.h>

void set_speed_global(float speed) {
	utils::write<float>(utils::read<__int64>(cheat::qwUnityPlayer +0x1D21D78) + 0xFC, speed);
}

void set_speed_battle(float speed) {
	utils::write<float>(utils::read<__int64>(utils::read<__int64>(cheat::qwGameAssembly + 0x8CAA6A0) + 0xC0) + 0x1DC, speed);
}

void cheat::SpeedHackTick() {
	if (cheat::bSpeedHackTickState) ExitThread(0);
	cheat::bSpeedHackTickState = !cheat::bSpeedHackTickState;
	while (cheat::bSpeedhack) {
		if (cheat::bFastDialogue && hooks::game::get_is_in_dialog()) { Sleep(300); continue; }
		if (hooks::game::current_phase == RPG_BATTLE) {
			set_speed_battle(cheat::fSpeedhack);
			set_speed_global(1.f);
		} else {
			set_speed_global(cheat::fSpeedhack);
			set_speed_battle(1.f);
		}
		Sleep(500);
	}
	set_speed_global(1.f);
	set_speed_battle(1.f);
	cheat::bSpeedHackTickState = !cheat::bSpeedHackTickState;
	ExitThread(0);
}

void cheat::AutoTalkTick() {
	if (cheat::bAutoTalkTickState) ExitThread(0);
	cheat::bAutoTalkTickState = !cheat::bAutoTalkTickState;
	while (cheat::bAutoTalk) {
		while (GetForegroundWindow() == Process::Hwnd && hooks::game::get_is_in_dialog()) {
			if (cheat::bFastDialogue) set_speed_global(cheat::fFastDialogue);
			keybd_event(VK_SPACE, 0, 0, 0);
			Sleep(25);
			keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
			Sleep(25);
		}
		Sleep(450);
	}
	cheat::bAutoTalkTickState = !cheat::bAutoTalkTickState;
	ExitThread(0);
}

void cheat::Magnet() {
	if (cheat::bMagnet && utils::read<uint32_t>(cheat::qwUnityPlayer + 0xD3E2AF) == 0x04C2C20F) {
		utils::write<uint32_t>(cheat::qwUnityPlayer + 0xD3E2AF, 0x04D2C20F);
	}
	else {
		utils::write<uint32_t>(cheat::qwUnityPlayer + 0xD3E2AF, 0x04C2C20F);
	}
}

void cheat::CheatThread() {
	if (cheat::bPeeking) {
		utils::write<uint8_t>(cheat::qwGameAssembly + 0x51292C0, 0xC3);
	} else {
		utils::write<uint8_t>(cheat::qwGameAssembly + 0x51292C0, 0x40);
	}

	if (cheat::bFPSUnlock) {
		utils::write<uint8_t>(cheat::qwUnityPlayer + 0x1C4E000, cheat::iFPSUnlock);
	} else {
		utils::write<uint8_t>(cheat::qwUnityPlayer + 0x1C4E000, 60);
	}
}