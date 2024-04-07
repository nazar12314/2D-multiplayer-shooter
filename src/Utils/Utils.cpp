#include "Utils.h"

#include <chrono>

int Utils::measureTime(void (*function)(), int iterations)
{
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iterations; i++)
		function();
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return duration / iterations;
}

float Utils::compareTime(void (*function1)(), void (*function2)(), int iterations)
{
	auto time1 = measureTime(function1, iterations);
	auto time2 = measureTime(function2, iterations);
	return time1 / (float)time2;
}

