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
			g_Settings = *reinterpret_cast<PlayerSettings**>((DWORD)GetModuleHandleA(0) + 0xA814A0);
		
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

	PVOID original_check_key = nullptr;
	bool __fastcall hook_check_key(Input* key)
	{
		if (!g_Settings)
		{
			g_Settings = *reinterpret_cast<PlayerSettings**>((DWORD)GetModuleHandleA(0) + 0xA814A0);
			return static_cast<bool(__thiscall*)(Input*)>(original_check_key)(key);
		}
		if(!can_input)
			return static_cast<bool(__thiscall*)(Input*)>(original_check_key)(key);
		
		if(g_PlayState)
		{
			if (g_PlayState->active_notes)
			{
				auto notes = g_PlayState->active_notes->GetObjects();
				for (auto* note : notes)
				{
					if (note && note->can_be_hit)
					{
						switch (note->note_type)
						{
						case 0:
							if (key == g_Settings->controls->left || key == g_Settings->controls->left_press)
							{
								return true;
							}
							break;
						case 1:
							if (key == g_Settings->controls->down || key == g_Settings->controls->down_press)
							{
								return true;
							}
							break;
						case 2:
							if (key == g_Settings->controls->up || key == g_Settings->controls->up_press)
							{
								return true;
							}
							break;
						case 3:
							if (key == g_Settings->controls->right || key == g_Settings->controls->right_press)
							{
								return true;
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
		return static_cast<bool(__thiscall*)(Input*)>(original_check_key)(key);
	}
	
	void init()
	{
		MH_Initialize();
		DWORD module = reinterpret_cast<DWORD>(GetModuleHandle(NULL));
		DWORD address = module + 0x4188B0;
		MH_CreateHook((PVOID*)address, (PVOID*)hook_playstate, (PVOID*)&original_playstate);
		MH_EnableHook((PVOID*)address);

		address = module + 0x0F6450;
		MH_CreateHook((PVOID*)address, (PVOID*)hook_check_key, (PVOID*)&original_check_key);
		//MH_EnableHook((PVOID*)address);

		g_Settings = *reinterpret_cast<PlayerSettings**>(module + 0xA814A0);
	}
}