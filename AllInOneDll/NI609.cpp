#include "StdAfx.h"
#include "NI609.h"


#pragma comment(lib,"NIDAQmx.lib")
#define		REY_TRYTIMES			3
#define		READ_TRYTIMES			100

#define		DOUBL_START_SENOR		"Dev1/port8"			//Double start
#define		GUANGSHAN_SENSOR		"Dev1/port8"			//��դ
#define		J28_SENSOR			    "Dev1/port8"			//�ж����Ƿ���
#define		J27_SENSOR			    "Dev1/port8"			//�ж����Ƿ�ر�
#define		J3_SENSOR			    "Dev1/port7"			//�ж�С�ذ��Ƿ�λ(����)
#define		J4_SENSOR			    "Dev1/port6"			//�ж�С�ذ��Ƿ�λ(����)
#define		J1_SENSOR			    "Dev1/port7"			//�жϴ��ذ��Ƿ�λ(����)
#define		J2_SENSOR			    "Dev1/port6"			//�жϴ��ذ��Ƿ�λ(����)
#define		J7_SENSOR			    "Dev1/port7"			//��DUT�ĸ�Ӧ�� 
#define		J20_SENSOR				"Dev1/port6"			//��Ƶ�Ƿ������λ   p6.3
#define		J21_SENSOR				"Dev1/port9"			//��Ƶ�Ƿ����ص�λ   p9.7
#define		J17_SENSOR				"Dev1/port9"			//���TOUCH�Ƿ����  p9.5
#define		J19_SENSOR				"Dev1/port9"			//�����Ƶ�Ƿ����   p9.6
#define		J18_SENSOR				"Dev1/port6"			//���mic�Ƿ�����    p6.2
#define		J11_SENSOR				"Dev1/port7"			//���mic�Ƿ����    p7.6
#define		J14_SENSOR				"Dev1/port6"			//����Ϲ�Դ�Ƿ���� p6.0
#define		J9_SENSOR				"Dev1/port6"			//���USB3.0�Ƿ���� p6.7
#define		J12_SENSOR				"Dev1/port9"			//����Ϲ����Ƿ���� p9.3
#define		J10_SENSOR				"Dev1/port9"			//����������Ƿ���� p9.2
#define		J5_SENSOR				"Dev1/port7"			//��ת��������λ   p7.3
#define		J6_SENSOR				"Dev1/port9"			//��ת�����½���λp  p9.0



#define		POERT_0_SENSOR			"Dev1/port0"
#define		POERT_1_SENSOR			"Dev1/port1"
#define		POERT_2_SENSOR			"Dev1/port2"
#define		POERT_3_SENSOR			"Dev1/port3"
#define		POERT_4_SENSOR			"Dev1/port4"


#define		DELAY_TIME				1000

#define		WAIT_TOO_LONG			1001		//�ȴ�ʱ��̫��
#define     SAVEFUNCTION_START		1002		


typedef enum _PORT
{
	port0	= 0,
	port1	= 1,
	port2	= 2,
	port3	= 3,
	port4	= 4,

}Port;

typedef enum _ReturnValue
{
	iReturnOK		= 0,
	InitialNIFail	= 1,
	WriteNIFail		= 2,
	ReadNIFail		= 3,
	SafetyStart		= 4,
}ReturnValue;


NI609::NI609(void)
{
	//write handle
	taskWrite_Port0 = 0;
	taskWrite_Port1 = 0;
	taskWrite_Port2 = 0;
	taskWrite_Port3 = 0;
	taskWrite_Port4 = 0;

	//read handle
	taskRead_Port6 = 0;
	taskRead_Port7 = 0;
	taskRead_Port8 = 0;
	taskRead_Port9 = 0;
	taskRead_Port10 = 0;

	//write initial
	port0_Initial = false;
	port1_Initial = false;
	port2_Initial = false;
	port3_Initial = false;
	port4_Initial = false;

	//read initials
	port6_Initial = false;
	port7_Initial = false;
	port8_Initial = false;
	port9_Initial = false;
	port10_Initial = false;

	//InitialNI();
}


NI609::~NI609(void)
{
}


