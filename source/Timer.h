#ifndef TIMER_H
#define TIMER_H

#include "../3rdparty/cudaHelper/helper_timer.h"
 
/**
@brief A class which has a singleton instance serving as a global as but can also be instantiated separately
*/
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
