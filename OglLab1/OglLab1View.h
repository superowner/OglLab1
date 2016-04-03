
// OglLab1View.h : interface of the COglLab1View class
//

#pragma once


class COglLab1View : public CView
{
protected: // create from serialization only
	COglLab1View();
	DECLARE_DYNCREATE(COglLab1View)

// Attributes
public:
	COglLab1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~COglLab1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//////////////////////////////////////////////////////////////////////////////
	// You will add the following stuff!!!

	virtual BOOL GetOldStyleRenderingContext(void);
	virtual BOOL SetupPixelFormat(void);


private:
	//OpenGL Setup
	BOOL GetRenderingContext();
	//Rendering Context and Device Context Pointers
	HGLRC     m_hRC;
	CDC*      m_pDC;

	//Error Handling
	void SetError(int e);
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
	COpenglContext *currContext;
	Workspace *editor;
	int screen_x_size, screen_y_size;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in OglLab1View.cpp
inline COglLab1Doc* COglLab1View::GetDocument() const
   { return reinterpret_cast<COglLab1Doc*>(m_pDocument); }
#endif

