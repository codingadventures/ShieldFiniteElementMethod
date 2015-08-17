#ifndef Timer_h__
#define Timer_h__

#include <chrono>
#include <ctime>

class ChronoTimer
{
public:
	ChronoTimer();
	void Start();
	void Stop();

	double ElapsedTime();
	double ElapsedTimeSinceStart();
protected:
private:

	std::chrono::time_point<std::chrono::high_resolution_clock> d_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> d_global_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> d_end;
	std::chrono::duration<double, std::milli> d_elapsed_seconds;
	std::chrono::duration<double, std::milli> d_total_duration; 
};

#endif // Timer_h__
