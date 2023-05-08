#include "utils.h"
#include "bypass.h"

#include <iostream>
#include <windows.h>

namespace bypass {
	uint32_t LDR_LOAD_DLL = 0xFCDC0;
	uint32_t CHEAT_ENGINE = 0xF9940;

	uint64_t base_address = 0;

	bool check() {
		uint64_t checksum = 0;
			
		checksum ^= utils::read<uint64_t>(base_address + LDR_LOAD_DLL);
		printf("[Info] Checksum LDR_LOAD_DLL: %llx\n", checksum);
		checksum ^= utils::read<uint64_t>(base_address + CHEAT_ENGINE);
		printf("[Info] Checksum CHEAT ENGINE: %llx\n\n", checksum);

		if (checksum < 0x75000000ull || checksum > 0x76000000ull) {
			printf("[Error] Unsupported SRB version -> 0x%I64X\n", checksum);
			return FALSE;
		}

		return TRUE;
	}

	void init() {

		while ((base_address = reinterpret_cast<uint64_t>(GetModuleHandleA("starrailbase.dll"))) == 0);

		utils::suspend_other_threads();

		if (check()) {
			printf("[Info] Before bypass Value\n");
			printf("[Info] LDR_LOAD_DLL Value: %llx\n", utils::read<uint64_t>(base_address + LDR_LOAD_DLL));
			printf("[Info] CHEAT ENGINE Value: %llx\n", utils::read<uint64_t>(base_address + CHEAT_ENGINE));
			utils::write<uint64_t>(base_address + LDR_LOAD_DLL, 0xCCCCC300000000B8ull); // bypass for dll injections (speedhack in cheat engine / debuggers)
			utils::write<uint8_t>(base_address + CHEAT_ENGINE, 0xC3); // bypass for cheat engine
			printf("\n[Info] After bypass Value\n");
			printf("[Info] LDR_LOAD_DLL Value: %llx\n", utils::read<uint64_t>(base_address + LDR_LOAD_DLL));
			printf("[Info] CHEAT ENGINE Value: %llx\n", utils::read<uint64_t>(base_address + CHEAT_ENGINE));
		}
		else {
			printf("[Error] Failed to init bypass\n");

			Sleep(5000);
			TerminateProcess(GetCurrentProcess(), 0);
		}

		utils::resume_other_threads();
	}
}