int NI609::InitialNI(void)
{
	int error = DAQmxSuccess;

	//create write channel
	if (!taskWrite_Port0)
	{
		error = DAQmxCreateTask("",&taskWrite_Port0);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port0 fail");
			return error;
		}
	}

	if (!taskWrite_Port1)
	{
		error = DAQmxCreateTask("",&taskWrite_Port1);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port1 fail");
			return error;
		}
	}

	if (!taskWrite_Port2)
	{
		error = DAQmxCreateTask("",&taskWrite_Port2);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port2 fail");
			return error;
		}
	}

	if (!taskWrite_Port3)
	{
		error = DAQmxCreateTask("",&taskWrite_Port3);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port3 fail");
			return error;
		}
	}

	if (!taskWrite_Port4)
	{
		error = DAQmxCreateTask("",&taskWrite_Port4);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port4 fail");
			return error;
		}
	}


	//create read channel
	if (!taskRead_Port6)
	{
		error = DAQmxCreateTask("",&taskRead_Port6);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port6 fail");
			return error;
		}
	}

	if (!taskRead_Port7)
	{
		error = DAQmxCreateTask("",&taskRead_Port7);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port7 fail");
			return error;
		}
	}

	if (!taskRead_Port8)
	{
		error = DAQmxCreateTask("",&taskRead_Port8);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port8 fail");
			return error;
		}
	}

	if (!taskRead_Port9)
	{
		error = DAQmxCreateTask("",&taskRead_Port9);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port9 fail");
			return error;
		}
	}

	if (!taskRead_Port10)
	{
		error = DAQmxCreateTask("",&taskRead_Port10);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("create port10 fail");
			return error;
		}
	}

	//���ݻ����Ľ��飬��ʼ��������Ϊ0֮ǰ��Ҫ����Ƶ�Ĳ����ź�����Ϊ0 2016/3/7



	//reset all channel to 0
	char portBuffer[MAX_PATH] = {0};
	//reset write port
	int err = DAQmxSuccess;
	for (int i=0; i<5; i++)
	{
		sprintf(portBuffer,"Dev1/port%d",i);
		err = WriteNI6509Function(GetHandle(portBuffer),portBuffer,0);
		if (err != DAQmxSuccess)
		{
			m_saveLog.WriteLog("reset to 0 fail");
			return err;
		}
	}	
	return 0;
}

/************************************************************************
 write port:
       port0;
	   port1;
	   port2;
	   port3;
	   port4;
************************************************************************/
int NI609::WriteNI6509Function(TaskHandle TemptaskWriteHandle,char *line,unsigned short iWriteValue)
{
	int error = DAQmxSuccess;
	bool bIsInitial;
	if(!TemptaskWriteHandle)
	{
		m_saveLog.WriteLog("GetTaskHandle fail");
		return -1;
	}
	SetLineState(line,bIsInitial,iWriteValue,true);
	if(bIsInitial == false)
	{
		error = DAQmxCreateDOChan(TemptaskWriteHandle,line,"",DAQmx_Val_ChanForAllLines);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxCreateDOChan fail");
			return error;
		}

		error = DAQmxStartTask(TemptaskWriteHandle);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStartTask fail");
			return error;
		}
		SetInitialState(line);
	}

	int32 written;
	int tryTimes = 0;
	for(tryTimes =0; tryTimes<REY_TRYTIMES; tryTimes++)
	{
		error = DAQmxWriteDigitalU16(TemptaskWriteHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&iWriteValue,&written,NULL);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxWriteDigitalU16 fail");
			return error;
		}
		else
		{
			//compare the valus
			break;

		}
		Sleep(300);
	}
	if(tryTimes >= REY_TRYTIMES)
	{
		return -1;
	}
	return DAQmxSuccess;
}

/************************************************************************
 read port:
       port6;
	   port7;
	   port8;
	   port9;
	   port10;
************************************************************************/
int NI609::ReadNI6509Function(TaskHandle TemptaskReadHandle,char *line,unsigned short &iReadValue,unsigned short compareValue,bool check,int bits)
{
	int error = DAQmxSuccess;
	bool bIsInitial;
	if(!TemptaskReadHandle)
	{
		m_saveLog.WriteLog("GetTaskHandle fail");
		return -1;
	}
	SetLineState(line,bIsInitial,0);
	if(bIsInitial == false)
	{
		error = DAQmxCreateDIChan(TemptaskReadHandle,line,"",DAQmx_Val_ChanForAllLines);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxCreateDIChan fail");
			return error;
		}
		error = DAQmxStartTask(TemptaskReadHandle);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStartTask fail");
			return error;
		}
		SetInitialState(line);
	}
	int32 read;
	int tryTimes = 0;
	for(tryTimes =0; tryTimes<READ_TRYTIMES; tryTimes++)
	{
		error = DAQmxReadDigitalU16(TemptaskReadHandle,1,10.0,DAQmx_Val_GroupByChannel,&iReadValue,1,&read,NULL);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxReadDigitalU16 fail");
			return error;
		}
		else
		{	
			if (check == false)
			{
				break;
			}
			else
			{
				//compare the valus
				int tempValeu = iReadValue >> bits;
				tempValeu = tempValeu & 0x01;
				if (compareValue == tempValeu)
				{
					break;
				}
			}
		}
		Sleep(1000);
	}
	if(tryTimes >= READ_TRYTIMES)
	{
		return WAIT_TOO_LONG;
	}
	return DAQmxSuccess;
}


