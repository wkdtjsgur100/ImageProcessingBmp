
// WinColorView.h: CWinColorView 클래스의 인터페이스
//

#pragma once


class CWinColorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CWinColorView();
	DECLARE_DYNCREATE(CWinColorView)

// 특성입니다.
public:
	CWinColorDoc* GetDocument() const;

// 작업입니다.
public:
	BITMAPINFO *BmInfo;
	int height;
	int width;
	int rwsize;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CWinColorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnChannelsplitRgb();
	afx_msg void OnUpdateChannelsplitRgb(CCmdUI *pCmdUI);
	afx_msg void OnChannelsplitHsi();
	afx_msg void OnUpdateChannelsplitHsi(CCmdUI *pCmdUI);
	afx_msg void OnColor2gray();
	afx_msg void OnUpdateColor2gray(CCmdUI *pCmdUI);
	afx_msg void OnRgbEmbossing();
	afx_msg void OnUpdateRgbEmbossing(CCmdUI *pCmdUI);
	afx_msg void OnRgbNegative();
	afx_msg void OnUpdateRgbNegative(CCmdUI *pCmdUI);
	afx_msg void OnRgb2grayExceptRed();
};

#ifndef _DEBUG  // WinColorView.cpp의 디버그 버전
inline CWinColorDoc* CWinColorView::GetDocument() const
   { return reinterpret_cast<CWinColorDoc*>(m_pDocument); }
#endif

