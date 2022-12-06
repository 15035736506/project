// UDPcar.h : main header file for the UDPCAR application
//

#if !defined(AFX_UDPCAR_H__4EEC5EBC_7694_4CE7_85F4_44C6EDDD3530__INCLUDED_)
#define AFX_UDPCAR_H__4EEC5EBC_7694_4CE7_85F4_44C6EDDD3530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUDPcarApp:
// See UDPcar.cpp for the implementation of this class
//

class CUDPcarApp : public CWinApp
{
public:
	CUDPcarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPcarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUDPcarApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCAR_H__4EEC5EBC_7694_4CE7_85F4_44C6EDDD3530__INCLUDED_)