int NI609::CloseNI(void)
{
	int error = DAQmxSuccess;
	/************************************************************************/
	/* write                                                                */
	/************************************************************************/
	//port0
	if (taskWrite_Port0)
	{
		error = DAQmxStopTask(taskWrite_Port0);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port0 fail");
			return error;
		}
		DAQmxClearTask(taskWrite_Port0);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port0 fail");
			return error;
		}
	}
	//port1
	if (taskWrite_Port1)
	{
		error = DAQmxStopTask(taskWrite_Port1);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port1 fail");
			return error;
		}
		DAQmxClearTask(taskWrite_Port1);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port1 fail");
			return error;
		}
	}
	//port2
	if (taskWrite_Port2)
	{
		error = DAQmxStopTask(taskWrite_Port2);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port2 fail");
			return error;
		}
		DAQmxClearTask(taskWrite_Port2);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port2 fail");
			return error;
		}
	}
	//port3
	if (taskWrite_Port3)
	{
		error = DAQmxStopTask(taskWrite_Port3);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port3 fail");
			return error;
		}
		DAQmxClearTask(taskWrite_Port3);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port3 fail");
			return error;
		}
	}
	//port4
	if (taskWrite_Port4)
	{
		error = DAQmxStopTask(taskWrite_Port4);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port4 fail");
			return error;
		}
		DAQmxClearTask(taskWrite_Port4);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port4 fail");
			return error;
		}
	}
/************************************************************************/
/* read                                                                 */
/************************************************************************/
	//port6
	if (taskRead_Port6)
	{
		error = DAQmxStopTask(taskRead_Port6);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port6 fail");
			return error;
		}
		DAQmxClearTask(taskRead_Port6);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port6 fail");
			return error;
		}
	}
	//port7
	if (taskRead_Port7)
	{
		error = DAQmxStopTask(taskRead_Port7);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port7 fail");
			return error;
		}
		DAQmxClearTask(taskRead_Port7);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port7 fail");
			return error;
		}
	}
	//port8
	if (taskRead_Port8)
	{
		error = DAQmxStopTask(taskRead_Port8);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port8 fail");
			return error;
		}
		DAQmxClearTask(taskRead_Port8);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port8 fail");
			return error;
		}
	}
	//port9
	if (taskRead_Port9)
	{
		error = DAQmxStopTask(taskRead_Port9);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port9 fail");
			return error;
		}
		DAQmxClearTask(taskRead_Port9);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port9 fail");
			return error;
		}
	}
	//port10
	if (taskRead_Port10)
	{
		error = DAQmxStopTask(taskRead_Port10);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxStopTask port10 fail");
			return error;
		}
		DAQmxClearTask(taskRead_Port10);
		if (error != DAQmxSuccess)
		{
			m_saveLog.WriteLog("DAQmxClearTask port10 fail");
			return error;
		}
	}
	return DAQmxSuccess;
}


