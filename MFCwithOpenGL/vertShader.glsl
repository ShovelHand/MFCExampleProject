#version 330 core
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

	

}