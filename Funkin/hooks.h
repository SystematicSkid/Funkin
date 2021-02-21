#pragma once
#include <map>
#include "MinHook.h"

namespace hooks
{
	static PlayerSettings* g_Settings = nullptr;
	PlayState* g_PlayState;
	Note* last_note;
	ptr g_CurrentSongPos = 0;
	int last_note_tick = 0;
	
	static bool can_input = false;
	const int release_ticks = 125;

	std::map<Input*, int> simulate_ticks;
	
	PVOID original_playstate = nullptr;
	void __fastcall hook_playstate(PlayState* play_state)
	{
		//printf("Playstate: 0x%p\n", play_state);
		////printf("Settings: 0x%p\n", g_Settings);
		if (!g_Settings)
			g_Settings = *reinterpret_cast<PlayerSettings**>(Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 88 45 8E"));
		int tick = FlxGame::Instance->tick;
		if(play_state->active_notes && !play_state->InCutscene())
		{
			auto notes = play_state->GetActiveNotes()->GetObjects();
			for(auto* note : notes)
			{
				if (note)
				{
					switch (note->note_type)
					{
					case 0:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
						{
							g_Settings->controls->left_press->Simulate();
							simulate_ticks[g_Settings->controls->left_release] = tick;
							last_note_tick = tick;
						}
						else
						{
							g_Settings->controls->left_press->Reset();
						}
						break;
					case 1:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
						{
							g_Settings->controls->down_release->Reset();
							g_Settings->controls->down_press->Simulate();
							simulate_ticks[g_Settings->controls->down_release] = tick;
							last_note_tick = tick;

						}
						else
						{
							g_Settings->controls->down_press->Reset();
						}
						break;
					case 2:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
						{
							g_Settings->controls->up_release->Reset();
							g_Settings->controls->up_press->Simulate();
							simulate_ticks[g_Settings->controls->up_release] = tick;
							last_note_tick = tick;

						}
						else
						{
							g_Settings->controls->up_press->Reset();
						}
						break;
					case 3:
						if (note->can_be_hit && note->must_press && !note->IsTooLate())
						{
							g_Settings->controls->right_release->Reset();
							g_Settings->controls->right_press->Simulate();
							simulate_ticks[g_Settings->controls->right_release] = tick;
							last_note_tick = tick;


						}
						else
						{
							g_Settings->controls->right_press->Reset();
						}
						break;
					default:
						break;
					}
				}
			}
		}

		for (auto& [key, value] : simulate_ticks)
		{
			if (tick - value > release_ticks)
			{
				key->Simulate();
				simulate_ticks.erase(key);
			}
		}

		if (tick - last_note_tick > 500)
		{
			play_state->SetHoldTimer(1.0);
		}

		return static_cast<void(__thiscall*)(PlayState*)>(original_playstate)(play_state);

		
	}

	PVOID original_popupscore = nullptr;
	void __fastcall hook_popupscore(PlayState* play_state, double strum_time)
	{
		/* Cause abs value to always be 0.0 */
		double current_song_pos = *(double*)(g_CurrentSongPos); //F2 0F 5C 35 ? ? ? ? 0F 54 35 ? ? ? ?
		//printf("Pos: %f\n", current_song_pos);
		return static_cast<void(__fastcall*)(PlayState*, double)>(original_popupscore)(play_state, current_song_pos);
	}
	
	void init()
	{
		MH_Initialize();
		DWORD64 module = reinterpret_cast<DWORD64>(GetModuleHandle(NULL));
		DWORD64 address = Memory::SigScan("48 8B C4 55 56 48 8D A8"); // 55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 53 8B D9 56 64 8B 00 57 C6 83 ? ? ? ? ? 
		MH_CreateHook((PVOID*)address, (PVOID*)hook_playstate, (PVOID*)&original_playstate);
		MH_EnableHook((PVOID*)address);

		address = Memory::GetCallAddress("E8 ? ? ? ? FF 83 ? ? ? ? 48 8B 07", "E8 ? ? ? ? FF 83 ? ? ? ? EB 14");
		MH_CreateHook((PVOID*)address, (PVOID*)hook_popupscore, (PVOID*)&original_popupscore);
		MH_EnableHook((PVOID*)address);
		//
		auto pos_address = Memory::SigScan("F2 0F 5C 35 ? ? ? ? 0F 54 35");
		pos_address = (ptr)(pos_address + *(signed long*)(pos_address + 4) + 8);
		g_CurrentSongPos = pos_address;

		g_Settings = *reinterpret_cast<PlayerSettings**>(Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 88 45 8E"));
	}
}