// UDPcarDlg.h : header file
//

#if !defined(AFX_UDPCARDLG_H__B8FE2306_E4B8_4AFA_B47D_5371C85EFF7E__INCLUDED_)
#define AFX_UDPCARDLG_H__B8FE2306_E4B8_4AFA_B47D_5371C85EFF7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUDPcarDlg dialog
#define WM_RECVDATA    WM_USER+1
struct RECVPARAM
{
	SOCKET sock;
	HWND hwnd;
};

typedef struct
{
	unsigned short x;	//显示起始点坐标
	unsigned short y;
	unsigned short data[162];
}Data;

class CUDPcarDlg : public CDialog
{
// Construction
public:
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	BOOL InitSocket();
	CUDPcarDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUDPcarDlg)
	enum { IDD = IDD_UDPCAR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPcarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUDPcarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONSend();
	//}}AFX_MSG
	afx_msg void OnRecvData(WPARAM wParam,LPARAM lParam);//自定义消息
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;
	HANDLE hThread;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCARDLG_H__B8FE2306_E4B8_4AFA_B47D_5371C85EFF7E__INCLUDED_)
