#pragma once
#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"

// CTab3 dialog

class CTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab3)

public:
	CTab3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab3();
	CSmartBoxMasterKeyAppDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// ��ü ��ġ
	CEdit m_Edit30;
	// ��ü BOX NO
	CEdit m_Edit31;
	// ��ü ��ǰ��
	CEdit m_Edit32;
	// ��ü �����ð�
	CEdit m_Edit33;
	// ��ü ���ð�
	CEdit m_Edit34;
	// ��ü ����ð�
	CEdit m_Edit35;
	// ��ü RAC ��з�
	CEdit m_Edit37;
	// ��ü RAC �ߺз�
	CEdit m_Edit38;
	// ��ü RAC �Һз�
	CEdit m_Edit39;
	// ��ü�� ����
	CEdit m_Edit3A;
	// ��ü ��ư
	CButton m_Button3A;
	afx_msg void OnBnClickedButton3a();
	virtual BOOL OnInitDialog();
	int ValUpdate();
	int ReplaceDBClicked();
	void ChangeCaption();
	CString GetCaption(int id);
};
