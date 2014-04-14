
// CMS_demoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS_demo.h"
#include "CMS_demoDlg.h"
#include "afxdialogex.h"
#include "IOTCAPIs.h"
#include "AVAPIs.h"
#include "AVFRAMEINFO.h" //2
#include "AVIOCTRLDEFs.h" //3
#include "CommonUtility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define VIDEO_BUF_SIZE	99999
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCMS_demoDlg �Ի���


DWORD WINAPI thread_ReceiveVideo(VOID *arg);

CCMS_demoDlg::CCMS_demoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMS_demoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	hloc = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE,
		sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256));
	s_BmpInfo = (LPBITMAPINFO) GlobalLock(hloc);
}

void CCMS_demoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEST_BK, m_BK);
}

BEGIN_MESSAGE_MAP(CCMS_demoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCMS_demoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CCMS_demoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCMS_demoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCMS_demoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCMS_demoDlg ��Ϣ�������

BOOL CCMS_demoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	int i;
	HANDLE hloc1;
	RGBQUAD *argbq;

	hloc1 = LocalAlloc(LMEM_ZEROINIT | LMEM_MOVEABLE,(sizeof(RGBQUAD) * 256));
	argbq = (RGBQUAD *) LocalLock(hloc1);

	for(i = 0; i < 256; i ++)
	{
		argbq[i].rgbBlue = i;
		argbq[i].rgbGreen = i;
		argbq[i].rgbRed = i;
		argbq[i].rgbReserved = 0;
	}

	

	LocalUnlock(hloc1);
	LocalFree(hloc1);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCMS_demoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCMS_demoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCMS_demoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCMS_demoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CCMS_demoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StarTest();
}


void CCMS_demoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCMS_demoDlg::ShowPic(char * pData)
{
	BITMAPINFO bInfo;		
	memcpy(&bInfo,pData,sizeof(bInfo));
	int x  = bInfo.bmiHeader.biWidth;
	int y  = bInfo.bmiHeader.biHeight;
	StretchDIBits(m_BK.GetDC()->m_hDC,0,0,x,y,0,0,x,y,&pData[sizeof(BITMAPINFO)],&bInfo,DIB_RGB_COLORS,SRCCOPY);
}
void CCMS_demoDlg::StarTest()
{
	int ret;
	//1	d1_DEK	DEKTEMMYCGU7BM6MW7YJ	 C6P9FMR6DPU7BMPMSRZJ F6U9FGP68KUFVG6MS7DJ	TUTK_Test	FTMSBG5Y1JL7AG6WG76J
	char* UID_test="ENG9T5RPJSFLVGPKU6W1";

	CCommonUtility::WriteDataIntoFile("StreamClient start");

	// use which Master base on location, port 0 means to get a random port
	ret = IOTC_Initialize(0, "61.188.37.216", "50.19.254.134", "m4.iotcplatform.com", "m2.iotcplatform.com");
	CString strret;
	strret.Format("%d",ret);
	CCommonUtility::WriteDataIntoFile(strret);
	if(ret != IOTC_ER_NoERROR) 
	{
		CCommonUtility::WriteDataIntoFile("IOTCAPIs_Device exit...!!");
		return ;
	}


	// alloc 3 sessions for video and two-way audio
	avInitialize(8);
	// use IOTC_Connect_ByUID or IOTC_Connect_ByName to connect with device
	int gSID = IOTC_Connect_ByUID(UID_test);// connect
	printf("Step 2: call IOTC_Connect_ByUID(%s) ret(%d).......\n", UID_test, gSID);
	struct st_SInfo Sinfo;	
	IOTC_Session_Check(gSID, &Sinfo);//get info

	if(Sinfo.Mode == 0)
		printf("Device is from %s:%d[%s] Mode=P2P\n",Sinfo.RemoteIP, Sinfo.RemotePort, Sinfo.UID);

	else if(Sinfo.Mode == 1)
		printf("Device is from %s:%d[%s] Mode=RLY\n",Sinfo.RemoteIP, Sinfo.RemotePort, Sinfo.UID);
	else if(Sinfo.Mode == 2)
		printf("Device is from %s:%d[%s] Mode=LAN\n",Sinfo.RemoteIP, Sinfo.RemotePort, Sinfo.UID);
	printf("Device info VER[%X] NAT[%d]\n", Sinfo.IOTCVersion, Sinfo.NatType);
	//printf("My NAT Type[%d]\n", IOTC_Get_Nat_Type());
	//	printf("Cost time = %ld ms\n", GetTickCount() - t1);

	unsigned long srvType;
	int avIndex = avClientStart(gSID, "admin", "123456", 20000, &srvType, 0);// may need change. 
	printf("Step 2: call avClientStart OK(%d).......\n", avIndex);	
	if(avIndex < 0)
	{
		printf("avClientStart failed[%d]\n", avIndex);
		return ;
	}

	if(start_ipcam_stream(avIndex))
	{
		DWORD ThreadVideo_ID;
		//, ThreadAudio_ID;

		HANDLE hThread1 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)thread_ReceiveVideo, (LPVOID)&avIndex, 0, &ThreadVideo_ID);
		if(NULL == ThreadVideo_ID || hThread1 == NULL)
		{
			printf("Failed to create thread1!!!!\n");
			return ;
		}

		/*
		HANDLE hThread2 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)thread_ReceiveAudio, (LPVOID)&avIndex, 0, &ThreadAudio_ID);
		if(NULL == ThreadAudio_ID || hThread2 == NULL)
		{
		printf("Failed to create thread2!!!!\n");
		return 0;

		}
		*/
		//Sleep(1000000000);
