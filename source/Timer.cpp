#include "Timer.h"


Timer::Timer()
{
	sdkCreateTimer(&m_timer);
	reset();
}

Timer::~Timer()
{
	sdkDeleteTimer(&m_timer);
}
#ifndef MAX
#define MAX(a,b) ((a > b) ? a : b)
#endif

float Timer::computeFPS()
{
	fpsCount++;

	if (fpsCount >= fpsLimit)
	{
		char fps[256];
		ifps = 1.f / (sdkGetAverageTimerValue(&m_timer) / 1000.f);

		//sprintf(fps, "Volume Render: %3.1f fps", ifps);

		//glutSetWindowTitle(fps);
		fpsTotalCount += fpsCount;
		m_average = m_average * ((fpsTotalCount-1)/fpsTotalCount) + ifps/fpsTotalCount;
		fpsCount = 0;

		fpsLimit = (int)MAX(1.f, ifps);
		sdkResetTimer(&m_timer);
	}
	return ifps;
}

void Timer::reset()
{
	m_average = 0;
	fpsCount = 0;
	fpsTotalCount = 0;
	fpsLimit = 1;
	sdkResetTimer(&m_timer);
}

float Timer::getAverage()
{
	return m_average;
}
