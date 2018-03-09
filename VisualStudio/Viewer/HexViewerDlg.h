
// HexViewerDlg.h : header file
//

#pragma once
#include <vector>
#include "../../Include/HexInclude.h"

typedef HexLayout_t<double> HexLayout;
typedef HexCube_t<int> HexCube;
typedef HexPoint_t<double> HexPoint;

// CHexViewerDlg dialog
class CHexViewerDlg : public CDialogEx
{
// Construction
public:
    CHexViewerDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    enum { IDD = IDD_HEXVIEWER_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    void CreateHexGrid();
    void DrawBackground();
    void DrawHexGrid();
    void DrawOver();
    void DrawRoute();
    void DrawGrid(const HexCube& grid, CClientDC& dc, COLORREF color);
    bool GetGrid(HexCube& out, const CPoint& point);

private:
    HexGrid_t<HexLayout> m_manager;
    CRect m_rect;
    CRect m_gridRect;
    HexCube m_begin;
    HexCube m_over;
    HexCube m_end;
};