void NI609::SetLineState(char *line,bool &isFinishInitial,unsigned short writePortValue,bool isSet)
{
	//write
	if(strcmp(line,"Dev1/port0") == 0)
	{
		isFinishInitial = port0_Initial;
		if (isSet)
		{
			port0_WriteValue = writePortValue;//record the port0 write value
		}
	}
	else if(strcmp(line,"Dev1/port1") == 0)
	{
		isFinishInitial = port1_Initial;
		if (isSet)
		{
			port1_WriteValue = writePortValue;//record the port1 write value
		}
	}
	else if(strcmp(line,"Dev1/port2") == 0)
	{
		isFinishInitial = port2_Initial;
		if (isSet)
		{
			port2_WriteValue = writePortValue;//record the port2 write value
		}
	}
	else if(strcmp(line,POERT_3_SENSOR) == 0)
	{
		isFinishInitial = port3_Initial;
		if (isSet)
		{
			port3_WriteValue = writePortValue;//record the port3 write value
		}
	}
	else if(strcmp(line,"Dev1/port4") == 0)
	{
		isFinishInitial = port4_Initial;
		if (isSet)
		{
			port4_WriteValue = writePortValue;//record the port4 write value
		}
	}

	//read 
	else if(strcmp(line,"Dev1/port6") == 0)
	{
		isFinishInitial = port6_Initial;
	}
	else if(strcmp(line,"Dev1/port7") == 0)
	{
		isFinishInitial = port7_Initial;
	}
	else if(strcmp(line,"Dev1/port8") == 0)
	{
		isFinishInitial = port8_Initial;
	}
	else if(strcmp(line,"Dev1/port9") == 0)
	{
		isFinishInitial = port9_Initial;
	}
	else if(strcmp(line,"Dev1/port10") == 0)
	{
		isFinishInitial = port10_Initial;
	}
}


bool NI609::SetInitialState(char *line)
{
	//write
	if(strcmp(line,"Dev1/port0") == 0)
	{
		port0_Initial = true;
	}
	else if(strcmp(line,"Dev1/port1") == 0)
	{
		port1_Initial = true;
	}
	else if(strcmp(line,"Dev1/port2") == 0)
	{
		port2_Initial = true;
	}
	else if(strcmp(line,POERT_3_SENSOR) == 0)
	{
		port3_Initial = true;
	}
	else if(strcmp(line,"Dev1/port4") == 0)
	{
		port4_Initial = true;
	}

	//read 
	else if(strcmp(line,"Dev1/port6") == 0)
	{
		port6_Initial = true;
	}
	else if(strcmp(line,"Dev1/port7") == 0)
	{
		port7_Initial = true;
	}
	else if(strcmp(line,"Dev1/port8") == 0)
	{
		port8_Initial = true;
	}
	else if(strcmp(line,"Dev1/port9") == 0)
	{
		port9_Initial = true;
	}
	else if(strcmp(line,"Dev1/port10") == 0)
	{
		port10_Initial = true;
	}
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int NI609::SafetyFunction(void)
{
	return 0;
	/*
	//need to add the check the grating function
	int err = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;//1 or 0
	//read p8.2
	err = ReadNI6509Function(GetHandle(GUANGSHAN_SENSOR),GUANGSHAN_SENSOR,iReadValue,iCompareValue,false);
	if (err != 0)
	{
		m_saveLog.WriteLog("SafetyFunction: ReadNI6509Function fail");
		return err;
	}

	unsigned short iAdjustValue = iReadValue >> 2;
	iAdjustValue = iAdjustValue & 0x01;
	if (iAdjustValue == 0)
	{
		//if the grating has the signal, set the port3/line4,5,6,7 to 0,and other do not change
		unsigned short iWriteValue = port3_WriteValue & 0x0f;//the higher 4 bits set to 0
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("SafetyFunction: WriteNI6509Function fail");
			return err;
		}
		return SafetyStart;
	}
	return 0;
	*/
}

/************************************************************************
��Ҫ�����ŵļ��ʹ�С�ذ��Ƿ�������ļ��
************************************************************************/
int NI609::CheckDoor_Carrier(void)
{
	//1:��ȡ���ŵ�sensor,����ǹ��ŵ�״̬��ִ�п��ŵĶ���
	int err = 0;
	unsigned short iReadValue;
	unsigned short iWriteValue = 0;
	unsigned short iCompareValue = 0;
	//read door open p8.1,��λ�ź���0
	err = ReadNI6509Function(GetHandle(J27_SENSOR),J27_SENSOR,iReadValue,iCompareValue,false);
	if (err != 0)
	{
		return err;
	}

	unsigned short iAdjustValue = iReadValue >> 1;
	iAdjustValue = iAdjustValue & 0x01;
	if (iAdjustValue == 0)
	{
		//���ǹصģ�ִ�п��Ŷ��� port3/line5 ��1
		iWriteValue = port3_WriteValue | 0x20; 
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			return err;
		}
		iCompareValue = 0;//����õ���ֵ 1 or 0
		//read p8.0,��λ�ź���0
		Sleep(DELAY_TIME * 3);
		err = ReadNI6509Function(GetHandle(J28_SENSOR),J28_SENSOR,iReadValue,iCompareValue,true,0);
		if (err != 0)
		{
			return err;
		}

		//�ſ�����ϣ���P35��Ϊ0
		iWriteValue = port3_WriteValue & 0xdf; 
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			return err;
		}
	}

