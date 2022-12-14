#type vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_Matrix;

void main()
{
	gl_Position = u_Matrix * vec4(position, 1);
}

#type fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(1, 0, 0, 1);
}