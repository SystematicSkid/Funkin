#pragma once

template <typename  T>
class ArrayObj
{
private:
	char pad[0x8];
public:
	int num_items;
	int max_items;
	T* items;
public:
	T GetItem(int i)
	{
		uintptr_t arr = *(uintptr_t*)((uintptr_t)this + 0x18);
		return *(T*)(arr + 0x8 * i);
	}
	int GetNumItems()
	{
		return *(int*)((uintptr_t)this + 0x10);
	}
};

template <typename  T>
struct ObjectGroup
{
	std::vector<T> GetObjects()
	{
		std::vector<T> ret;
		std::uintptr_t virtual_array = *reinterpret_cast<std::uintptr_t*>((uintptr_t)this + 0x20);
		if (!virtual_array)
			return ret;
		ArrayObj<T>* array_object = *reinterpret_cast<ArrayObj<T>**>(virtual_array + 0x18);

		if (!array_object)
			return ret;

		if (array_object->GetNumItems() <= 0)
			return ret;

		for (int i = 0; i < array_object->GetNumItems(); i++)
		{
			ret.push_back(array_object->GetItem(i));
		}

		return ret;
	}
};

class PlayState
{
private:
	char pad[0x128];
public:
	ObjectGroup<Note*>* active_notes;
public:
	bool InCutscene()
	{
		auto offset = *(DWORD*)(Memory::SigScan("80 BE ? ? ? ? ? 4C 8B A4 24") + 0x2);
		return *(bool*)((uintptr_t)this + offset);
	}

public:

	ObjectGroup<Note*>* GetActiveNotes()
	{
		auto offset = *(DWORD*)(Memory::SigScan("48 8B 8E ? ? ? ? 48 8D 55 8F") + 0x3);
		return *(ObjectGroup<Note*>**)((uintptr_t)this + offset);
	}
};