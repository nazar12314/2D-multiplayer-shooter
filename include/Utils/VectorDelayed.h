#pragma once
#include <vector>

template <typename T> class VectorDelayed : public std::vector<T>
{
	std::vector<T> toAdd {};
	std::vector<T> toRemove {};

public:
	void push_back_delayed(const T& t)
	{
		toAdd.push_back(t);
	}
	void erase_delayed(const T& t)
	{
		toRemove.push_back(t);
	}

	void apply_changes()
	{
		this->insert(this->end(), toAdd.begin(), toAdd.end());
		toAdd.clear();

		for (const T& t : toRemove)
			std::erase(*this, t);
		toRemove.clear();
	}
};
