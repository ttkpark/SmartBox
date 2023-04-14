#pragma once


// CDataRecordDialog dialog

class CDataRecordDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDataRecordDialog)

public:
	CDataRecordDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDataRecordDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATARECORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
