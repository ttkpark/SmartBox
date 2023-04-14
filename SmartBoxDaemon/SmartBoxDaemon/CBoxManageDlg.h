#pragma once

class CSmartBoxDaemonDlg;
// CBoxManageDlg dialog
struct SensorDataList;

class CBoxManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBoxManageDlg)

public:
	CBoxManageDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CBoxManageDlg();
	CSmartBoxDaemonDlg* p_parent;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOXMANAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CtrlidxGoods;
	CEdit m_Ctrlsavetotmin;
	CEdit m_Ctrlwarningmin;
	CEdit m_Ctrldangermin;
	CEdit m_CtrlRacno;
	CEdit m_Ctrlstartdate;
	CEdit m_Ctrlequipsn;
	afx_msg void OnCbnSelchangeEditgoods();
	afx_msg void OnEnChangeEditsavetot();
	afx_msg void OnEnChangeEditwarnmin();
	afx_msg void OnEnChangeEditdangermin();
	afx_msg void OnEnChangeEditracn();
	afx_msg void OnEnChangeEditstadate();
	afx_msg void OnEnChangeEditsn();
};
