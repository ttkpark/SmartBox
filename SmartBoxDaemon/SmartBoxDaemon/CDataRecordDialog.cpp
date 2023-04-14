// CDataRecordDialog.cpp : implementation file
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "CDataRecordDialog.h"
#include "afxdialogex.h"


// CDataRecordDialog dialog

IMPLEMENT_DYNAMIC(CDataRecordDialog, CDialogEx)

CDataRecordDialog::CDataRecordDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATARECORD, pParent)
{

}

CDataRecordDialog::~CDataRecordDialog()
{
}

void CDataRecordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataRecordDialog, CDialogEx)
END_MESSAGE_MAP()


// CDataRecordDialog message handlers
