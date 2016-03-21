#ifndef _ENTRANCE_H
#define _ENTRANCE_H
#pragma once 
#include "StdAfx.h"


/************************************************************************
控制设备的基本动作出来
************************************************************************/
int OutFunction();



/************************************************************************
declare the entrance function,and show the dialog
************************************************************************/
extern "C" __declspec(dllexport) void __stdcall ShowDlg();


/************************************************************************

************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoRunFunction();


/************************************************************************
C# call this function to close NI6509
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall ExternalCloseNI6509();


/************************************************************************
C# call this function to get process
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall GetProcessStep(int &setp);


/************************************************************************
C# call this function to make the NI4461 action
sampleNum : 待写入采样个数
sampleTotal ： 采样率
sleepTime ： 响应时间
声音频率：采样率 / 待写入采样个数
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoSoundOut(double volatage,int sampleNum,int sampleTotal,int sleepTime);


/************************************************************************
C# call this function to make the NI4461 action
sampleNum : 待写入采样个数
sampleTotal ： 采样率
sleepTime ： 响应时间
声音频率：采样率 / 待写入采样个数
minFreq : 起始扫频
maxFreq : 结束扫频
step    ： 间隔的扫频
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoSweepFrequency(double volatage,int sampleNum,int sampleTotal,int sleepTime,int minFreq,int maxFreq,int step);
#endif