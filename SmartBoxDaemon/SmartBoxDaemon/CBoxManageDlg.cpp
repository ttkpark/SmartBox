// CBoxManageDlg.cpp : implementation file
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "SmartBoxDaemonDlg.h"
#include "CBoxManageDlg.h"
#include "afxdialogex.h"


// CBoxManageDlg dialog

IMPLEMENT_DYNAMIC(CBoxManageDlg, CDialogEx)

CBoxManageDlg::CBoxManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOXMANAGE, pParent)
{
	p_parent = (CSmartBoxDaemonDlg*)pParent;
}

CBoxManageDlg::~CBoxManageDlg()
{
}

void CBoxManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGOODS, m_CtrlidxGoods);
	DDX_Control(pDX, IDC_EDITSAVETOT, m_Ctrlsavetotmin);
	DDX_Control(pDX, IDC_EDITWARNMIN, m_Ctrlwarningmin);
	DDX_Control(pDX, IDC_EDITDANGERMIN, m_Ctrldangermin);
	DDX_Control(pDX, IDC_EDITRACN, m_CtrlRacno);
	DDX_Control(pDX, IDC_EDITSTADATE, m_Ctrlstartdate);
	DDX_Control(pDX, IDC_EDITSN, m_Ctrlequipsn);
}


BEGIN_MESSAGE_MAP(CBoxManageDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_EDITGOODS, &CBoxManageDlg::OnCbnSelchangeEditgoods)
	ON_EN_CHANGE(IDC_EDITSAVETOT, &CBoxManageDlg::OnEnChangeEditsavetot)
	ON_EN_CHANGE(IDC_EDITWARNMIN, &CBoxManageDlg::OnEnChangeEditwarnmin)
	ON_EN_CHANGE(IDC_EDITDANGERMIN, &CBoxManageDlg::OnEnChangeEditdangermin)
	ON_EN_CHANGE(IDC_EDITRACN, &CBoxManageDlg::OnEnChangeEditracn)
	ON_EN_CHANGE(IDC_EDITSTADATE, &CBoxManageDlg::OnEnChangeEditstadate)
	ON_EN_CHANGE(IDC_EDITSN, &CBoxManageDlg::OnEnChangeEditsn)
END_MESSAGE_MAP()


// CBoxManageDlg message handlers


void CBoxManageDlg::OnCbnSelchangeEditgoods()
{
	// TODO: Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditsavetot()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditwarnmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditdangermin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditracn()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditstadate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBoxManageDlg::OnEnChangeEditsn()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
