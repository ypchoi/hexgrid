
// HexViewerDlg.h : header file
//

#pragma once
#include "HexInclude.h"

// CHexViewerDlg dialog
class CHexViewerDlg : public CDialogEx
{
// Construction
public:
	CHexViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HEXVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    void CreateHexGrid();
    void DrawHexGrid();

private:
    typedef HexGrid_t<float, eHexGridShape::PointyTopped> TGrid;
    HexGridManager<TGrid> m_manager;
    CRect m_rect;
};
