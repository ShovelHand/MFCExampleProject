#include "stdafx.h"
#include "OpenGLControl.h"
#include ".\openglcontrol.h"

using namespace glm;

COpenGLControl::COpenGLControl(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 10.0f;	// Zoom on model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY = 0.0f;		// Rotation on model in camera view
	m_bIsMaximized = false;
}

COpenGLControl::~COpenGLControl(void)
{
	glBindVertexArray(0);
	glUseProgram(0);
	glDeleteBuffers(1, &_vbo_vpoint);
//	glDeleteBuffers(1, &_vbo_vtexcoord);
	glDeleteProgram(pid);
	glDeleteVertexArrays(1, &_vao);
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
//	glMatrixMode(GL_PROJECTION);

//	glLoadIdentity();

	// Set our current view perspective
//	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
//	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
	case SIZE_MAXIMIZED:
	{
		// Get the current window rect
		GetWindowRect(m_rect);

		// Move the window accordingly
		MoveWindow(6, 6, cx - 14, cy - 14);

		// Get the new window rect
		GetWindowRect(m_rect);

		// Store our old window as the new rect
		m_oldWindow = m_rect;

		break;
	}

	// If window resize token is "restore"
	case SIZE_RESTORED:
	{
		// If the window is currently maximized
		if (m_bIsMaximized)
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly (to our stored old window)
			MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;
		}

		break;
	}
	}
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();
	
//	m_unpTimer = SetTimer(1, 1, 0);
	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// If the current view is perspective...
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControl::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		glClearColor(0.50f, 0.50f, 0.50f, 0.0f);
		// Clear color and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw OpenGL scene
		oglDrawScene();

		// Swap buffers
		SwapBuffers(hdc);

		break;
	}

	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX = (float)point.x;
	m_fLastY = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}

	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(NULL, className, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow = rect;
	m_originalRect = rect;

	hWnd = parent;
}


void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	/*
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	*/
	// *** initialize program shaders
	pid = LoadShader("c:\\Users\\Alex\\documents\\Shaders\\vertShader.glsl", 
		"c:\\Users\\Alex\\documents\\Shaders\\fragShader.glsl");
	if (!pid) exit(EXIT_FAILURE);

	//make the grid

	//--- Vertex one vertex Array
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	MakeVertices(40,40);
//	triangle_vec = { vec3(0.0f,1.0f,-2.0f), vec3(-1.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, -3.0f) };

	///--- Buffer
	glGenBuffers(1, &_vbo_vpoint);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*triangle_vec.size(), &triangle_vec[0], GL_STATIC_DRAW);

	///--- Attribute
	GLuint vpoint_id = glGetAttribLocation(pid, "vpoint");
	glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, GL_TRUE, 0, NULL);
	glEnableVertexAttribArray(vpoint_id);

	//TODO: function for generating noise image goes here.


	///--- to avoid the current object being polluted
	glBindVertexArray(0);
	glUseProgram(0);

	//setup camera
	dirVec = vec3(0.0f, 0.0f, -2.0f);
	eye = vec3(0.0f, 10.0f, 10.0f);
	Projection = perspective(radians(45.0f), 1.0f, 1.0f, 100.0f);
//	View = translate(mat4(1.0f), eye);
	View = lookAt(eye, dirVec, vec3(0.0f, 1.0f, 0.0f));
	Model = scale(mat4(1.0f), vec3(1.0f));

	// Send draw request
	OnDraw(NULL);
}

void COpenGLControl::oglDrawScene(void)
{
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(pid);
	
	glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pid, "view"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, &Projection[0][0]);
	
	glBindVertexArray(_vao);

	glPointSize(5.0f);
	glDrawArrays(GL_TRIANGLES, 0, triangle_vec.size()); 

	glUseProgram(0);
	glBindVertexArray(0);
}

void COpenGLControl::MakeVertices(int width, int height)
{

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			vertices.push_back(vec3(float(i), 0.0, float(j)));
	}

	//triangle strip
	for (int j = 0; j < (height - 1); ++j)
	{
		//sqaures inside a triangle strip
		for (int i = 0; i < (width - 1); ++i)
		{
			int topleft = j * width + i;
			int topright = topleft + 1;
			int bottomleft = (j + 1) * width + i;
			int bottomright = bottomleft + 1;
			//the upper left triangle
			triangle_vec.push_back(vertices[topleft]);
			triangle_vec.push_back(vertices[topright]);
			triangle_vec.push_back(vertices[bottomleft]);
			//the lower right triangle
			triangle_vec.push_back(vertices[topright]);
			triangle_vec.push_back(vertices[bottomright]);
			triangle_vec.push_back(vertices[bottomleft]);
		}
	}
	/*
	//make the pixel coordinates for the height map texture
	for (std::vector<vec3>::iterator itr = triangle_vec.begin(); itr != triangle_vec.end(); ++itr)
		vtexcoord.push_back(vec2((*itr).x() / float(width), (*itr).z() / float(height)));
		*/
}

