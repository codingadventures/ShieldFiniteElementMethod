#include "ChronoTimer.h"


ChronoTimer::ChronoTimer()
{
	d_global_start = std::chrono::high_resolution_clock::now();
}

void ChronoTimer::Start()
{
	d_start = std::chrono::high_resolution_clock::now();
}

void ChronoTimer::Stop()
{
	d_end = std::chrono::high_resolution_clock::now(); 
	d_elapsed_seconds = d_end - d_start;
	d_total_duration += d_elapsed_seconds;
}

double ChronoTimer::ElapsedTime()
{
	return d_elapsed_seconds.count(); //retrieve milliseconds
}

double ChronoTimer::ElapsedTimeSinceStart()
{
	auto elapsedTimeSinceStart = std::chrono::high_resolution_clock::now() - d_global_start;
	return elapsedTimeSinceStart.count();
}


