#pragma once
#include "afxwin.h"

#include "GLShader.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>

#include <glm/glm/vec3.hpp> // glm::vec3
#include <glm/glm/vec4.hpp> // glm::vec4
#include <glm/glm/mat4x4.hpp> // glm::mat4
#include <glm/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/glm/gtc/type_ptr.hpp>
#include "FBMGenerator.h"

#define FBM_SIZE 512
class COpenGLControl : public CWnd
{
public:
	/******************/
	/* Public Members */
	/******************/
	UINT_PTR m_unpTimer;
	// View information variables
	float	 m_fLastX;
	float	 m_fLastY;
	float	 m_fPosX;
	float	 m_fPosY;
	float	 m_fZoom;
	float	 m_fRotX;
	float	 m_fRotY;
	bool	 m_bIsMaximized;

private:
	/*******************/
	/* Private Members */
	/*******************/
	// Window information
	CWnd  *hWnd;
	HDC   hdc;
	HGLRC hrc;
	int   m_nPixelFormat;
	CRect m_rect;
	CRect m_oldWindow;
	CRect m_originalRect;

	//OpenGL stuff
	GLuint pid; //program id
	GLuint _vao; ///< vertex array object
	GLuint _vbo_vpoint; ///< memory buffer
	GLuint vpoint_id;
	GLuint normalbuffer;

	GLuint vbo_vtexcoord; ///< memory buffer
	GLuint tex_heightmap;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> triangle_vec;
	std::vector<glm::vec2> vtexcoord;  //texture coords for height map. Covers whole mesh
	void MakeVertices(int width, int height);
//	FBMGenerator noiseGenerator;

	//camera stuff
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;

	glm::vec3 dirVec;
	glm::vec3 eye;
	void RotateX(glm::vec3 *dir, float rot);
	void RotateY(glm::vec3 *dir, float rot);

public:
	COpenGLControl(void);
	virtual ~COpenGLControl(void);

	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);
	void oglDrawScene(void);

	// Added message classes:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg	void OnDraw(CDC *pDC);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);



	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

