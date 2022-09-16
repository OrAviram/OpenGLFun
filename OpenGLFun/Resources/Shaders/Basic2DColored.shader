#type vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 u_Matrix;

void main()
{
	gl_Position = u_Matrix * vec4(position, 0, 1);
}

#type fragment
#version 330 core

uniform vec4 u_Color;
out vec4 color;

void main()
{
	color = u_Color;
}