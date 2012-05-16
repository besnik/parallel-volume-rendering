// timer.cpp

#include <stdio.h>
#if defined WIN32
#include <windows.h>
#else
	#include <sys/time.h>
#endif

#include "../include/timer.h"

#if ! defined WIN32

double timer()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec * 1e6 + t.tv_usec)/1000.0;
}

#else

double timer()
{
	LARGE_INTEGER timerFreq, timerCount;

	if (! QueryPerformanceFrequency(&timerFreq)) {
		fprintf(stderr, "determing timer frequency failed\n");
		exit(1);
	}
	QueryPerformanceCounter(&timerCount);
	
	return (timerCount.QuadPart * 1000.0)/timerFreq.QuadPart;
}

#endif