#if 1
		WaitForSingleObject(hThread1, INFINITE);
		//WaitForSingleObject(hThread2, INFINITE);
		//WaitForSingleObject(hThread3, INFINITE);
#endif
	}

	//CloseHandle(  //because hThreads are local varialbes.
	//DeinitIOTC
	printf("avClientStop calling....\n");
	avClientStop(avIndex);
	printf("avClientStop OK\n");
	IOTC_Session_Close(gSID);
	printf("IOTC_Session_Close OK\n");
	avDeInitialize();
	IOTC_DeInitialize();

	printf("StreamClient exit...\n");

//	MessageBox("IOTC finished!");
	//return MessageBox(hWnd,L"Get date OK!",0,0);
	return ;
}
DWORD WINAPI thread_ReceiveVideo(VOID *arg)
{
//	printf("[thread_ReceiveVideo] Starting....\n");

	int avIndex = *(int *)arg;
	char *buffer=new char[VIDEO_BUF_SIZE];
	int ret;

	FRAMEINFO_t frameInfo;
	unsigned int frmNo;

	//int flag = 0, cnt = 0;

	//open file to store date used to analyse.
	//ofstream myfile;
	//myfile.open("example.txt");
	//char fn[32];
	while(1)
	{
		ret = avRecvFrameData(avIndex, buffer, VIDEO_BUF_SIZE, (char *)&frameInfo, sizeof(FRAMEINFO_t), &frmNo);
		//my output ret 

		if(ret == AV_ER_DATA_NOREADY)
		{
			//printf("AV_ER_DATA_NOREADY[%d]\n", frmNo);
			Sleep(300);
			continue;
		}
		else if(ret == AV_ER_LOSED_THIS_FRAME)
		{
			printf("Lost video frame NO[%d]\n", frmNo);
			continue;
		}
		else if(ret == AV_ER_INCOMPLETE_FRAME)
		{
			printf("Incomplete video frame NO[%d]\n", frmNo);
			continue;
		}
		else if(ret == AV_ER_SESSION_CLOSE_BY_REMOTE)
		{
			printf("[thread_ReceiveVideo] AV_ER_SESSION_CLOSE_BY_REMOTE\n");
			break;
		}
		else if(ret == AV_ER_REMOTE_TIMEOUT_DISCONNECT)
		{
			printf("[thread_ReceiveVideo] AV_ER_REMOTE_TIMEOUT_DISCONNECT\n");
			break;
		}
		else if(ret == IOTC_ER_INVALID_SID)
		{
			printf("[thread_ReceiveVideo] Session cant be used anymore\n");
			break;
		}
		//the key 
		//memcpy(bufTemp[frmNo],buffer,sizeof(buffer));
		
		((CCMS_demoDlg*)theApp.m_pMainWnd)->ShowPic(buffer);
	}


	return 0;
}
int CCMS_demoDlg::start_ipcam_stream(int avIndex)
{
	int ret;
	unsigned short val = 0;
	if((ret = avSendIOCtrl(avIndex, IOTYPE_INNER_SND_DATA_DELAY, (char *)&val, sizeof(unsigned short)) < 0))
	{
		printf("start_ipcam_stream failed[%d]\n", ret);
		return 0;
	}

	SMsgAVIoctrlAVStream ioMsg;
	memset(&ioMsg, 0, sizeof(SMsgAVIoctrlAVStream));
	if((ret = avSendIOCtrl(avIndex, IOTYPE_USER_IPCAM_START, (char *)&ioMsg, sizeof(SMsgAVIoctrlAVStream)) < 0))
	{
		printf("start_ipcam_stream failed[%d]\n", ret);
		return 0;
	}
	return 1;

}

void CCMS_demoDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStdioFile stFile;
	char* pT = new char[];
	if (stFile.Open("c://1.bmp",CFile::modeRead))
	{
		//����ļ���С
		DWORD Length = stFile.GetLength();
		//���仺��������СΪ�ļ���С
		void far* p = ::GlobalAlloc(0,Length+1); 
		if(p==NULL)
		{
			AfxMessageBox("Alloc memory error!");
		}
		//���ļ�һ�����ݶ�����������
		stFile.Read(p,Length);
		
		pT = (char*)p;
		stFile.Close();
		//�ͷŻ�����
		::GlobalFree(p);
	}
	ShowPic(pT);
	UpdateWindow();
}
