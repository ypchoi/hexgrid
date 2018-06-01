
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
static const HexCube INVALID_INDEX = HexCube(-1, -1);
static const HexLayout layout = HexLayout(
    HexOrientation::Pointy(), HexPoint(64), HexPoint(100));

CHexViewerDlg::CHexViewerDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CHexViewerDlg::IDD, pParent)
    , m_manager(layout)
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
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

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
        DrawBackground();
        DrawHexGrid();
        DrawRoute();
        DrawOver();
        DrawBox();
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

    const HexLayout& layout = m_manager.GetLayout();
    m_gridRect = m_rect;
    m_gridRect.left = layout.origin.x;
    m_gridRect.bottom = m_rect.Height() - layout.origin.y;
    m_gridRect.right -= 100;
    m_gridRect.top += 100;

    m_manager.Create(HexPoint(m_gridRect.Width(), m_gridRect.Height()));
}


void CHexViewerDlg::DrawBackground()
{
    CClientDC dc(this);
    dc.FillSolidRect(&m_rect, RGB(0xFF, 0xFF, 0xFF));
    dc.FillSolidRect(&m_gridRect, RGB(0xC0, 0xC0, 0xC0));
}

void CHexViewerDlg::DrawHexGrid()
{
    CClientDC dc(this);

    const HexLayout& layout = m_manager.GetLayout();

    m_manager.ForEach([this, &layout, &dc](const HexCube& grid)
    {
        DWORD color = RGB(0x80, 0x80, 0x80);
        DrawGrid(grid, dc, color);

        HexPoint center = grid.GetCenter(layout);

        RECT textRect;
        textRect.left = center.x + 20;
        textRect.top = m_rect.bottom - center.y - 20;
        textRect.right = textRect.left + 1;
        textRect.bottom = textRect.right + 1;

        CString text;
        text.Format(L"%d,%d,%d", grid.q, grid.r, grid.s);
        dc.DrawText(text, &textRect, DT_NOCLIP | DT_CENTER | DT_VCENTER);
    });
}

void CHexViewerDlg::DrawOver()
{
    if (m_over == INVALID_INDEX)
        return;

    CClientDC dc(this);
    DrawGrid(m_over, dc, RGB(0xFF, 0, 0));
}

void CHexViewerDlg::DrawBox()
{
    if (m_begin == INVALID_INDEX || m_end == INVALID_INDEX)
        return;

    CClientDC dc(this);

    {
        CPoint begin = HexPointToPoint(m_beginPoint);
        CPoint end = HexPointToPoint(m_endPoint);

        m_boxRect.left = (std::min)(begin.x, end.x);
        m_boxRect.top = (std::min)(begin.y, end.y);
        m_boxRect.right = (std::max)(begin.x, end.x);
        m_boxRect.bottom = (std::max)(begin.y, end.y);
        dc.FillSolidRect(&m_boxRect, RGB(0xC0, 0x80, 0x00));
    }

    {
        HexCube endIndex = (m_end != INVALID_INDEX) ? m_end : m_over;
        std::set<HexCube> grids = m_manager.IntersectBox(m_beginPoint, m_endPoint);

        for (const auto& r : grids)
        {
            DrawGrid(r, dc, RGB(0x00, 0x80, 0x00));
        }
    }
}

void CHexViewerDlg::DrawRoute()
{
    if (m_begin == INVALID_INDEX || (m_over == INVALID_INDEX && m_end == INVALID_INDEX))
        return;

    CClientDC dc(this);

    HexCube endIndex = (m_end != INVALID_INDEX) ? m_end : m_over;

    auto route = m_begin.GetLine(endIndex);
    for (const auto& r : route)
    {
        DrawGrid(r, dc, RGB(0, 0, 0xFF));
    }
}

void CHexViewerDlg::DrawGrid(const HexCube& grid, CClientDC& dc, COLORREF color)
{
    CPen pen;
    pen.CreatePen(PS_SOLID, 5, color);
    CPen* oldPen = dc.SelectObject(&pen);

    const HexPoint& start = m_manager.GetStart();
    const auto layout = m_manager.GetLayout();
    const auto corners = grid.GetCorners(layout);

    for (int i = 0; i < 6; ++i)
    {
        const HexPoint& from = corners[i];
        const HexPoint& to = corners[(i + 1) % 6];

        dc.MoveTo(from.x, m_rect.bottom - from.y);
        dc.LineTo(to.x, m_rect.bottom - to.y);
    }

    dc.SelectObject(oldPen);
}

HexPoint CHexViewerDlg::PointToHexPoint(const CPoint& point) const
{
    return HexPoint(point.x, m_rect.Height() - point.y);
}

CPoint CHexViewerDlg::HexPointToPoint(const HexPoint& hp) const
{
    return CPoint(hp.x, m_rect.Height() - hp.y);
}

void CHexViewerDlg::OnSize(UINT nType, int cx, int cy)
{
    CreateHexGrid();
    Invalidate(TRUE);
}

void CHexViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    const HexPoint hp = PointToHexPoint(point);

    HexCube grid;
    if (m_manager.GetGrid(grid, hp))
    {
        if (m_over == grid)
            return;

        m_over = grid;
        Invalidate(TRUE);
    }
}

void CHexViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    const HexPoint hp = PointToHexPoint(point);

    HexCube grid;
    if (!m_manager.GetGrid(grid, hp))
        return;

    if (m_begin == INVALID_INDEX || m_end != INVALID_INDEX)
    {
        m_begin = grid;
        m_end = INVALID_INDEX;
        m_beginPoint = hp;
    }
    else
    {
        m_end = grid;
        m_endPoint = hp;
    }

    Invalidate(FALSE);
}
