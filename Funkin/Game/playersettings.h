#pragma once
class Input
{
private:
	char pad[0x18];
public:
	char* name;
public:
	void Simulate()
	{
		/*
		   cmp     [esi+24h], eax -- eax = game tick
		   jnz     short loc_916C26
		   mov     al, [esi+28h]
		   mov     [esi+14h], al
		   pop     edi
		   pop     esi
		   pop     ebx
		   mov     esp, ebp
		   pop     ebp
		   retn
		 */
		printf("Simulating: %s\n", name);
		*(bool*)((DWORD64)this + 0x44) = 0x1;
		FlxGame* game = *(FlxGame**)((DWORD64)GetModuleHandleA(0) + 0x00CC9CD0); // A1 ? ? ? ? 8B 80 ? ? ? ? 39 46 24 
		DWORD64 game_tick = game->tick;
		*(DWORD*)((DWORD64)this + 0x40) = game_tick;
	}
	void Reset()
	{
		*(bool*)((uintptr_t)this + 0x24) = 0x0;
	}
};

class Controls
{
private:
	char pad[0x30];
public:
	Input* up;
	Input* down;
	Input* left;
	Input* right;
	Input* up_press;
	Input* left_press;
	Input* right_press;
	Input* down_press;
};

class PlayerSettings
{
private:
	char pad[0x10];
public:
	Controls* controls;
};