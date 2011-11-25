// TTPlayerControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TTPlayerControl.h"
#include "TTPlayerControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTTPlayerControlDlg �Ի���




CTTPlayerControlDlg::CTTPlayerControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTPlayerControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	parentHandler = listHandler = nextSongHandler = lastSongHandler = startSongHandler = stopSongHandler = exitSongHandler = NULL;
}

void CTTPlayerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTTPlayerControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_START_PAUSE, &CTTPlayerControlDlg::OnBnClickedStartPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTTPlayerControlDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PREV, &CTTPlayerControlDlg::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CTTPlayerControlDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_SHOW_WINDOW, &CTTPlayerControlDlg::OnBnClickedShowWindow)
	ON_BN_CLICKED(IDC_HIDE_WINDOW, &CTTPlayerControlDlg::OnBnClickedHideWindow)
	ON_BN_CLICKED(IDC_BTN_GETHANDLE, &CTTPlayerControlDlg::OnBnClickedBtnGethandle)
END_MESSAGE_MAP()


// CTTPlayerControlDlg ��Ϣ�������

BOOL CTTPlayerControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	HANDLE m_listenHandler = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerThread, this, 0, NULL);

	parentHandler = FindWindow(L"TTPlayer_PlayerWnd",NULL)->GetSafeHwnd();
	listHandler = FindWindow(L"TTPlayer_PlayListWnd",NULL)->GetSafeHwnd();
	if (parentHandler == NULL)
	{
		// ����ttplayer
		ShellExecute(NULL, NULL,L"ttplayer.exe",NULL,NULL,SW_HIDE);
	}
	else
	{
		GetHandler();
	}

	while( parentHandler == NULL)
		parentHandler = FindWindow(L"TTPlayer_PlayerWnd",NULL)->GetSafeHwnd();

	while (exitSongHandler == NULL || nextSongHandler == NULL || lastSongHandler == NULL || startSongHandler == NULL || stopSongHandler == NULL)
		GetHandler();

	//FindWindow(parentHandler,NULL,"SkinButton",L"");
	//HWND handler = FindWindowEx(parentHandler,NULL,L"SkinButton",L"")->GetSafeHwnd();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTTPlayerControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTTPlayerControlDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTTPlayerControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTTPlayerControlDlg::SendMessageTT(HWND handler)
{
	::SendMessage(handler,WM_LBUTTONDOWN,NULL,NULL);
	::SendMessage(handler,WM_LBUTTONUP, NULL,NULL);
}

