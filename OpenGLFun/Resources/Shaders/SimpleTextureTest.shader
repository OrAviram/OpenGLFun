#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 u_Projection;
uniform mat4 u_Model;

out vec2 uv;

void main()
{
	gl_Position = u_Projection * u_Model * vec4(position, 1);
	uv = texCoords;
}

#type fragment
#version 330 core

in vec2 uv;
out vec4 color;
uniform sampler2D u_Texture;

void main()
{
	color = texture2D(u_Texture, uv);
}