#include "stdafx.h"
#include "Entrance.h"
#include "AllInOneDebug.h"
#include "NI609.h"
#include "Audio.h"
#include <stdio.h>
#include <Windows.h>
#include "SaveLog.h"


/************************************************************************/
/* declare the global variable                                          */
/************************************************************************/
char dllFullPath[MAX_PATH];
NI609 m_ni609;
Audio m_ni4461;
static int iGlobalStep = 0;

typedef enum _ReturnValue
{
	iReturnOK		= 0,
	InitialNIFail	= 1,
	WriteNIFail		= 2,
	ReadNIFail		= 3,
}ReturnValue;

typedef enum _STEP
{
	InitialNI			= 0,
	Check_Door_Carrier	= 1,
	Open_Door			= 2,
	Big_Carrier_Out		= 3,
	Small_Carrier_Out	= 4,
	Wait_Put_Dut		= 5,
	Inhale				= 6,
	Wait_Press_Start	= 7,
	Small_Carrier_Back	= 8,
	Big_Carrier_Back	= 9,
	Close_Door			= 10,
	Audio_Out			= 11,
}Step;

typedef enum _PORT
{
	port0	= 0,
	port1	= 1,
	port2	= 2,
	port3	= 3,
	port4	= 4,

}Port;

#define		DOUBL_START_SENOR		"Dev1/port8"			//Double start
#define		GUANGSHAN_SENSOR		"Dev1/port8"			//光栅
#define		J28_SENSOR			    "Dev1/port8"			//判断门是否开启
#define		J27_SENSOR			    "Dev1/port8"			//判断门是否关闭
#define		J3_SENSOR			    "Dev1/port7"			//判断小载板是否到位(外面)
#define		J4_SENSOR			    "Dev1/port6"			//判断小载板是否到位(里面)
#define		J1_SENSOR			    "Dev1/port7"			//判断大载板是否到位(外面)
#define		J2_SENSOR			    "Dev1/port6"			//判断大载板是否到位(里面)
#define		J7_SENSOR			    "Dev1/port7"			//放DUT的感应器 
#define		J20_SENSOR				"Dev1/port6"			//音频是否伸出到位   p6.3
#define		J21_SENSOR				"Dev1/port9"			//音频是否缩回到位   p9.7
#define		J17_SENSOR				"Dev1/port9"			//检查TOUCH是否伸出  p9.5
#define		J19_SENSOR				"Dev1/port9"			//检查音频是否插入   p9.6
#define		J18_SENSOR				"Dev1/port6"			//检查mic是否缩回    p6.2
#define		J11_SENSOR				"Dev1/port7"			//检查mic是否伸出    p7.6
#define		J14_SENSOR				"Dev1/port6"			//检查上光源是否伸出 p6.0
#define		J9_SENSOR				"Dev1/port6"			//检查USB3.0是否插入 p6.7
#define		J12_SENSOR				"Dev1/port9"			//检查上光纤是否伸出 p9.3
#define		J10_SENSOR				"Dev1/port9"			//检查上喇叭是否伸出 p9.2
#define		J5_SENSOR				"Dev1/port7"			//翻转气缸升起到位   p7.3
#define		J6_SENSOR				"Dev1/port9"			//翻转气缸下降到位p  p9.0



#define		POERT_0_SENSOR			"Dev1/port0"
#define		POERT_1_SENSOR			"Dev1/port1"
#define		POERT_2_SENSOR			"Dev1/port2"
#define		POERT_3_SENSOR			"Dev1/port3"
#define		POERT_4_SENSOR			"Dev1/port4"

