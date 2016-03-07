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
#endif