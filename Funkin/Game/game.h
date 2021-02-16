#pragma once

class FlxGame
{
public:
	static FlxGame* Instance;
private:
	char pad[0x240];
public:
	DWORD tick;
};