#define		WAIT_TOO_LONG			1001		//等待时间太长
/************************************************************************/
/* conver tchar to char                                                 */
/************************************************************************/
void ConvertLPWSTRToLPSTR(TCHAR *_tchar,char *_char)
{
	if (!_tchar)
	{
		return;
	}
	int iLength = 0;
	//get the tchar length
	iLength = WideCharToMultiByte(CP_ACP,0,_tchar,-1,NULL,0,NULL,NULL);
	//convert thcar to char
	WideCharToMultiByte(CP_ACP,0,_tchar,-1,_char,iLength,NULL,NULL);
}
/************************************************************************
C# call this function to close NI6509
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall ExternalCloseNI6509()
{
	int err = 0;
	err = m_ni609.CloseNI();
	if (err != 0)
	{
		return err;
	}
	return 0;
}

/************************************************************************
Show debug dialog
************************************************************************/
extern "C" __declspec(dllexport) void __stdcall ShowDlg()
{
	//添加AFX_MANAGE_STATE 进行DLL模块状态资源的切换
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR tempDllFullPath[256] = {0};
	GetModuleFileName(NULL,tempDllFullPath,MAX_PATH);
	//Tchar ----> char 
	int iLen = sizeof(tempDllFullPath);
	for(int i=iLen; i>=0; i--)
	{
		if(tempDllFullPath[i] == '\\')
		{
			tempDllFullPath[i+1] = '\0'; 
			break;
		}
	}
	ConvertLPWSTRToLPSTR(tempDllFullPath,dllFullPath);
	AllInOneDebug dialog;
	dialog.DoModal();
}

extern "C" __declspec(dllexport) int __stdcall GetProcessStep(int &setp)
{
	setp = iGlobalStep;
	return 0;
}