#if 0
	//2:���С�ذ��Ƿ������棬Ŀǰû��sensor,��ʱִ��ÿ�ζ�����
	//С�ذ����� p33��0
	iWriteValue = port3_WriteValue & 0xf7;
	err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		return err;
	}
	Sleep(DELAY_TIME*3);
#else
	//read p7.2����λ�ź���0
	err = ReadNI6509Function(GetHandle(J3_SENSOR),J3_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	iAdjustValue = iReadValue >> 2;
	iAdjustValue = iAdjustValue & 0x01;
	if(iAdjustValue == 1)
	{
		//���С�ذ������棬��ִ������С�ذ�p33,��Ϊ0
		iWriteValue = port3_WriteValue & 0xf7;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			return err;
		}

		Sleep(DELAY_TIME);
		iCompareValue = 1;//����õ���ֵ 1 or 0
		//read p6.7
		err = ReadNI6509Function(GetHandle(J4_SENSOR),J4_SENSOR,iReadValue,iCompareValue,true,7);
		if (err != 0)
		{
			return err;
		}
	}
#endif

	//�����ذ��Ƿ�������
	//read p7.1
	err = ReadNI6509Function(GetHandle(J1_SENSOR),J1_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	iAdjustValue = iReadValue >> 1;
	iAdjustValue = iAdjustValue & 0x01;
	if(iAdjustValue == 1)
	{
		//������ذ������棬��ִ������С�ذ� p36��1
		iWriteValue = port3_WriteValue | 0x40;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			return err;
		}

		Sleep(DELAY_TIME);
		iCompareValue = 1;//����õ���ֵ 1 or 0
		//read p6.6
		err = ReadNI6509Function(GetHandle(J2_SENSOR),J2_SENSOR,iReadValue,iCompareValue,true,6);
		if (err != 0)
		{
			return err;
		}
	}

	return 0;
}


unsigned short NI609::GetPortWriteValue(int port)
{
	char debug[256] = {0};
	switch(port)
	{
	case 0:
		return port0_WriteValue;
		break;
	case 1:
		return port1_WriteValue;
		break;
	case 2:
		return port2_WriteValue;
		break;
	case 3:
		return port3_WriteValue;
		break;
	case 4:
		return port4_WriteValue;
		break;
	}
	return 0;
}



TaskHandle NI609::GetHandle(char *line)
{
	//write
	if(strcmp(line,"Dev1/port0") == 0)
	{
		return taskWrite_Port0;
	}
	else if(strcmp(line,"Dev1/port1") == 0)
	{
		return taskWrite_Port1;
	}
	else if(strcmp(line,"Dev1/port2") == 0)
	{
		return taskWrite_Port2;
	}
	else if(strcmp(line,"Dev1/port3") == 0)
	{
		return taskWrite_Port3;
	}
	else if(strcmp(line,"Dev1/port4") == 0)
	{
		return taskWrite_Port4;
	}

	//read 
	else if(strcmp(line,"Dev1/port6") == 0)
	{
		return taskRead_Port6;
	}
	else if(strcmp(line,"Dev1/port7") == 0)
	{
		return taskRead_Port7;
	}
	else if(strcmp(line,"Dev1/port8") == 0)
	{
		return taskRead_Port8;
	}
	else if(strcmp(line,"Dev1/port9") == 0)
	{
		return taskRead_Port9;
	}
	else if(strcmp(line,"Dev1/port10") == 0)
	{
		return taskRead_Port10;
	}
	return 0;
}

/************************************************************************/
/* ���п��ŵĶ���                                                       */
/************************************************************************/
int NI609::OpenDoor(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	//1:�ȶ�ȡ���Ƿ��ǹرյ�״̬
	iCompareValue = 0;//1 or 0
	//P8.1
	err = ReadNI6509Function(GetHandle(J27_SENSOR),J27_SENSOR,iReadValue,iCompareValue,false);
	if (err != 0)
	{
		m_saveLog.WriteLog("open the door,read J27_SENSOR fail");
		return ReadNIFail;
	}
	unsigned short iAdjustValue = iReadValue >> 1;
	iAdjustValue = iAdjustValue & 0x01;

	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	if (iAdjustValue == iCompareValue)	//���ǹص�
	{
		//1:��ʼ������ p35
		iWriteValue = GetPortWriteValue(port3) | 0x20;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("open the door,write p35  1 fail");
			return WriteNIFail;
		}

		//��ʼ���sensor��ȷ���ſ������
		Sleep(DELAY_TIME*2);
		iCompareValue = 0;//1 or 0
		//read the door open sensor p8.0
		err = ReadNI6509Function(GetHandle(J28_SENSOR),J28_SENSOR,iReadValue,iCompareValue,true,0);
		if (err != 0)
		{
			m_saveLog.WriteLog("open the door,read J28_SENSOR fail");
			return ReadNIFail;
		}

		//�ſ�����ϣ�����p35�ĸ�λ����
		//finish to open door,and set p35 to 0
		iWriteValue = GetPortWriteValue(port3) & 0xdf;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("open the door,write p35  0 fail");
			return WriteNIFail;
		}
	}
	return 0;
}

