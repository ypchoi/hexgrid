
// HexViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexViewer.h"
#include "HexViewerDlg.h"
#include "afxdialogex.h"

#include "HexInclude.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexViewerDlg dialog



CHexViewerDlg::CHexViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHexViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CHexViewerDlg message handlers

BOOL CHexViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexViewerDlg::OnPaint()
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
		CDialogEx::OnPaint();

        DrawHexGrid();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHexViewerDlg::DrawHexGrid()
{
    typedef HexGrid<HexIndexI, POINT, eHexGridType::PointyTopped> TGrid;

    CClientDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    dc.FillSolidRect(&rect, RGB(0xFF, 0xFF, 0xFF));

    float radius = 30.0f;
    int countX = (int)std::ceil(rect.Width() / TGrid::GetHorizontalDistance(radius));
    int countY = (int)std::ceil(rect.Height() / TGrid::GetVerticalDistance(radius));

    POINT offset;
    offset.x = TGrid::GetWidth(radius) / 2;
    offset.y = TGrid::GetHeight(radius) / 2;

    for (int y = 0; y < countY; ++y)
    {
        int startX = -((y + 1) / 2);
        int endX = startX + countX;

        CString buf;
        buf.Format(L"Line %d : %d ~ %d\n", y, startX, endX - 1);
        ::OutputDebugStringW((LPCWSTR)buf);

        for (int x = startX; x < endX; ++x)
        {
            TGrid grid(HexIndexI(x, y), radius);

            for (int i = 0; i < 6; ++i)
            {
                POINT start = grid.GetCorner(i);
                POINT end = grid.GetCorner((i + 1) % 6);

                dc.MoveTo(start.x + offset.x, rect.bottom - start.y - offset.y);
                dc.LineTo(end.x + offset.x, rect.bottom - end.y - offset.y);
            }

            RECT textRect;
            textRect.left = grid.center.x + offset.x;
            textRect.top = rect.bottom - grid.center.y - offset.y;
            textRect.right = textRect.left + 1;
            textRect.bottom = textRect.right + 1;

            CString text;
            text.Format(L"%d,%d", x, y);
            dc.DrawText(text, &textRect, DT_NOCLIP | DT_CENTER | DT_VCENTER);
        }
    }
}

void CHexViewerDlg::OnSize(UINT nType, int cx, int cy)
{
    Invalidate(TRUE);
}