/************************************************************************
C# call this function to auto fun
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoRunFunction()
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;
	SaveLog m_saveLog;

	//1: initial the NI DAQ card
	iGlobalStep = InitialNI;
	err = m_ni609.InitialNI();
	if (err != 0)
	{
		m_saveLog.WriteLog("Initial NI fail");
		return InitialNIFail;
	}
	

	//2:check the door
	iGlobalStep = Check_Door_Carrier;
	err = m_ni609.CheckDoor_Carrier();
	if (err != 0)
	{
		m_saveLog.WriteLog("Check door fail");
		return WriteNIFail;
	}
	
	
	//3:进行开门动作
	iGlobalStep = Open_Door;
	err =m_ni609.OpenDoor();
	if (err != 0)
	{
		m_saveLog.WriteLog("Check door fail");
		return err;
	}
	

	//4:进行大载板出去的动作
	iGlobalStep = Big_Carrier_Out;
	err = m_ni609.BigCarrierOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("big carrier out fail");
		return err;
	}
	

	//5:小载板出去的动作
	iGlobalStep = Small_Carrier_Out;
	err = m_ni609.SmallCarrierOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("small carrier out fail");
		return err;
	}
	

	//6:等待放DUT的sensor
	iCompareValue = 1; //1 or 0
	iGlobalStep = Wait_Put_Dut;
	while(1)
	{
		//read DUT sensor p7.4
		err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J7_SENSOR),J7_SENSOR,iReadValue,iCompareValue,true,4);
		if (err == 0)
		{
			break;
		}
		if (err != 0 && err != WAIT_TOO_LONG)
		{
			m_saveLog.WriteLog("wait put the DUT fail");
			return ReadNIFail;
		}
		else if (err == WAIT_TOO_LONG)
		{
			Sleep(1000);
			continue;
		}
		Sleep(1000);
	}


	//7:进行真空吸气的动作
	iGlobalStep = Inhale;
	err = m_ni609.Inhale();	
	if (err != 0)
	{
		m_saveLog.WriteLog("inhale fail");
		return err;
	}
	

	//7:等待按双启动
	iCompareValue = 1; //1 or 0
	iGlobalStep = Wait_Press_Start;
	while(1)
	{
		//read the double start p8.3
		err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(DOUBL_START_SENOR),DOUBL_START_SENOR,iReadValue,iCompareValue,true,3);
		if (err == 0)
		{
			break;
		}
		if (err != 0 && err != WAIT_TOO_LONG)
		{
			m_saveLog.WriteLog("wait clcik start button fail");
			return ReadNIFail;
		}
		else if (err == WAIT_TOO_LONG)
		{
			Sleep(1000);
			continue;
		}
		Sleep(1000);
	}

	//8:让小载板回去
	iGlobalStep = Small_Carrier_Back;
	err = m_ni609.SmallCarrierBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("small carrier back fail");
		return err;
	}
	

	//9:让大载板回去
	iGlobalStep = Big_Carrier_Back;
	err = m_ni609.BigCarrierBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("big carrier back fail");
		return err;
	}
	
	//10:关门的动作
	iGlobalStep = Close_Door;
	err = m_ni609.CloseDoor();
	if (err != 0)
	{
		m_saveLog.WriteLog("close door fail");
		return err;
	}

	//音频伸出，p3.1 to 1 
	iGlobalStep = Audio_Out; 
	err = m_ni609.AudioOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("Audio out fail");
		return err;
	}

	//音频插入, p0.7 set 1
	err = m_ni609.AudioInsert();
	if (err != 0)
	{
		m_saveLog.WriteLog("Audio inset fail");
		return err;
	}


	//USB3.0电源插入， p0.4 set 1
	err = m_ni609.USBPowerInsert();
	if (err != 0)
	{
		m_saveLog.WriteLog("USB2.0 power insert");
		return err;
	}

	//touch伸出 p3.0,
	err = m_ni609.TouchOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("Touch out");
		return err;
	}


	//touch 屏幕  p4.1 set 1
	err = m_ni609.TouchScreenOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("Touch screen out");
		return err;
	}


	//touch 屏幕返回 p4.1 set 0
	err = m_ni609.TouchScreenBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("Touch screen back");
		return err;
	}


	//touch 缩回 p3.0 set 0
	err = m_ni609.TouchBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("Touch back");
		return err;
	}

	//mic伸出  p0.6 set 1  需要确认第几个bit?
	err = m_ni609.MicOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("Mic out");
		return err;
	}


	//mic 缩回  p0.6 set to 0
	err = m_ni609.MicBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("Mic back");
		return err;
	}


	//上光源伸出 p0.5 to 1
	err = m_ni609.UpLightOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("up light out");
		return err;
	}


	//上光源缩回  p0.5 to 0
	err = m_ni609.UpLightBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("up light back");
		return err;
	}


	//上光纤伸出 p0.3 to 1
	err = m_ni609.UpOpticalFiberOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("up optical filber out");
		return err;
	}


	//上光纤缩回  p0.3 to 0
	err = m_ni609.UpOpticalFiberBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("up optical filber back");
		return err;
	}


	//下光纤伸出 p0.2 to 1
	err = m_ni609.DownOpticalFiberOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("down optical fiber out");
		return err;
	}

	//下光纤缩回 p0.2 to 0
	err = m_ni609.DownOpticalFiberBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("down optical fiber back");
		return err;
	}


	//下光源p0.1 to 1
	err = m_ni609.DownLightOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("down light out");
		return err;
	}

	//下光源p0.1 to 0
	err = m_ni609.DownLightBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("down light back");
		return err;
	}


	//上喇叭伸出 p0.0 to 1
	err = m_ni609.UpHornOut();
	if (err != 0)
	{
		m_saveLog.WriteLog("up horn out");
		return err;
	}


	//上喇叭伸出 p0.0 to 0
	err = m_ni609.UpHornBack();
	if (err != 0)
	{
		m_saveLog.WriteLog("up horn back");
		return err;
	}

	//开关按键 p4.2 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port4) | 0x04;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(2000);

	//开关按键 p4.2 to 0
	iWriteValue = m_ni609.GetPortWriteValue(port4) & 0xfb;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(2000);



	//音量加  p4.3 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port4) | 0x08;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	} 
	Sleep(2000);
	iWriteValue = m_ni609.GetPortWriteValue(port4) & 0xf7;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	} 
	Sleep(2000);




	//音量减  p4.4 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port4) | 0x10;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	} 
	Sleep(2000);
	iWriteValue = m_ni609.GetPortWriteValue(port4) & 0xef;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	} 
	Sleep(2000);


	//p0.7 to 0 音频插入缩回
	iWriteValue = m_ni609.GetPortWriteValue(port0) & 0x7f;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(2000);

	//p3.1 to 0 音频模组缩回
	iWriteValue = m_ni609.GetPortWriteValue(port3) & 0xfd;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(2000);


	//翻转气缸升起 p3.2 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port3) | 0x04;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}

	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J5_SENSOR),J5_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//翻转气缸下降 p3.2 to 0
	iWriteValue = m_ni609.GetPortWriteValue(port3) & 0xfb;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p9.0
	iCompareValue = 1;
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J6_SENSOR),J6_SENSOR,iReadValue,iCompareValue,true,0);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}

	//USB3.0电源缩回 p0.4 to 0
	iWriteValue = m_ni609.GetPortWriteValue(port0) & 0xef;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(3000);


	//p4.0  set to 0 真空关闭
	iWriteValue = m_ni609.GetPortWriteValue(port4) & 0xfe;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	Sleep(3000);

	/*
	err = OutFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("out function fail");
		return err;
	}
	*/
	return 0;
}

