// TTPlayerControl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTTPlayerControlApp:
// �йش����ʵ�֣������ TTPlayerControl.cpp
//

class CTTPlayerControlApp : public CWinApp
{
public:
	CTTPlayerControlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTTPlayerControlApp theApp;