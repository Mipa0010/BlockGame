#pragma once
#include <SDL\SDL_timer.h>

struct Time
{
	Time()
	{
		start_time = SDL_GetPerformanceCounter();
		time_now = start_time;
		last_frame_time = start_time;
		delta_time = 0;
	}

	void Update()
	{
		last_frame_time = time_now;
		time_now = SDL_GetPerformanceCounter();
		delta_time = static_cast<float>(static_cast<double>((SDL_GetPerformanceCounter() - last_frame_time)) / SDL_GetPerformanceFrequency());
	}

	float Elapsed()
	{
		return (static_cast<double>(time_now - start_time) / SDL_GetPerformanceFrequency());
	}

	uint64_t start_time;
	uint64_t time_now;
	uint64_t last_frame_time;
	float delta_time;
};

struct Timer
{
	Timer()
	{
		start_time = SDL_GetPerformanceCounter();
	}

	void Reset()
	{
		start_time = SDL_GetPerformanceCounter();
	}

	float Elapsed()
	{
		return (static_cast<double>(SDL_GetPerformanceCounter() - start_time) / SDL_GetPerformanceFrequency());
	}

	uint64_t start_time;
};