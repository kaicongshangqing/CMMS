
// CMS_demoDlg.h : ͷ�ļ�
//

#pragma once


// CCMS_demoDlg �Ի���
class CCMS_demoDlg : public CDialogEx
{
// ����
public:
	CCMS_demoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CMS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	char*   m_TempData;		//��ʱ����ָ��
	afx_msg void OnBnClickedButton3();
};
