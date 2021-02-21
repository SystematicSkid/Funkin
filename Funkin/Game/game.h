#pragma once

class FlxGame
{
public:
	static FlxGame*& Instance;
private:
	char pad[0x240];
public:
	DWORD tick;
};

FlxGame*& FlxGame::Instance = *(FlxGame**)(Memory::GetInstanceAddress("48 8B 05 ? ? ? ? 8B 88 ? ? ? ? 39 4B 40"));