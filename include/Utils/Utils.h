#pragma once
#include <memory>

template <typename T> using SPtr = std::shared_ptr<T>;
template <typename T> using UPtr = std::unique_ptr<T>;

class Utils
{
public:
	static int measureTime(void (*function)(), int iterations = 1);
	static float compareTime(void (*function1)(), void (*function2)(), int iterations = 1);
};

