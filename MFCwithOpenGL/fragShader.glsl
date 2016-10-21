#version 330 core


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
	
}