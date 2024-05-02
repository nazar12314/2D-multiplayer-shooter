#pragma once

#include <iostream>
#include <sstream>
#include <string>

enum class LogType
{
	// Powers of 2
	Default = 1,
};

inline LogType operator|(LogType a, LogType b)
{
	return (LogType)((int)a | (int)b);
}
inline bool hasFlag(LogType flags, LogType flag)
{
	return ((int)flags & (int)flag) != 0;
}

class Debug
{
public:
	inline static LogType enabledLogTypes = LogType::Default;

	template <typename... P>
	static void log(LogType logType, const P&... params)
	{
		if (!hasFlag(enabledLogTypes, logType)) return;
		std::stringstream stream;
		(stream << ... << params);
		std::cout << stream.str() << std::endl;
	}
};