void CTTPlayerControlDlg::OnBnClickedStartPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessageTT(startSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessageTT(stopSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnPrev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessageTT(lastSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessageTT(nextSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedShowWindow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::ShowWindow(parentHandler, 1);
	::ShowWindow(listHandler, 1);
}

void CTTPlayerControlDlg::OnBnClickedHideWindow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::ShowWindow(parentHandler, 0);
	::ShowWindow(listHandler, 0);
}

void CTTPlayerControlDlg::GetHandler(void)
{
	listHandler = FindWindow(L"TTPlayer_PlayListWnd",NULL)->GetSafeHwnd();
	nextSongHandler = ::GetDlgItem(parentHandler, 32006);
	lastSongHandler = ::GetDlgItem(parentHandler, 32005);
	startSongHandler = ::GetDlgItem(parentHandler, 32000);
	stopSongHandler = ::GetDlgItem(parentHandler, 32002);
	exitSongHandler = ::GetDlgItem(parentHandler, 57665);
}

void CTTPlayerControlDlg::OnBnClickedBtnGethandle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetHandler();
}

UINT CTTPlayerControlDlg::ServerThread(void* lpParam)
{
	CTTPlayerControlDlg *dlg = (CTTPlayerControlDlg*)lpParam;
	//dlg->ShowMessage(L"hello");

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	//	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo * result = NULL;
	struct addrinfo hints;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		CString str;
		str.Format(L"WSAStartup failed with error: %d\n", iResult);
		//dlg->ShowMessage(str);
		AfxMessageBox(str);
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol =  IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Resolve the server address and port
	iResult = getaddrinfo(NULL,DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		CString str;
		str.Format(L"getaddrinfo failed with error: %d\n", iResult);
		//dlg->ShowMessage(str);
		AfxMessageBox(str);
		return -1;
	} 

	//Creating a Socket for the server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		CString str;
		str.Format(L"Socket Failed with error: %ld\n", WSAGetLastError());
		AfxMessageBox(str);
		WSACleanup();
		return -1;
	}

	// Setup the TCP listening socket, Binding a Socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		CString str;
		str.Format(L"Bind failed with error: %d\n", WSAGetLastError());
		AfxMessageBox(str);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	freeaddrinfo(result);

	//Listen on a socket
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		CString str;
		str.Format(L"Listen failed with error: %d\n", WSAGetLastError());
		AfxMessageBox(str);
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}

	while (true)
	{
		//Accept a client socket
		SOCKET clientSocket = accept(ListenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			CString str;
			str.Format(L"Accept failed with error: %d\n", WSAGetLastError());
			AfxMessageBox(str);
			closesocket(ListenSocket);
			WSACleanup();
			return -1;
		}

		//��ʼ���ṹ�崫�ݲ����� �û��߳̽��д���
		CLIENT_INF *pInfo = new CLIENT_INF;
		memset(pInfo, 0, sizeof(CLIENT_INF));			//��ʼ��pIOInfo
		memset(pInfo->Buffer, 0, DEFAULT_BUFLEN);
		//memset(pIOInfo->Overlapped, 0, sizeof(OVERLAPPED));		//��ʼ���ص��ṹ
		pInfo->socket = clientSocket;
		pInfo->dwRecvLen = DEFAULT_BUFLEN;
		pInfo->dwSendLen = DEFAULT_BUFLEN;
		pInfo->process = dlg;
		// 		pInfo->RecBuf.len = DEFAULT_BUFLEN;
		// 		pInfo->RecBuf.buf = pIOInfo->Buffer;

		HANDLE  hThread;  
		hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, pInfo, 0, NULL);
		if (hThread == NULL)
		{
			AfxMessageBox(L"CreateThread error!");
			return -1;
		}
		//HANDLE m_listenHandler = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenSocket, this, 0, NULL);
		//closesocket(clientSocket);
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

UINT CTTPlayerControlDlg::ClientThread(void* lpParam)
{
	CLIENT_INF * pInfo = (CLIENT_INF*)lpParam;
	int iResult = 0;
	int iSendResult = 0;


	do 
	{
		//�������Unicode �� char * ת��������
		//CString str;
		iResult = recv(pInfo->socket, pInfo->Buffer, pInfo->dwRecvLen, 0);

		char *dst = (char*)malloc(22);
		pInfo->process->UnicodeToAnsi(pInfo->Buffer, iResult, dst);

		if (iResult > 0)
		{
			if ( !strncmp(dst, "start/pause", 11) )
				pInfo->process->OnBnClickedStartPause();
			else if ( !strncmp(dst, "prev", 4) )
				pInfo->process->OnBnClickedBtnPrev();
			else if ( !strncmp(dst, "next", 4) )
				pInfo->process->OnBnClickedBtnNext();
			else if ( !strncmp(dst, "stop", 4) )
				pInfo->process->OnBnClickedBtnStop();
			
			//Echo the buffer back to the sender
// 			iSendResult = send(pInfo->socket, pInfo->Buffer, iResult, 0);
// 			if (iSendResult == SOCKET_ERROR)
// 			{
// 				AfxMessageBox(L"send failed!");
// 				return -1;
// 			}
			//pInfo->process->OnBnClickedStartPause();
		}
		else if (iResult == 0)
		{	//AfxMessageBox(L"Connetion closing...\n");
		}
		else
		{
			AfxMessageBox(L"recv failed!");
			return -1;
		}
	} while (iResult > 0);

	closesocket(pInfo->socket);	
	return 0;
}

int CTTPlayerControlDlg::RecvMsg(void* lpParam)
{
	return 0;
}

char* CTTPlayerControlDlg::UnicodeToAnsi(char* src, int len, char * dst)
{
	char * dst2 = dst;
	for (int i = 0; i < len; i++, src++)
	{
		if (*src == NULL)
		{
			*(dst2++) = *(src+1);
		}
		//src++;
	}
	
	*(dst2+1) = NULL;

	return NULL;
}
