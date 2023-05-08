#pragma once
#include <cstdint>
#include "hook.h"

namespace cheat {
	inline uint64_t qwGameAssembly, qwUnityPlayer, qwUIDAddress;

	inline bool bAutoBattle, bAutoTalk, bFastDialogue,  bSpeedhack, bPeeking, bFPSUnlock, bBlind, bUIDChanger, bMagnet;
	inline float fSpeedhack = 2.0f, fFastDialogue = 5.0f;
	inline int iFPSUnlock = 60, iUID = 1;


	void CheatThread();

	inline bool bAutoTalkTickState, bSpeedHackTickState = false;
	void SpeedHackTick();
	void AutoTalkTick();
	void Magnet();
}

void set_speed_global(float speed);
void set_speed_battle(float speed);