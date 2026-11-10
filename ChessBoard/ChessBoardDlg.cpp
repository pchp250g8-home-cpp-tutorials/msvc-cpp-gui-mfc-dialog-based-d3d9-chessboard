
// ChessBoardDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChessBoard.h"
#include "ChessBoardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChessBoardDlg dialog



CChessBoardDlg::CChessBoardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHESSBOARD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
}

void CChessBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChessBoardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CChessBoardDlg message handlers

BOOL CChessBoardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	if (!InitializeDirect3D())
	{
		MessageBox(_T("Error Loading DirectX"), _T("Error"));
		EndDialog(0);
		return FALSE;
	}
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChessBoardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChessBoardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		RenderScene();
		//CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChessBoardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChessBoardDlg::InitializeDirect3D()
{
	D3DDISPLAYMODE d3ddm;
	HRESULT hr;
	CRect rect;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	ZeroMemory(&d3ddm, sizeof(d3ddm));
	m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_d3d9 == nullptr) return FALSE;
	hr = m_d3d9->GetAdapterDisplayMode(0, &d3ddm);
	if (FAILED(hr)) return FALSE;
	GetClientRect(&rect);
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = rect.Width();
	d3dpp.BackBufferHeight = rect.Height();
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	hr = m_d3d9->CreateDevice
	(
		0, D3DDEVTYPE_HAL, m_hWnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
		&d3dpp, &m_d3d_Device
	);
	if (FAILED(hr)) return FALSE;
	return TRUE;
}

void CChessBoardDlg::DestroyDirect3D()
{
	m_d3d_Device.Release();
	m_d3d9.Release();
}

void CChessBoardDlg::RenderScene()
{
	int k = 0;
	int width, height;
	CRect rect;
	D3DRECT rects[32];
	D3DCOLOR d3dColor1 = D3DCOLOR_XRGB(0, 255, 255);
	D3DCOLOR d3dColor2 = D3DCOLOR_XRGB(165, 42, 42);
	if (m_d3d9 == nullptr) return;
	if (m_d3d_Device == nullptr) return;
	GetClientRect(&rect);
	width = rect.Width() / 8;
	height = rect.Height() / 8;
	m_d3d_Device->Clear(0, nullptr, D3DCLEAR_TARGET, d3dColor1, 0, 0);
	for (int k = 0; k < 32; k++)
	{
		int i = k / 4;
		int j = (k % 4) * 2 + (i % 2);
		int left = i * width;
		int top = j * height;
		rects[k].x1 = left;
		rects[k].y1 = top; 
		rects[k].x2 = left + width;
		rects[k].y2 = top + height;
	}

	m_d3d_Device->Clear(32, rects, D3DCLEAR_TARGET, d3dColor2, 0, 0);
	m_d3d_Device->BeginScene();
	m_d3d_Device->EndScene();
	m_d3d_Device->Present(nullptr, nullptr, 0, nullptr);
}
void CChessBoardDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	CDialogEx::OnSize(nType, cx, cy);
	GetClientRect(&rect);
	if (m_d3d9 == nullptr) return;
	if (m_d3d_Device == nullptr) return;
	if (rect.Width() > 0 && rect.Height() > 0)
	{
		d3dpp.BackBufferWidth = rect.Width();
		d3dpp.BackBufferHeight = rect.Height();
		m_d3d_Device->Reset(&d3dpp);
	}
	Invalidate();
	// TODO: Add your message handler code here
}

BOOL CChessBoardDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
