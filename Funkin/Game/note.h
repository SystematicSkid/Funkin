#pragma once

class Note
{
private:
	char pad[0x170];
public:
	bool must_press; // 0x170
	int note_type; // 0x174
	bool can_be_hit; // 0x178
	int unk1; // 0x17C
	int unk2; // 0x180
	int unk3; // 0X184
	bool is_sustained_note;
public:
	bool IsTooLate()
	{
		return *(bool*)((uintptr_t)this + 0x179);
	}
};