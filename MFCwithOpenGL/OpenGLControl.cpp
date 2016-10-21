#include "stdafx.h"
#include "OpenGLControl.h"
#include ".\openglcontrol.h"


using namespace glm;

COpenGLControl::COpenGLControl(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 3.0f;	// Zoom on model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY = 0.0f;		// Rotation on model in camera view
	m_bIsMaximized = false;
	m_bShowHeightMap = false;

	generator = new FBMGenerator(m_iRandSeed);
	lightPos = vec3(110, 40, 11);
}

COpenGLControl::~COpenGLControl(void)
{
	glBindVertexArray(0);
	glUseProgram(0);
	glDeleteBuffers(1, &_vbo_vpoint);
//	glDeleteBuffers(1, &_vbo_vtexcoord);
	glDeleteProgram(pid);
	glDeleteVertexArrays(1, &_vao);
	delete(generator);
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()

END_MESSAGE_MAP()


void COpenGLControl::Generate()
{
	//--- Vertex one vertex Array
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	MakeVertices(m_iTerrainSize, m_iTerrainSize);

	///--- Buffer
	glGenBuffers(1, &_vbo_vpoint);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*triangle_vec.size(), &triangle_vec[0], GL_STATIC_DRAW);

	///--- Attribute
	GLuint vpoint_id = glGetAttribLocation(pid, "vpoint");
	glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, GL_TRUE, 0, NULL);
	glEnableVertexAttribArray(vpoint_id);

	//Height map inits
	
	RGBImage Noise = generator->BuildNoiseImage(FBM_SIZE, FBM_SIZE); 
	
	
	//vertex buffer for heightmap texture
	{
		///--- Buffer
		glGenBuffers(1, &vbo_vtexcoord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vtexcoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*vtexcoord.size(), &vtexcoord[0], GL_STATIC_DRAW);

		///--- Attribute
		GLuint vtexcoord_id = glGetAttribLocation(pid, "TexCoord");
		glEnableVertexAttribArray(vtexcoord_id);
		glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, GL_TRUE, 0, NULL);
	}
	///--- Load texture heightmap
	glGenTextures(1, &tex_heightmap);

	glUniform1i(glGetUniformLocation(pid, "tex_height"), 0 /*GL_TEXTURE6*/);
	glBindTexture(GL_TEXTURE_2D, tex_heightmap);
	//	glfwLoadTexture2D("_mesh/blacknwhitefbm.tga", 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Noise.rows(),
		Noise.cols(), 0, GL_RGB, GL_FLOAT,
		Noise.data());


	///--- to avoid the current object being polluted
	glBindVertexArray(0);
	glUseProgram(0);
}

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

//camera rotation helpers
void COpenGLControl::RotateX(vec3 *dir, float rot) {
	double cosPhi = (double)cos(rot);
	double sinPhi = (double)sin(rot);
	//need to learn proper way to mult vec3 by matrix
	float xnew = 0;
	xnew += (*dir).x;
	float ynew = 0;
	ynew += (*dir).y * cosPhi;
	ynew += (*dir).z * sinPhi;
	float znew = 0;
	znew += (*dir).y * -sinPhi;
	znew += (*dir).z * cosPhi;

	(*dir).x = xnew; (*dir).y = ynew; (*dir).z = znew;
}


void COpenGLControl::RotateY(vec3 *dir, float rot) {
	float cosPhi = (float)cos(rot);
	float sinPhi = (float)sin(rot);
	float xnew = 0;
	xnew += (*dir).x*cosPhi;
	xnew += (*dir).z * sinPhi;
	float ynew = 0;
	ynew += (*dir).y * 1;
	ynew += (*dir).z * sinPhi;
	float znew = 0;
	znew += (*dir).x * -sinPhi;
	znew += (*dir).z * cosPhi;

	(*dir).x = xnew; (*dir).y = ynew; (*dir).z = znew;
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
		m_fRotX += (float)0.1f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.1f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}

		//rotate the camera and the point it's looking at.
