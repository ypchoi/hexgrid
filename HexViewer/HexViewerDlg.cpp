
// HexViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexViewer.h"
#include "HexViewerDlg.h"
#include "afxdialogex.h"

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
    ON_WM_MOUSEMOVE()
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
    CreateHexGrid();

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
        DrawSelected();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHexViewerDlg::CreateHexGrid()
{
    GetClientRect(&m_rect);

    const HexPixelF start(0, 0);
    const HexPixelF end(m_rect.Width(), m_rect.Height());

    m_manager.Create(start, end, 50);
}

void CHexViewerDlg::DrawHexGrid()
{
    CClientDC dc(this);
    dc.FillSolidRect(&m_rect, RGB(0xFF, 0xFF, 0xFF));

    const HexPixelF& start = m_manager.GetStart();

    m_manager.ForEach([this, &dc, &start](const TGrid& grid) -> bool
    {
        for (int i = 0; i < 6; ++i)
        {
            HexPixelF from = grid.GetCorner(i);
            HexPixelF to = grid.GetCorner((i + 1) % 6);

            dc.MoveTo(start.x + from.x, m_rect.bottom - from.y - start.y);
            dc.LineTo(start.x + to.x, m_rect.bottom - to.y - start.y);
        }

        RECT textRect;
        textRect.left = start.x + grid.center.x + 10;
        textRect.top = m_rect.bottom - grid.center.y - start.y - 20;
        textRect.right = textRect.left + 1;
        textRect.bottom = textRect.right + 1;

        CString text;
        text.Format(L"%d,%d", grid.index.q, grid.index.r);
        dc.DrawText(text, &textRect, DT_NOCLIP | DT_CENTER | DT_VCENTER);

        return true;
    });
}

void CHexViewerDlg::DrawSelected()
{
    const TGrid* pGrid = m_manager.GetGrid(m_selected);
    if (!pGrid)
        return;

    CClientDC dc(this);

    CPen pen;
    pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));    // ������ �� ����
    CPen* oldPen = dc.SelectObject(&pen);

    const HexPixelF& start = m_manager.GetStart();

    for (int i = 0; i < 6; ++i)
    {
        HexPixelF from = pGrid->GetCorner(i);
        HexPixelF to = pGrid->GetCorner((i + 1) % 6);

        dc.MoveTo(start.x + from.x, m_rect.bottom - from.y - start.y);
        dc.LineTo(start.x + to.x, m_rect.bottom - to.y - start.y);
    }

    dc.SelectObject(oldPen);
}

void CHexViewerDlg::OnSize(UINT nType, int cx, int cy)
{
    CreateHexGrid();
    Invalidate(TRUE);
}

void CHexViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    HexPixelF pixel(point.x, m_rect.Height() - point.y);
    const TGrid* pGrid = m_manager.GetGrid(pixel);
    if (!pGrid)
        return;

    if (m_selected == pGrid->index)
        return;

    m_selected = pGrid->index;
    Invalidate(FALSE);
}
