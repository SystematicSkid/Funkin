#pragma once
#include "MinHook.h"

namespace hooks
{
	static PlayerSettings* g_Settings = nullptr;
	PlayState* g_PlayState;
	Note* last_note;
	
	static bool can_input = false;
	
	PVOID original_playstate = nullptr;
	void __fastcall hook_playstate(PlayState* play_state)
	{
		g_PlayState = play_state;
		can_input = true;
		if (!g_Settings)
			g_Settings = *reinterpret_cast<PlayerSettings**>((DWORD)GetModuleHandleA(0) + 0x00CC6FE8);
		if(play_state->active_notes)
		{
			auto notes = play_state->active_notes->GetObjects();
			for(auto* note : notes)
			{
				if (note)
				{
					switch (note->note_type)
					{
					case 0:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
							g_Settings->controls->left_press->Simulate();
						else
							g_Settings->controls->left_press->Reset();
						break;
					case 1:
						if (note->can_be_hit && note->must_press && !note->IsTooLate() && (note != last_note && !note->is_sustained_note))
						{
							g_Settings->controls->down_press->Simulate();
							last_note = note;
						}
						else
							g_Settings->controls->down_press->Reset();
						break;
					case 2:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
							g_Settings->controls->up_press->Simulate();
						else
							g_Settings->controls->up_press->Reset();
						break;
					case 3:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
							g_Settings->controls->right_press->Simulate();
						else
							g_Settings->controls->right_press->Reset();
						break;
					default:
						break;
					}
				}
			}
		}
		static_cast<void(__thiscall*)(PlayState*)>(original_playstate)(play_state);
		can_input = false;
		return;
	}

	PVOID original_popupscore = nullptr;
	void __fastcall hook_popupscore(PlayState* play_state, double strum_time)
	{
		/* Cause abs value to always be 0.0 */
		double current_song_pos = *(double*)((uintptr_t)GetModuleHandle(0) + 0x00CC8740); //F2 0F 5C 35 ? ? ? ? 0F 54 35 ? ? ? ? 
		return static_cast<void(__fastcall*)(PlayState*, double)>(original_popupscore)(play_state, current_song_pos);
	}
	
	void init()
	{
		MH_Initialize();
		DWORD64 module = reinterpret_cast<DWORD64>(GetModuleHandle(NULL));
		DWORD64 address = module + 0x004B9A00; // 55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 53 8B D9 56 64 8B 00 57 C6 83 ? ? ? ? ? 
		MH_CreateHook((PVOID*)address, (PVOID*)hook_playstate, (PVOID*)&original_playstate);
		MH_EnableHook((PVOID*)address);

		address = module + 0x004B7390; // E8 ? ? ? ? FF 83 ? ? ? ? 48 8B 07 ? ? 
		MH_CreateHook((PVOID*)address, (PVOID*)hook_popupscore, (PVOID*)&original_popupscore);
		MH_EnableHook((PVOID*)address);
		//

		g_Settings = *reinterpret_cast<PlayerSettings**>(module + 0x00CC6FE8);
	}
}