//		RotateX(&dirVec, m_fRotX/100.0f);
//		RotateY(&dirVec, m_fRotY/100.0f);
		RotateX(&eye, m_fRotX / 100.0f);
		RotateY(&eye, m_fRotY / 100.0f);
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
		eye.z += diffY;
		dirVec.z += diffY;

		eye.x += diffX;
		dirVec.x += diffX;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;	
	}
	View = lookAt(eye, dirVec, vec3(0.0f, 1.0f, 0.0f));
	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}


BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
	{
		eye -= normalize(dirVec);	
		dirVec -= normalize(dirVec);
	}
	else
	{
		eye += normalize(dirVec);
		dirVec += normalize(dirVec);
	}
	View = lookAt(eye, dirVec, vec3(0.0f, 1.0f, 0.0f));
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
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
	glClearDepth(1.0f);


	// *** initialize program shaders
	pid = LoadShader("c:\\Users\\Alex\\documents\\Shaders\\vertShader.glsl", 
		"c:\\Users\\Alex\\documents\\Shaders\\fragShader.glsl");
	if (!pid) exit(EXIT_FAILURE);

	m_iTerrainSize = 100;
	m_iRandSeed = 19;

	

	//setup camera
	dirVec = vec3(0.0f, 0.0f, 0.0f);
	eye = vec3(0.0f, 70.0f, 200.0f);
	Projection = perspective(radians(45.0f), 1.0f, 1.0f, 500.0f);
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
	
	if(m_bShowHeightMap)
		glUniform1i(glGetUniformLocation(pid, "showHeightMap"), 1);
	else
		glUniform1i(glGetUniformLocation(pid, "showHeightMap"), 0);
	
	glUniform1f(glGetUniformLocation(pid, "intensity"), m_fIntensity);
	glUniform1f(glGetUniformLocation(pid, "ambientLight"), m_fAmbient);

	float light[3] = { lightPos.x, lightPos.y, lightPos.z };
	glUniform3f(glGetUniformLocation(pid, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_heightmap);

	glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pid, "view"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, &Projection[0][0]);
	
	glBindVertexArray(_vao);

	glDrawArrays(GL_TRIANGLES, 0, triangle_vec.size()); 

	glUseProgram(0);
	glBindVertexArray(0);
}

void COpenGLControl::MakeVertices(int width, int height)
{
	vertices.clear();
	triangle_vec.clear();
	vtexcoord.clear();

	for (int i = -0; i < height; i++)
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
	
	//make the pixel coordinates for the height map texture
	for (std::vector<vec3>::iterator itr = triangle_vec.begin(); itr != triangle_vec.end(); ++itr)
		vtexcoord.push_back(vec2((*itr).x / float(width), (*itr).z / float(height)));
		
}

void COpenGLControl::UpdateGenerator()
{
	generator->m_iPeriod = values.period;
	generator->m_fH = values.H;
	generator->m_fLac = values.lac;
	generator->m_iOctaves = values.octaves;
	generator->m_fOffset = values.offset;

	generator->m_fDistort = values.distortVal;
	generator->m_bDistort = values.distort;
}

void COpenGLControl::ResetCamera()
{
	dirVec = vec3(20.0f, 0.0f, 0.0f);
	eye = vec3(20.0f, 70.0f, 10.0f);
	View = lookAt(eye, dirVec, vec3(0.0f, 1.0f, 0.0f));
}

void COpenGLControl::SetTerrainType(TerrainType type)
{
	switch (type)
	{
	case hills:
		generator->m_bHills = true;
		generator->m_bRidges = false;
		break;
	case ridges:
		generator->m_bRidges = true;
		generator->m_bHills = false;
		break;
	case plain: 
		generator->m_bRidges = false;
		generator->m_bHills = false;
		break;
	default:
		break;
	}
}