/************************************************************************/
/* ���ذ����                                                           */
/************************************************************************/
int NI609::BigCarrierOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;

	//1:�ȼ����ذ��ǲ���������
	//p6.6����λ�ź���1
	err = ReadNI6509Function(GetHandle(J2_SENSOR),J2_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	unsigned short iAdjustValue = iReadValue >> 6;
	iAdjustValue = iAdjustValue & 0x01;
	
	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	//���ذ�������
	if (iAdjustValue == iCompareValue)
	{
		//1:���ذ��ȥ,set p34 to 1
		iWriteValue = GetPortWriteValue(port3) | 0x10; 
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier out ,write 1 fail");
			return WriteNIFail;
		}
		iCompareValue = 1;//1 or 0  
		Sleep(DELAY_TIME);

		//2:ȷ�ϴ��ذ��Ѿ�������Ķ���
		//read the big carrier sensor out : p7.1
		err = ReadNI6509Function(GetHandle(J1_SENSOR),J1_SENSOR,iReadValue,iCompareValue,true,1);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier out ,read J1_SENSOR fail");
			return ReadNIFail;
		}

		//3:���ذ��Ѿ���ȥ�ˣ���P34��Ϊ0
		iWriteValue = GetPortWriteValue(port3) & 0xef;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier out ,write 1 fail");
			return WriteNIFail;
		}
	}
	return 0;
}

/************************************************************************/
/* С�ذ����                                                           */
/************************************************************************/
int NI609::SmallCarrierOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;

	//1:�ȼ��С�ذ��ǲ���������
	//p6.7����λ�ź���1
	err = ReadNI6509Function(GetHandle(J4_SENSOR),J4_SENSOR,iReadValue,1,false);
	if (err != 0)
	{
		return err;
	}
	unsigned short iAdjustValue = iReadValue >> 7;
	iAdjustValue = iAdjustValue & 0x01;

	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	if (iAdjustValue == iCompareValue)
	{
		//1:С�ذ���� set p33 to 1
		iWriteValue = GetPortWriteValue(port3) | 0x08; 
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("small carrier out ,write 1 fail");
			return WriteNIFail;
		}
		Sleep(DELAY_TIME);

		//2:ȷ��С�ذ�����Ѿ���λ
#if 1
		iCompareValue = 1; //1 or 0
		//read the small carrier sensor out : p7.2
		err = ReadNI6509Function(GetHandle(J3_SENSOR),J3_SENSOR,iReadValue,iCompareValue,true,2);
		if (err != 0)
		{
			m_saveLog.WriteLog("wait the small carrier out fail");
			return ReadNIFail;
		}
#endif
	}
	return 0;
}

/************************************************************************/
/* ��������Ķ���                                                       */
/************************************************************************/
int NI609::Inhale(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;

	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	//������������Ķ���,p4.0 set to 1
	iWriteValue = GetPortWriteValue(port4) | 0x01; 
	err = WriteNI6509Function(GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("inhal set p4.0  fail");
		return WriteNIFail;
	}
	Sleep(DELAY_TIME);
	return 0;
}

