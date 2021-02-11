#pragma once

class FlxGame
{
public:
	static FlxGame* Instance;
private:
	char pad[0x158];
public:
	DWORD tick;
};

