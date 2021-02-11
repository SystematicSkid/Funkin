#pragma once
class Input
{
private:
	char pad[0xC];
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
		*(bool*)((DWORD)this + 0x28) = 0x1;
		FlxGame* game = *(FlxGame**)((DWORD)GetModuleHandleA(0) + 0xA831E8);
		DWORD game_tick = game->tick;
		*(DWORD*)((DWORD)this + 0x24) = game_tick;
	}
	void Reset()
	{
		*(bool*)((uintptr_t)this + 0x14) = 0x0;
	}
};

class Controls
{
private:
	char pad[0x18];
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
	char pad[0x8];
public:
	Controls* controls;
};