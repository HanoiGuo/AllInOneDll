#ifndef _ENTRANCE_H
#define _ENTRANCE_H
#pragma once 
#include "StdAfx.h"


/************************************************************************
�����豸�Ļ�����������
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
sampleNum : ��д���������
sampleTotal �� ������
sleepTime �� ��Ӧʱ��
����Ƶ�ʣ������� / ��д���������
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoSoundOut(double volatage,int sampleNum,int sampleTotal,int sleepTime);


/************************************************************************
C# call this function to make the NI4461 action
sampleNum : ��д���������
sampleTotal �� ������
sleepTime �� ��Ӧʱ��
����Ƶ�ʣ������� / ��д���������
minFreq : ��ʼɨƵ
maxFreq : ����ɨƵ
step    �� �����ɨƵ
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoSweepFrequency(double volatage,int sampleNum,int sampleTotal,int sleepTime,int minFreq,int maxFreq,int step);
#endif