
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
static const HexIndexI INVALID_INDEX = HexIndexI(-1, -1);

CHexViewerDlg::CHexViewerDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CHexViewerDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_begin = INVALID_INDEX;
    m_over = INVALID_INDEX;
    m_end = INVALID_INDEX;
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
    ON_WM_LBUTTONDOWN()
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
        DrawRoute();
        DrawOver();
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
        DrawGrid(grid, dc, RGB(0xA0, 0xA0, 0xA0));

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

void CHexViewerDlg::DrawOver()
{
    if (m_over == INVALID_INDEX)
        return;

    const TGrid* pGrid = m_manager.GetGrid(m_over);
    if (!pGrid)
        return;

    CClientDC dc(this);
    DrawGrid(*pGrid, dc, RGB(0xFF, 0, 0));
}

void CHexViewerDlg::DrawRoute()
{
    if (m_begin == INVALID_INDEX || (m_over == INVALID_INDEX && m_end == INVALID_INDEX))
        return;

    CClientDC dc(this);

    HexIndexI endIndex = (m_end != INVALID_INDEX) ? m_end : m_over;

    HexCubeIndexI beginCube = HexConvert<TGrid::Shape>::ToCube(m_begin);
    HexCubeIndexI endCube = HexConvert<TGrid::Shape>::ToCube(endIndex);

    auto route = HexLine::Get(beginCube, endCube);
    for (const auto& r : route)
    {
        HexIndexI hex = HexConvert<TGrid::Shape>::ToHex(r);
        const TGrid* pGrid = m_manager.GetGrid(hex);
        if (!pGrid)
            continue;

        DrawGrid(*pGrid, dc, RGB(0, 0, 0xFF));
    }
}

void CHexViewerDlg::DrawGrid(const TGrid& grid, CClientDC& dc, COLORREF color)
{
    CPen pen;
    pen.CreatePen(PS_SOLID, 5, color);
    CPen* oldPen = dc.SelectObject(&pen);

    const HexPixelF& start = m_manager.GetStart();

    for (int i = 0; i < 6; ++i)
    {
        HexPixelF from = grid.GetCorner(i) + start;
        HexPixelF to = grid.GetCorner((i + 1) % 6) + start;

        dc.MoveTo(from.x, m_rect.bottom - from.y);
        dc.LineTo(to.x, m_rect.bottom - to.y);
    }

    dc.SelectObject(oldPen);
}

const CHexViewerDlg::TGrid* CHexViewerDlg::GetGrid(const CPoint & point)
{
    HexPixelF pixel(point.x, m_rect.Height() - point.y);
    const TGrid* pGrid = m_manager.GetGrid(pixel);
    return pGrid;
}

void CHexViewerDlg::OnSize(UINT nType, int cx, int cy)
{
    CreateHexGrid();
    Invalidate(TRUE);
}

void CHexViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    const TGrid* pGrid = GetGrid(point);
    if (!pGrid || m_over == pGrid->index)
        return;

    m_over = pGrid->index;
    Invalidate(TRUE);
}

void CHexViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    const TGrid* pGrid = GetGrid(point);
    if (!pGrid)
        return;

    if (m_begin == INVALID_INDEX || m_end != INVALID_INDEX)
    {
        m_begin = pGrid->index;
        m_end = INVALID_INDEX;
    }
    else
    {
        m_end = pGrid->index;
    }

    Invalidate(FALSE);
}
