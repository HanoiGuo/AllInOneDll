#ifndef _SAVELOG_H
#define _SAVELOG_H
#pragma once
#include <fstream>
using namespace std;

class SaveLog
{
public:
	SaveLog(void);
	~SaveLog(void);
private:
	fstream logFile;
public:
	bool WriteLog(char *writeMessage);
};
#endif

