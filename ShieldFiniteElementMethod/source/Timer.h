#ifndef TIMER_H
#define TIMER_H

#include <cudaHelper/helper_timer.h>
 
class Timer
{
public:
	Timer();
	~Timer();
	StopWatchInterface *m_timer;
	int fpsCount;        
	float fpsTotalCount; // FPS count for averaging
	int fpsLimit;        // FPS limit for sampling
	float ifps;
	float m_average;
	float computeFPS();
	void reset();
	float getAverage();
};
#endif // !TIMER_H