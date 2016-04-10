
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
//static UINT BASED_CODE buttons[] =
//{
//	// same order as in the bitmap 'toolbar.bmp'
//	ID_CUBE_ADD,
//	ID_PRISM_ADD,
//	ID_SHIFT_MODE,
//	ID_ROTATE_MODE,
//	ID_SELECT_MODE,
//	ID_COLOR_MODE,
//	ID_CAMERA_MODE,
//	ID_FILE_SAVE,
//	ID_FILE_OPEN//,
//};
IMPLEMENT_DYNCREATE(COglLab1View, CView)

BEGIN_MESSAGE_MAP(COglLab1View, CView)
	ON_COMMAND(ID_CUBE_ADD, OnCubeCreate)
	ON_COMMAND(ID_PRISM_ADD, OnPrismCreate)
	ON_COMMAND(ID_SHIFT_MODE, OnShiftMode)
	ON_COMMAND(ID_ROTATE_MODE, OnRotateMode)
	ON_COMMAND(ID_SELECT_MODE, OnSelectMode)
	ON_COMMAND(ID_COLOR_MODE, OnColorMode)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_LOAD, OnLoad)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// COglLab1View construction/destruction
// You will add stuff here!!!!
const char* const COglLab1View::_ErrorStrings[] = {
	{ "No Error" },                     // 0
	{ "Unable to get a DC" },           // 1
	{ "ChoosePixelFormat failed" },     // 2
	{ "SelectPixelFormat failed" },     // 3
	{ "wglCreateContext failed" },      // 4
	{ "wglMakeCurrent failed" },        // 5
	{ "wglDeleteContext failed" },      // 6
	{ "SwapBuffers failed" },           // 7
};
COglLab1View::COglLab1View() : m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0]) // Call constructors

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
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

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

	currContext->Draw();
	//Swap buffers to show result

	SwapBuffers(wglGetCurrentDC());
	Invalidate(FALSE);
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

HANDLE console = NULL; // консоль дл€ вывода 

					   // создаем консоль
void CreateConsole(HWND hwnd)
{
	if (!AllocConsole())
		MessageBox(hwnd, L"Ќе могу создать консоль!", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
	else
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		freopen("CONOUT$", "wt", stdout);
		printf("ѕерва€ строка в консоли\n\n");
	}

}
void COglLab1View::OnCubeCreate()
{
		currContext->addObject(vec3(0), vec3(0), vec3(1), 4, 2, 2, vec4(5.f / 10.f, 0, 1.0, 1));
}
void COglLab1View::OnPrismCreate()
{
	currContext->addObject(vec3(0), vec3(0), vec3(1), 6, 2, 1, vec4(5.f / 10.f, 0, 0.5f, 1));
}
void COglLab1View::OnShiftMode()
{
	currContext->setMode(movement);
}
void COglLab1View::OnRotateMode()
{
	currContext->setMode(rotation);
}
void COglLab1View::OnSelectMode()
{
	currContext->setMode(selection);
}
void COglLab1View::OnColorMode()
{

	currContext->setMode(colour);
}
void COglLab1View::OnSave()
{
	currContext->onSave(m_hWnd);
}
void COglLab1View::OnLoad()
{
	currContext->onLoad(m_hWnd);
}
int COglLab1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
#ifdef _DEBUG
	CreateConsole(m_hWnd);
#endif
	//GetRenderingContext();
	if (!GetRenderingContext())
	{
		//Something went wrong with getting the rendering context.
		//Create a popup with the error message, then quit.
		AfxMessageBox(CString(m_ErrorString));
		return -1;
	}
	currContext = new COpenglContext();
	editor = currContext->editor;
	/*CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	screen_x_size = rect.Width();
	screen_y_size = rect.Height();
*/
	currContext->resize(800, 600);
	
	// We now have a rendering context, so we can set the initial drawing state.
	// Find the initialize OpenGL function provided in the Lab 1 notes and call it here
	//	OnDraw(NULL);
	
	return 0;
}


