
// CMS_demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCMS_demoApp:
// �йش����ʵ�֣������ CMS_demo.cpp
//

class CCMS_demoApp : public CWinApp
{
public:
	CCMS_demoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCMS_demoApp theApp;