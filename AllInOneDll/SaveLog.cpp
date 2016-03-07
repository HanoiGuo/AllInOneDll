#include "StdAfx.h"
#include "SaveLog.h"

using namespace std;
extern char dllFullPath[MAX_PATH];

SaveLog::SaveLog(void)
{
	fstream debug("D:\\debug.txt",ios::out);
	debug << dllFullPath << endl;

	logFile.open("D:\\debug.txt",ios::out);
	//logFile.open(dllFullPath,ios::out);
}


SaveLog::~SaveLog(void)
{
	logFile.close();
}

bool SaveLog::WriteLog(char *writeMessage)
{
	if (logFile.is_open())
	{
		logFile << writeMessage << endl;
	}
	else
	{
		return false;
	}
	return true;
}
