#pragma once
class CSmartBoxMasterKeyAppDlg;

// CLoginDlg dialog


class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLoginDlg();

	CSmartBoxMasterKeyAppDlg* p_parent;
	CString idxClient;
	CString work_zone_num;
	CString ID;
	CString PW;

	CString Token;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CEdit m_EditID;
	CEdit m_EditPW;
	int GETAPILGN();
	virtual void OnOK();
	int Pingtoserver();
	CString GetCaption(int id);
};
