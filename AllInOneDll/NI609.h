#ifndef _NI6509_H
#define _NI6509_H
#pragma once
#include "NIDAQmx.h"
#include "SaveLog.h"

class NI609
{
public:
	NI609(void);
	~NI609(void);
public:
	int InitialNI(void);
	int WriteNI6509Function(TaskHandle TemptaskWriteHandle,char *line,unsigned short iWriteValue);
	int ReadNI6509Function(TaskHandle TemptaskReadHandle,char *line,unsigned short &iReadValue,unsigned short compareValue,bool check,int bits=0);
	int CloseNI(void);
	bool SetInitialState(char *line);
	int SafetyFunction(void);
	int CheckDoor_Carrier(void);
	void SetLineState(char *line,bool &isFinishInitial,unsigned short writePortValue,bool isSet=false);
	unsigned short GetPortWriteValue(int port);
private:
	SaveLog m_saveLog;
	//port0~port4  write
	TaskHandle  taskWrite_Port0;
	TaskHandle  taskWrite_Port1;
	TaskHandle  taskWrite_Port2;
	TaskHandle  taskWrite_Port3;
	TaskHandle  taskWrite_Port4;

	//port6~port10 read
	TaskHandle  taskRead_Port6;
	TaskHandle  taskRead_Port7;
	TaskHandle  taskRead_Port8;
	TaskHandle  taskRead_Port9;
	TaskHandle  taskRead_Port10;

	//port0~port4  write
	bool port0_Initial;
	bool port1_Initial;
	bool port2_Initial;
	bool port3_Initial;
	bool port4_Initial;

	//port6~port10  read
	bool port6_Initial;
	bool port7_Initial;
	bool port8_Initial;
	bool port9_Initial;
	bool port10_Initial;


	//port0~port 4 write;
	unsigned short port0_WriteValue;
	unsigned short port1_WriteValue;
	unsigned short port2_WriteValue;
	unsigned short port3_WriteValue;
	unsigned short port4_WriteValue;

public:
	TaskHandle GetHandle(char *line);
	int OpenDoor(void);
	int BigCarrierOut(void);
	int SmallCarrierOut(void);
	int Inhale(void);
	int SmallCarrierBack(void);
	int BigCarrierBack(void);
	int CloseDoor(void);
	int AudioOut(void);
	int AudioInsert(void);
	int USBPowerInsert(void);
	int TouchOut(void);
	int TouchScreenOut(void);
	int TouchScreenBack(void);
	int TouchBack(void);
	int MicOut(void);
	int MicBack(void);
	int UpLightOut(void);
	int UpLightBack(void);
	int UpOpticalFiberOut(void);
	int UpOpticalFiberBack(void);
	int DownOpticalFiberOut(void);
	int DownOpticalFiberBack(void);
	int DownLightOut(void);
	int DownLightBack(void);
	int UpHornOut(void);
	int UpHornBack(void);
};
#endif