void COglLab1View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		SetError(6);
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
}
/////////////////////////////////////////////////////////////////////////////
// GL Rendering Context Creation Functions
//
// Since we are using Windows native windowing, we need to set up our own
// OpenGL rendering context. These functions do it to the main view area.
// It is possible to do it to a smaller sub view. If you are curious, you can
// find tutorials on how to do that on the net.
//

// Error reporting utility
void COglLab1View::SetError(int e)
{
	// if there was no previous error,
	// then save this one
	if (_ErrorStrings[0] == m_ErrorString)
	{
		m_ErrorString = _ErrorStrings[e];
	}
}

BOOL COglLab1View::GetRenderingContext()
{
	// Can we put this in the constructor?
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) // failure to get DC
	{
		SetError(1);
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}

	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}

	//Get a new style pixel format
	if (!SetupPixelFormat())
	{
		return FALSE;
	}

	//Setup request for OpenGL 4.4 Core Profile
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,   4,
		WGL_CONTEXT_MINOR_VERSION_ARB,   2,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0, 0  //End
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		//If this driver supports new style rendering contexts, create one
		HGLRC oldContext = m_hRC;
		if (0 == (m_hRC = m_hRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs)))
		{
			SetError(4);
			return FALSE;
		}

		if (!wglMakeCurrent(NULL, NULL))
			wglDeleteContext(oldContext);
		if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
		{
			SetError(5);
			return FALSE;
		}
	}
	else
	{
		//Otherwise use the old style rendering context we created earlier.
		AfxMessageBox(_T("GL 4.4 Context not possible. Using old style context. (GL 2.1 and before)"));
	}


	return TRUE;
}

BOOL COglLab1View::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		SetError(2);
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		SetError(4);
		return FALSE;
	}

	//Make it current. Now we're ready to get extended features.
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		SetError(5);
		return FALSE;
	}
	return TRUE;
}

BOOL COglLab1View::SetupPixelFormat()
{
	//This is a modern pixel format attribute list.
	//It has an extensible structure. Just add in more argument pairs 
	//befroe the null to request more features.
	int attribList[] =
	{
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
		WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
		WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,     32,
		WGL_DEPTH_BITS_ARB,     24,
		WGL_STENCIL_BITS_ARB,     0,
		0  //End
	};


	unsigned int numFormats;
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfds;
	//Select a pixel format number
	wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);

	//Optional: Get the pixel format's description. We must provide a 
	//description to SetPixelFormat(), but its contents mean little.
	//According to MSDN: 
	//  The system's metafile component uses this structure to record the logical
	//  pixel format specification. The structure has no other effect upon the
	//  behavior of the SetPixelFormat function.
	DescribePixelFormat(m_pDC->GetSafeHdc(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfds);

	//Set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfds))
	{

		DWORD errCode = GetLastError();
		SetError(3);
		return FALSE;
	}

	return TRUE;
}


void COglLab1View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	currContext->Draw();
	//Swap buffers to show result

	SwapBuffers(wglGetCurrentDC());
	Invalidate(FALSE);
}


void COglLab1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	screen_x_size = cx;
	screen_y_size = cy;

	currContext->resize(cx, cy);
	glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);
//	SetCursorPos(cx / 2, cy / 2);
	// TODO: Add your message handler code here
}


BOOL COglLab1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void COglLab1View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	currContext->Draw();
	//Swap buffers to show result
	SwapBuffers(wglGetCurrentDC());
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	MSG msg;
	while (::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));

}


void COglLab1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(currContext->getMode()==selection)
	currContext->selectObject(false, point);

	CView::OnLButtonDown(nFlags, point);
}


void COglLab1View::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint prev = point;
	switch (nFlags)
	{
	/*case MK_MBUTTON:

		break;*/
	case MK_LBUTTON:
		currContext->LButtonMove(point);
		currContext->setCursor(point);
		break;
	case MK_RBUTTON:
		currContext->RButtonMove(point);
		currContext->setCursor(point);
		break;
	default:
		currContext->setCursor(point);
		break;
	}
}

BOOL COglLab1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	currContext->mouseWheel(zDelta);
	return 0;
}


void COglLab1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonDown(nFlags, point);
}
