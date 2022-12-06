// UDPcarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPcar.h"
#include "UDPcarDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPcarDlg dialog

CUDPcarDlg::CUDPcarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPcarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDPcarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPcarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDPcarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUDPcarDlg, CDialog)
	//{{AFX_MSG_MAP(CUDPcarDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Send, OnBUTTONSend)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_RECVDATA,OnRecvData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPcarDlg message handlers

BOOL CUDPcarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitSocket();
	//将套接字和对话框关联起来
	RECVPARAM *pRecvParam = new RECVPARAM; 
	pRecvParam->sock=m_socket;
	pRecvParam->hwnd=m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
	CloseHandle(hThread);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPcarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUDPcarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUDPcarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CUDPcarDlg::InitSocket()
{
	char name[255];
	LPCSTR localIP;
	PHOSTENT hostinfo;
	//先获取本地IP地址
	if(gethostname(name,sizeof(name))==0)
	{//如果能获取计算机主机信息的话，则获取本机IP地址
		if((hostinfo = gethostbyname(name))!=NULL)
		{
			localIP = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
		}
	}
	SetDlgItemText(IDC_EDIT_LocalIP,localIP);
	SetDlgItemText(IDC_EDIT_LoaclPort,"1025");

	SetDlgItemText(IDC_IPADDRESS_Server,"192.168.159.19");
	SetDlgItemText(IDC_EDIT_ServerPort,"1025");


	//建立套接字
	m_socket=socket(AF_INET,SOCK_DGRAM,0);//UDP
	if(INVALID_SOCKET==m_socket)
	{
		MessageBox("套接字创建失败！");
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(1025);
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	retval=bind(m_socket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR));
	if(SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		MessageBox("绑定失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CUDPcarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	CString strSend;//待发送的命令
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case 'W':
		//	MessageBox("W down");
			strSend = "@001#";//前进
			break;
		case 'S':
			strSend = "@002#";//后退
			break;
		case 'A':
			strSend = "@003#";//左转
			break;
		case 'D':
			strSend = "@004#";//右转
			break;
		default:
			break;
		}
	}
	else if(pMsg->message == WM_KEYUP)
	{
		if(pMsg->wParam == 'W'||pMsg->wParam == 'S'||pMsg->wParam == 'A'||pMsg->wParam == 'D')
//			MessageBox("w up");
			strSend = "@000#";//停止
	}

	//命令发送
	DWORD dwIP;   
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_Server))->GetAddress(dwIP);

	SOCKADDR_IN addrTo;  //服务器信息
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(1025);
	addrTo.sin_addr.S_un.S_addr=htonl(dwIP);

	sendto(m_socket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrTo,sizeof(SOCKADDR));

	return CDialog::PreTranslateMessage(pMsg);
}

void CUDPcarDlg::OnBUTTONSend() 
{
	// TODO: Add your control notification handler code here
	DWORD dwIP;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_Server))->GetAddress(dwIP);

	SOCKADDR_IN addrTo;  //服务器信息
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(8080);
	addrTo.sin_addr.S_un.S_addr=htonl(dwIP);

	CString strSend;
	GetDlgItemText(IDC_EDIT_Send,strSend);
	sendto(m_socket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrTo,sizeof(SOCKADDR));
//	SetDlgItemText(IDC_EDIT_Send,"");
}

DWORD WINAPI CUDPcarDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;//释放内存操作

	CClientDC picDraw(AfxGetMainWnd()->GetDlgItem(IDC_STATIC_pic));
	int i=0,j=0;

	SOCKADDR_IN addrFrom;
	int len = sizeof(SOCKADDR);

	char recbuf[644] = {0};
	Data *OVdata;
	int retval;
	unsigned short color;
	char R,G,B;
	while(1)
	{
		retval = recvfrom(sock,recbuf,sizeof(recbuf),0,(SOCKADDR*)&addrFrom,&len);
		if(SOCKET_ERROR==retval)
			break;
		if(recbuf==NULL)
		{
			Sleep(1);
			continue;
		}
		OVdata = (Data *)recbuf;
		j = OVdata->x;
		for(i=0;i<320;i++)
		{
			color = OVdata->data[i];

			R = ((char)(color>>11)&(0x1f))<<2;
			G = ((char)(color>>5)&(0x3f))<<1;
			B = ((char)color&(0x1f))<<2;
			
			picDraw.SetPixel(i,j,RGB(R,G,B));
		}
		
	}

/*
	char recvBuf[644]={0};
	char tempBuf[60];
	int retval;
	while(TRUE)
	{
		retval = recvfrom(sock,recvBuf,40,0,(SOCKADDR*)&addrFrom,&len);
		if(SOCKET_ERROR==retval)
			break;
		sprintf(tempBuf,"来自%s: %s",inet_ntoa(addrFrom.sin_addr),recvBuf);
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempBuf);
	} */
	return 0;

}



void CUDPcarDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
//	CString str=(char*)lParam;		
//	CString strTemp;
//	GetDlgItemText(IDC_EDIT_RECV,strTemp);
//	str+="\r\n";
//	str+='\0';
//	str=strTemp;
	SetDlgItemText(IDC_EDIT_Rev,(char*)lParam);
}
