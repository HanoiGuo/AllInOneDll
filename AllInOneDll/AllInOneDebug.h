#ifndef _ALLINONEDEBUG_H
#define _ALLINONEDEBUG_H
#pragma once
#include "resource.h"
#include "afxwin.h"
#include "NI609.h"

// AllInOneDebug dialog
class AllInOneDebug : public CDialogEx
{
	DECLARE_DYNAMIC(AllInOneDebug)

public:
	AllInOneDebug(CWnd* pParent = NULL);   // standard constructor
	virtual ~AllInOneDebug();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	CComboBox m_portCombox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonRead();
	bool CstringToChar(CString cstr,char *charStr);

private:
	NI609 m_ni609;
public:
	afx_msg void OnClose();
};
#endif