/************************************************************************/
/* С�ذ��ȥ                                                           */
/************************************************************************/
int NI609::SmallCarrierBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;

	//1:�ȼ��С�ذ��ǲ���������
	//p7.2
	err = ReadNI6509Function(GetHandle(J3_SENSOR),J3_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	unsigned short iAdjustValue = iReadValue >> 2;
	iAdjustValue = iAdjustValue & 0x01;


	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	//С�ذ�������
	if (iAdjustValue == iCompareValue)
	{
		//1:��С�ذ��ȥ,set p33 to 0
		iWriteValue = GetPortWriteValue(port3) & 0xf7;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("small carrier back,write p33 to 0 fail");
			return WriteNIFail;
		}
		Sleep(DELAY_TIME);
#if 1
		iCompareValue = 1; //1 or 0
		//read the small carrier out p6.7
		err = ReadNI6509Function(GetHandle(J4_SENSOR),J4_SENSOR,iReadValue,iCompareValue,true,7);
		if (err != 0)
		{
			m_saveLog.WriteLog("small carrier back,read J4_SENSOR fail");
			return ReadNIFail;
		}
#endif
	}
	return 0;
}

/************************************************************************/
/* ���ذ��ȥ                                                           */
/************************************************************************/
int NI609::BigCarrierBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 1;

	//1:�ȼ����ذ��ǲ���������
	//p7.1 ��λ�ź���1
	err = ReadNI6509Function(GetHandle(J1_SENSOR),J1_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	unsigned short iAdjustValue = iReadValue >> 1;
	iAdjustValue = iAdjustValue & 0x01;


	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	//���ذ�������
	if (iAdjustValue == iCompareValue)
	{
		//1:�Ѵ��ذ��ȥ,set p36 to 1
		iWriteValue = GetPortWriteValue(port3) | 0x40;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier back,write p36 to 1 fail");
			return WriteNIFail;
		}

		iCompareValue = 1; //1 or 0
		//read big carrier back p6.6  ��λ�ź�1
		Sleep(DELAY_TIME * 2);
		err = ReadNI6509Function(GetHandle(J2_SENSOR),J2_SENSOR,iReadValue,iCompareValue,true,6);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier back,read J2_SENSOR fail");
			return ReadNIFail;
		}

		//���ذ��Ѿ���ȥ�ˣ���P36��Ϊ0
		iWriteValue = GetPortWriteValue(port3) & 0xbf;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("big carrier back ,write 0 fail");
			return WriteNIFail;
		}
	}
	return 0;
}

/************************************************************************/
/* ���Ŷ���                                                             */
/************************************************************************/
int NI609::CloseDoor(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	//1:�ȼ�����ǲ��ǹ��ŵ�
	//p8.0
	err = ReadNI6509Function(GetHandle(J28_SENSOR),J28_SENSOR,iReadValue,0,false);
	if (err != 0)
	{
		return err;
	}
	unsigned short iAdjustValue = iReadValue >> 0;
	iAdjustValue = iAdjustValue & 0x01;

	//0:���а�ȫ��դ�ļ��
	err = SafetyFunction();
	if (err != 0)
	{
		m_saveLog.WriteLog("safety function fail");
		return err;
	}

	//������ǿ��ŵ�״̬,��Ҫ���й��ŵĶ���
	if (iAdjustValue == iCompareValue)
	{
		//p37 to 1
		iWriteValue = GetPortWriteValue(port3) | 0x80;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("close the door,write 1 fail");
			return WriteNIFail;
		}

		iCompareValue = 0; //1 or 0
		Sleep(DELAY_TIME * 3);
		//read the close door p8.1
		err = ReadNI6509Function(GetHandle(J27_SENSOR),J27_SENSOR,iReadValue,iCompareValue,true,1);
		if (err != 0)
		{
			m_saveLog.WriteLog("close the door,read J2_SENSOR fail");
			return ReadNIFail;
		}
		//close the door set the p3.7 to 0
		iWriteValue = GetPortWriteValue(port3) & 0x7f;
		err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
		if (err != 0)
		{
			m_saveLog.WriteLog("close the door ,write 0 fail");
			return WriteNIFail;
		}
	}

	return 0;
}

/************************************************************************/
/* ��Ƶ�����p3.1 to 1                                                  */
/************************************************************************/
int NI609::AudioOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port3) | 0x02;
	err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}

	//p6.3
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J20_SENSOR),J20_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("audio out ,check J20_SENSOR fail");
		return err;
	}

	return 0;
}

/************************************************************************/
//��Ƶ����, p0.7 set 1
/************************************************************************/
int NI609::AudioInsert(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x80;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p9.6
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J19_SENSOR),J19_SENSOR,iReadValue,iCompareValue,true,6);
	if (err != 0)
	{
		return err;
	}
	return 0;
}

/************************************************************************/
//USB3.0��Դ���룬 p0.4 set 1
/************************************************************************/
int NI609::USBPowerInsert(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x10;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p6.7
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J9_SENSOR),J9_SENSOR,iReadValue,iCompareValue,true,7);
	if (err != 0)
	{
		return err;
	}
	return 0;
}

