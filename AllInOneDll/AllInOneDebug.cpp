// AllInOneDebug.cpp : implementation file
//

#include "stdafx.h"
#include "AllInOneDll.h"
#include "AllInOneDebug.h"
#include "afxdialogex.h"
#include "resource.h"


// AllInOneDebug dialog

IMPLEMENT_DYNAMIC(AllInOneDebug, CDialogEx)

AllInOneDebug::AllInOneDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(AllInOneDebug::IDD, pParent)
{

}

AllInOneDebug::~AllInOneDebug()
{
}

void AllInOneDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT_COMBOX, m_portCombox);
}


BEGIN_MESSAGE_MAP(AllInOneDebug, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &AllInOneDebug::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_READ, &AllInOneDebug::OnBnClickedButtonRead)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// AllInOneDebug message handlers



BOOL AllInOneDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString csPortName = L"";
	/*
	for (int port=0; port<12; port++)
	{
		for (int line=0; line<8; line++)
		{
			csPortName.Format(L"Dev1/port%d/line%d",port,line);
			m_portCombox.AddString(csPortName);
		}
	}
	*/
	for (int port=0; port<12; port++)
	{
		csPortName.Format(L"Dev1/port%d",port);
		m_portCombox.AddString(csPortName);
	}
	m_portCombox.SetCurSel(0);

	m_ni609.InitialNI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AllInOneDebug::OnBnClickedButtonWrite()
{
	// TODO: Add your control notification handler code here
	//1:get the chanel
	int err = 0;
	CString csChanel;
	int iIndex = m_portCombox.GetCurSel();
	if (iIndex < 0)
	{
		AfxMessageBox(L"需要选择一个Channel");
		return;
	}
	m_portCombox.GetLBText(iIndex,csChanel);
	char channelBuffer[MAX_PATH] = {0};
	CstringToChar(csChanel,channelBuffer);


	CString csWrite;
	GetDlgItemText(IDC_EDIT_WRITE,csWrite);
	unsigned short writeData = 0;
	writeData = _ttoi(csWrite);
	if (writeData > 255)
	{
		writeData = 255;
	}
	err = m_ni609.WriteNI6509Function(m_ni609.GetHandle(channelBuffer),channelBuffer,writeData);
	if(err != 0)
	{
		AfxMessageBox(L"Write DAQ Fail");
	}
}


void AllInOneDebug::OnBnClickedButtonRead()
{
	// TODO: Add your control notification handler code here
	//1:get the chanel
	CString csChanel;
	int err = 0;
	int iIndex = m_portCombox.GetCurSel();
	if (iIndex < 0)
	{
		AfxMessageBox(L"需要选择一个Channel");
		return;
	}
	m_portCombox.GetLBText(iIndex,csChanel);
	char channelBuffer[MAX_PATH] = {0};
	CstringToChar(csChanel,channelBuffer);

	unsigned short readData;
	err = m_ni609.ReadNI6509Function(m_ni609.GetHandle(channelBuffer),channelBuffer,readData,1,FALSE);
	if(err != 0)
	{
		AfxMessageBox(L"Write DAQ Fail");
	}
	CString csRead;
	csRead.Format(L"%d",readData);
	GetDlgItem(IDC_EDIT_READ)->SetWindowText(csRead);
}

bool AllInOneDebug::CstringToChar(CString cstr,char *charStr)
{
	int iSize = (int)cstr.GetLength();
	if (iSize < 1)
	{
		charStr[0] = '\0';
		return true;
	}
	for (int i=0; i<iSize; i++)
	{
		charStr[i] = cstr[i];
	}
	charStr[iSize] = '\0';
	return true;  
}


void AllInOneDebug::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_ni609.CloseNI();
	CDialogEx::OnClose();
}
