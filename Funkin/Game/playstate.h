#pragma once

template <typename  T>
class ArrayObj
{
private:
	char pad[0x8];
public:
	int num_items;
	int max_items;
public:
	T GetItem(int i)
	{
		return *reinterpret_cast<T*>((uintptr_t)this + 0x18 + (0x4 * i));
	}

};

template <typename  T>
struct ObjectGroup
{
	std::vector<T> GetObjects()
	{
		std::vector<T> ret;
		std::uintptr_t virtual_array = *reinterpret_cast<std::uintptr_t*>((uintptr_t)this + 0x14);
		if (!virtual_array)
			return ret;
		ArrayObj<T>* array_object = *reinterpret_cast<ArrayObj<T>**>(virtual_array + 0xC);
		if (!array_object)
			return ret;

			if (array_object->max_items <= 0 || array_object->num_items <= 0)
				return ret;
		for (int i = 0; i < array_object->num_items; i++)
		{
			if((uintptr_t)array_object->GetItem(i) % 0x4 != 0x0)
				continue;
			ret.push_back(array_object->GetItem(i));
		}

		return ret;
	}
};

class PlayState
{
private:
	char pad[0xB4];
public:
	ObjectGroup<Note*>* active_notes;
};