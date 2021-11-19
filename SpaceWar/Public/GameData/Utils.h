#pragma once

#include <random>  

namespace Utils
{
	inline float toRadian(float InAngle)
	{
		return static_cast<float>((3.14159265358979323846 * InAngle) / 180);
	}

	inline int getRandom(int InMaxValue, int InMinValue = 0)
	{
		std::mt19937 gen{ std::random_device()() };
		std::uniform_int_distribution<int> urd(0, InMaxValue);
		return  urd(gen);
	}
}
