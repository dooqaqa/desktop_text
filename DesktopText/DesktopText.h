
// DesktopText.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDesktopTextApp: 
// �йش����ʵ�֣������ DesktopText.cpp
//

class CDesktopTextApp : public CWinApp
{
public:
	CDesktopTextApp();

// ��д
public:
	virtual BOOL InitInstance();
	ULONG_PTR gdiplus_token_ = 0;
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDesktopTextApp theApp;