#ifndef _COUNT_TIME_H
#define _COUNT_TIME_H

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

class TCountTime
{
public:
	TCountTime(){}
	~TCountTime(){}
	
public:
	void vBegin()
	{
		gettimeofday(&m_tBegin, 0);
	}
	
	void vEnd()
	{
		gettimeofday(&m_tEnd, 0);
	}
	
	int iCountUsec()
	{
		return (m_tEnd.tv_sec - m_tBegin.tv_sec)*1000000 + (m_tEnd.tv_usec - m_tBegin.tv_usec);
	}

public:
	timeval m_tBegin;
	timeval m_tEnd;
};

#endif

