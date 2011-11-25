// TTPlayerControlDlg.h : ͷ�ļ�
//
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27000"

class CTTPlayerControlDlg;

typedef struct CLIENT_INF
{
	//OVERLAPPED Overlapped;			//IO�ص����
	SOCKET socket;						//�׽���
	char Buffer[DEFAULT_BUFLEN];				//�û����ݻ�����
	//	WSABUF	RecBuf;						//���ݻ�����
	DWORD dwSendLen;					//�������ݳ���
	DWORD dwRecvLen;					//�������ݳ���
	CTTPlayerControlDlg * process;
	//NetEvent neType;					//�����¼�
}*LPCLIENT_INF;


//��ͻ���ͨ����Ϣ�б�
typedef struct SOCKET_INF
{
	SOCKET socket;
	int type;
	char Buffer[DEFAULT_BUFLEN];
	DWORD dwSendLen;
	DWORD dwRecvLen;
}*LPSOCKET_INF;

// CTTPlayerControlDlg �Ի���
class CTTPlayerControlDlg : public CDialog
{
// ����
public:
	CTTPlayerControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TTPLAYERCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	HWND parentHandler;
	HWND listHandler;
	HWND nextSongHandler;
	HWND lastSongHandler;
	HWND startSongHandler;
	HWND stopSongHandler;
	HWND exitSongHandler;

	bool bServerStop;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void SendMessageTT(HWND handler);
	afx_msg void OnBnClickedStartPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedShowWindow();
	afx_msg void OnBnClickedHideWindow();
	void GetHandler(void);
	afx_msg void OnBnClickedBtnGethandle();
	UINT static ServerThread(void* lpParam);
	UINT static ClientThread(void* lpParam);
	int RecvMsg(void* lpParam);
	char* UnicodeToAnsi(char* unStr, int , char *);
};

