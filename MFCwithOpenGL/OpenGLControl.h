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
#include <iostream>

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

	struct values{
		int period;
		float H;
		float lac;
		int octaves;
		int offset;
		float distortVal;
		bool distort;
	}values;

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
	
	GLuint vertexShader;
	GLuint fragmentShader;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> triangle_vec;
	std::vector<glm::vec2> vtexcoord;  //texture coords for height map. Covers whole mesh
	void MakeVertices(int width, int height);
//	FBMGenerator noiseGenerator;
	FBMGenerator* generator;

	//camera stuff
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;

	glm::vec3 dirVec;
	glm::vec3 eye;
	void RotateX(glm::vec3 *dir, float rot);
	void RotateY(glm::vec3 *dir, float rot);
	
	//user defined controls
	int m_iTerrainSize;
	int m_iRandSeed;

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

	void Generate();
	void SetSize(int size) { m_iTerrainSize = size; }
	void SetRandomSeed(int seed) { generator->m_iRandSeed = seed; }
	void UpdateGenerator(void);
	bool m_bShowHeightMap;

	void ResetCamera();

	float m_fIntensity;
	float m_fAmbient;
	glm::vec3 lightPos;

	enum TerrainType  { hills = 1, ridges, plain };
	void SetTerrainType(TerrainType type);

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	
	//Shaders added to header for portability
	/****************************************************/
	private:

		GLuint loadShaders();

		const char* vshader = R"GLSL( #version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

in vec3 vpoint;
in vec2 TexCoord;
out vec2 TexCoord0;
in vec3 vnormal;


out vec3 fpoint; ///< for debug
out vec3 fnormal_cam;
out float vheight; 
out vec2 uv;



uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D tex_height;
float tex_at(vec2 uv){ return texture(tex_height,uv).r; }

void main()
{
	fpoint = vpoint + .5; ///< For coloring Debug [0,1]^3 
	vheight =  tex_at(TexCoord);
	gl_Position = projection * view * vec4(vpoint.x, vheight, vpoint.z, 1.0f);
	TexCoord0 = TexCoord;
    uv = vec2(fpoint.x, fpoint.z);
})GLSL";

	const char* fshader = R"GLSL(#version 330 core


in vec2 TexCoord0;
out vec3 color;
in vec3 fpoint;
in vec3 fnormal_cam;
uniform float time;
in float vheight;

in vec2 uv;
uniform int showHeightMap;
uniform float ambientLight;
uniform float intensity;

uniform sampler2D tex_height;
uniform vec3 lightPos;
void main()
{

	float dx_p = textureOffset(tex_height, TexCoord0.st, ivec2(+1,0))[0];
	float dx_m = textureOffset(tex_height, TexCoord0.st, ivec2(-1,0))[0];
	float dy_p = textureOffset(tex_height, TexCoord0.st, ivec2(0,+1))[0];
	float dy_m = textureOffset(tex_height, TexCoord0.st, ivec2(0,-1))[0];
	
	vec3 dx = normalize(vec3(1.0,dx_p - dx_m,0.0));
	vec3 dy = normalize(vec3(1.0,dy_p - dy_m,0.0));
	vec3 surfaceNorm = vec3(cross(dx,dy));
	
	vec3 L = lightPos; // vec3(11,40,-11); //light position
	vec3 H = normalize(L + fnormal_cam);

	float light = max(dot(surfaceNorm, normalize(L))*intensity, ambientLight);
	
	
	if(showHeightMap > 0)
		color = texture(tex_height, TexCoord0.st).rgb;
	else
	{
		color = vec3(0.50,0.50,0.5)* light;
	}
	
})GLSL";

};

