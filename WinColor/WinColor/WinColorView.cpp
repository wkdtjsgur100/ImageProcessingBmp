
// WinColorView.cpp: CWinColorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinColor.h"
#endif

#include "WinColorDoc.h"
#include "WinColorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinColorView

IMPLEMENT_DYNCREATE(CWinColorView, CView)

BEGIN_MESSAGE_MAP(CWinColorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWinColorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, &CWinColorView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWinColorView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CWinColorView::OnUpdateEditPaste)
END_MESSAGE_MAP()

// CWinColorView 생성/소멸

CWinColorView::CWinColorView()
{
	// TODO: add construction code here
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i<256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CWinColorView::~CWinColorView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CWinColorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWinColorView 그리기

void CWinColorView::OnDraw(CDC* pDC)
{
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	if (pDoc->m_InImg == NULL) return;

	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height,
		0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
}


// CWinColorView 인쇄


void CWinColorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWinColorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWinColorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWinColorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CWinColorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWinColorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWinColorView 진단

#ifdef _DEBUG
void CWinColorView::AssertValid() const
{
	CView::AssertValid();
}

void CWinColorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinColorDoc* CWinColorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinColorDoc)));
	return (CWinColorDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinColorView 메시지 처리기

void CWinColorView::OnEditCopy()
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->CopyClipboard(pDoc->m_InImg, height,
		width, pDoc->dibHi.biBitCount);
}



void CWinColorView::OnEditPaste()
{
	// TODO: Add your command handler code here
	if (!IsClipboardFormatAvailable(CF_DIB)) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_DIB));
}