/************************************************************************/
//touch��� p3.0,                                                       */
/************************************************************************/
int NI609::TouchOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port3) | 0x01;
	err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p9.6
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J17_SENSOR),J17_SENSOR,iReadValue,iCompareValue,true,6);
	if (err != 0)
	{
		return err;
	}
	return 0;
}

/************************************************************************/
//touch ��Ļ  p4.1 set 1
/************************************************************************/
int NI609::TouchScreenOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port4) | 0x02;
	err = WriteNI6509Function(GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);//û��sensor����ʱ���ӳ�
#else
	iCompareValue = 1;
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J17_SENSOR),J17_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		return err;
	}
#endif
	return 0;
}

/************************************************************************/
//touch ��Ļ���� p4.1 set 0
/************************************************************************/
int NI609::TouchScreenBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port4) & 0xfd;
	err = WriteNI6509Function(GetHandle(POERT_4_SENSOR),POERT_4_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(1000);//û��sensor����ʱ���ӳ�
#else
	iCompareValue = 1;
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J17_SENSOR),J17_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		return err;
	}
#endif
	return 0;
}


/************************************************************************/
//touch ���� p3.0 set 0
/************************************************************************/
int NI609::TouchBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port3) & 0xfe;
	err = WriteNI6509Function(GetHandle(POERT_3_SENSOR),POERT_3_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);//û��sensor����ʱ���ӳ�
#else
	iCompareValue = 1;
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J17_SENSOR),J17_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		return err;
	}
#endif
	return 0;
}

/************************************************************************/
//mic���
/************************************************************************/
int NI609::MicOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x40;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p7.6
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J11_SENSOR),J11_SENSOR,iReadValue,iCompareValue,true,6);
	if (err != 0)
	{
		return err;
	}
	return 0;
}


/************************************************************************/
//mic ����
/************************************************************************/
int NI609::MicBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xbf;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p6.2
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,2);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	return 0;
}

/************************************************************************/
//�Ϲ�Դ��� p0.5 to 1
/************************************************************************/
int NI609::UpLightOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x20;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//pp6.0
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J14_SENSOR),J14_SENSOR,iReadValue,iCompareValue,true,0);
	if (err != 0)
	{
		return err;
	}
	return 0;
}


/************************************************************************/
//�Ϲ�Դ����  p0.5 to 0
/************************************************************************/
int NI609::UpLightBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xdf;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
//�Ϲ������
/************************************************************************/
int NI609::UpOpticalFiberOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x08;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p9.3
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J12_SENSOR),J12_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		return err;
	}

	return 0;
}


/************************************************************************/
//�Ϲ�������
/************************************************************************/
int NI609::UpOpticalFiberBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xf7;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
//�¹������ p0.2 to 1
/************************************************************************/
int NI609::DownOpticalFiberOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x04;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
//�¹������� p0.2 to 0
/************************************************************************/
int NI609::DownOpticalFiberBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xfb;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
/* �¹�Դp0.1 to 1                                                      */
/************************************************************************/
int NI609::DownLightOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x02;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
/* �¹�Դp0.1 to 0                                                      */
/************************************************************************/
int NI609::DownLightBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xfd;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J18_SENSOR),J18_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}


/************************************************************************/
/* ���������                                                            */
/************************************************************************/
int NI609::UpHornOut(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) | 0x01;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	//p9.2
	iCompareValue = 1;
	err = ReadNI6509Function(GetHandle(J10_SENSOR),J10_SENSOR,iReadValue,iCompareValue,true,2);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
	return 0;
}

/************************************************************************/
/* ����������                                                           */
/************************************************************************/
int NI609::UpHornBack(void)
{
	int err = 0;
	unsigned short iWriteValue = 0;
	unsigned short iReadValue = 0;
	unsigned short iCompareValue = 0;

	iWriteValue = GetPortWriteValue(port0) & 0xfe;
	err = WriteNI6509Function(GetHandle(POERT_0_SENSOR),POERT_0_SENSOR,iWriteValue);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#if 1
	Sleep(3000);
#else
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(J10_SENSOR),J10_SENSOR,iReadValue,iCompareValue,true,3);
	if (err != 0)
	{
		m_saveLog.WriteLog("check the sound,write 1 fail");
		return WriteNIFail;
	}
#endif
	return 0;
}