int OutFunction()
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;
	SaveLog m_saveLog;

	//3:open the door,set p35 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port3) | 0x20;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("open the door,write p35  1 fail");
		return WriteNIFail;
	}
	iCompareValue = 0;//1 or 0
	//read the door open sensor p8.0
	Sleep(5000);
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J28_SENSOR),J28_SENSOR,iReadValue,iCompareValue,true,0);
	if (err != 0)
	{
		m_saveLog.WriteLog("open the door,read J28_SENSOR fail");
		return ReadNIFail;
	}
	//finish to open door,and set p35 to 0
	iWriteValue = m_ni609.GetPortWriteValue(port3) & 0xdf;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("open the door,write p35  0 fail");
		return WriteNIFail;
	}



	//4:大载板出去,set p34 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port3) | 0x10; 
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("big carrier out ,write 1 fail");
		return WriteNIFail;
	}
	iCompareValue = 1;//1 or 0  
	Sleep(3000);
	//read the big carrier sensor out : p7.1
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J1_SENSOR),J1_SENSOR,iReadValue,iCompareValue,true,1);
	if (err != 0)
	{
		m_saveLog.WriteLog("big carrier out ,read J1_SENSOR fail");
		return ReadNIFail;
	}
	//大载板已经出去了，把P34置为0
	iWriteValue = m_ni609.GetPortWriteValue(port3) & 0xef;
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("big carrier out ,write 1 fail");
		return WriteNIFail;
	}

	//5:小载板出来 set p33 to 1
	iWriteValue = m_ni609.GetPortWriteValue(port3) | 0x08; 
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("small carrier out ,write 1 fail");
		return WriteNIFail;
	}
	return 0;
}

/************************************************************************
C# call this function to make the NI4461 action
sampleNum : 待写入采样个数
sampleTotal ： 采样率
sleepTime ： 响应时间
声音频率：采样率 / 待写入采样个数
************************************************************************/
extern "C" __declspec(dllexport) int __stdcall AutoSoundOut(double volatage,int sampleNum,int sampleTotal,int sleepTime)
{
	int err = 0;
	err = m_ni4461.InitialNI4461(volatage,sampleTotal);
	if (err != 0)
	{
		return err;
	}

	err = m_ni4461.SountOut(sampleNum,sleepTime,sampleTotal);
	if (err != 0)
	{
		return err;
	}
	return 0;
}


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
extern "C" __declspec(dllexport) int __stdcall AutoSweepFrequency(double volatage,int sampleNum,int sampleTotal,int sleepTime,int minFreq,int maxFreq,int step)
{
	int err = 0;
	err = m_ni4461.InitialNI4461(volatage,sampleTotal);
	if (err != 0)
	{
		return err;
	}
	
	err = m_ni4461.SweepFrequency(sampleTotal,sleepTime,minFreq,maxFreq,step);
	if (err != 0)
	{
		return err;
	}
	return 0;
}