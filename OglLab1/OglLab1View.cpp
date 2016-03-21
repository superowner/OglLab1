
// OglLab1View.cpp : implementation of the COglLab1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OglLab1.h"
#endif

#include "OglLab1Doc.h"
#include "OglLab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COglLab1View

IMPLEMENT_DYNCREATE(COglLab1View, CView)

BEGIN_MESSAGE_MAP(COglLab1View, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// COglLab1View construction/destruction

COglLab1View::COglLab1View()
{
	// TODO: add construction code here

}

COglLab1View::~COglLab1View()
{
}

BOOL COglLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COglLab1View drawing

void COglLab1View::OnDraw(CDC* /*pDC*/)
{
	COglLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// COglLab1View diagnostics

#ifdef _DEBUG
void COglLab1View::AssertValid() const
{
	CView::AssertValid();
}

void COglLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COglLab1Doc* COglLab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COglLab1Doc)));
	return (COglLab1Doc*)m_pDocument;
}
#endif //_DEBUG


// COglLab1View message handlers


int COglLab1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void COglLab1View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
}


void COglLab1View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
}


void COglLab1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


BOOL COglLab1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}


void COglLab1View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnTimer(nIDEvent);
}
