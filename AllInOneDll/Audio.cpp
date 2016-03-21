#include "StdAfx.h"
#include "Audio.h"
#include <math.h>
#include <vector>


#pragma comment(lib,"NIDAQmx.lib") 
#define PI 3.141529



using namespace std;
Audio::Audio(void)
{
}


Audio::~Audio(void)
{
}

/************************************************************************
初始化                                                               
sampleNum : 采样率
volatage :电压值
************************************************************************/
int Audio::InitialNI4461(double volatage,int sampleNum)
{
	int err = 0;

	if (!AudioOut)
	{
		double minVoltage = 0 - volatage;
		double maxVolatage = volatage;

		err = DAQmxCreateTask("AudioOut", &AudioOut);
		if (err != 0)
		{
			return err;
		}
		err = DAQmxCreateAOVoltageChan(AudioOut, "Dev1/ao0", "电压输出", minVoltage, maxVolatage, DAQmx_Val_Volts, "");
		if (err != 0)
		{
			return err;
		}
		err = DAQmxSetChanAttribute(AudioOut, "电压输出", DAQmx_AO_TermCfg, DAQmx_Val_PseudoDiff);
		if (err != 0)
		{
			return err;
		}
	}
	else
	{
		return -1;
	}


	return 0;
}

/************************************************************************
发声                                                                 
sample ： 待写入采样
************************************************************************/
int Audio::SountOut(int sample,int time,int sampleTotal)
{
	if (AudioOut)
	{
		int err = 0;
		err =  DAQmxCfgSampClkTiming(AudioOut, "", sampleTotal, DAQmx_Val_Rising, DAQmx_Val_ContSamps, sample);
		if (err != 0)
		{
			return err;
		}

		err = DAQmxSetWriteAttribute(AudioOut, DAQmx_Write_RegenMode, DAQmx_Val_AllowRegen);
		if (err != 0)
		{
			return err;
		}

		float64 *data = new float64[sample];
		memset(data,0,sample);
		int times = 360 / sample;
		for(int i=0; i<sample; i++)
		{
			data[i] =  sin(PI*i*times/180);
		}
		err = DAQmxWriteAnalogF64(AudioOut,20,1,10.0,DAQmx_Val_GroupByChannel,data,NULL,NULL);
		if (err != 0)
		{
			return err;
		}
		Sleep(time*1000);
		CloseNI4461();
		delete data;
	}
	else
	{
		return -1;
	}

	return 0;
}

/************************************************************************
关闭NI4461
************************************************************************/
int Audio::CloseNI4461(void)
{
	int err = 0;
	if (AudioOut)
	{
		err = DAQmxStopTask(AudioOut);
		if (err != 0)
		{
			return err;
		}
		err = DAQmxClearTask(AudioOut);
		if (err != 0)
		{
			return err;
		}
		AudioOut = NULL;
	}
	return 0;
}

/************************************************************************
扫频模式
************************************************************************/
int Audio::SweepFrequency(int sampleTotal,int time,int minFrequency,int maxFrequency,int step)
{
	if(AudioOut)
	{
		int err = 0;
		int sample = 0; //每个频率需要写入的采样数
		err =  DAQmxCfgSampClkTiming(AudioOut, "", sampleTotal, DAQmx_Val_Falling, DAQmx_Val_ContSamps, 20);
		if (err != 0)
		{
			return err;
		}

		err = DAQmxSetWriteAttribute(AudioOut, DAQmx_Write_RegenMode, DAQmx_Val_DoNotAllowRegen);
		if (err != 0)
		{
			return err;
		}
		for(int f=minFrequency; f<=maxFrequency; f+=step)
		{
			sample = sampleTotal / f;
			double data[1000] = {0};
			for(int i=0; i<1000; i++)
			{
				data[i] = sin(2*PI*i/sample);
			}

			DAQmxWriteAnalogF64(AudioOut,sample,1,10.0,DAQmx_Val_GroupByChannel,data,NULL,NULL);
		}
		CloseNI4461();
		return 0;
	}
}
