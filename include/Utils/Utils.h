#pragma once

#include <chrono>

class Utils
{
public:
	static int measureTime(void (*function)(), int iterations = 1)
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < iterations; i++)
			function();
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		return duration / iterations;
	}

	static float compareTime(void (*function1)(), void (*function2)(), int iterations = 1)
	{
		auto time1 = measureTime(function1, iterations);
		auto time2 = measureTime(function2, iterations);
		return time1 / (float)time2;
	}
};
