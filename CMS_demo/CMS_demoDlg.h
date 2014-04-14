
// CMS_demoDlg.h : 头文件
//

#pragma once


// CCMS_demoDlg 对话框
class CCMS_demoDlg : public CDialogEx
{
// 构造
public:
	CCMS_demoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CMS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
public:
		LPBITMAPINFO s_BmpInfo;
		HANDLE hloc;
public:
	void ShowPic(char * pData);
	void StarTest();
	int start_ipcam_stream(int avIndex);

	CStatic	m_BK;
	char*   m_TempData;		//临时数据指针
	afx_msg void OnBnClickedButton3();
};
