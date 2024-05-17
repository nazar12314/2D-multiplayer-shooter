#pragma once

#include <functional>

template <typename... Ts> class Action
{
	std::vector<std::function<void(Ts...)>> callbacks {};

public:
	int subscribe(const std::function<void(Ts...)>& func)
	{
		callbacks.emplace_back(func);
		return callbacks.size() - 1;
	}
	void unsubscribe(int id)
	{
		callbacks[id] = nullptr;
	}

	void operator+=(const std::function<void(Ts...)>& func)
	{
		subscribe(func);
	}

	void operator()(Ts... args) const
	{
		for (auto& func : callbacks)
		{
			if (func == nullptr) continue;
			func(args...);
		}
	}
};
