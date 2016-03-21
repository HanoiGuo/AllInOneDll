#pragma once
#include "NIDAQmx.h"
class Audio
{
public:
	Audio(void);
	~Audio(void);
private:
	TaskHandle AudioOut;
public:
	int InitialNI4461(double volatage,int sampleNum);
	int SountOut(int sample,int time,int sampleTotal);
	int CloseNI4461(void);
	int SweepFrequency(int sampleTotal,int time,int minFrequency,int maxFrequency,int step);
};

