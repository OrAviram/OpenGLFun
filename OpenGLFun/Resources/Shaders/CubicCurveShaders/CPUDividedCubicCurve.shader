#type vertex
#version 410 core

layout(location = 0) in float t;

uniform vec2 u_Control1;
uniform vec2 u_Control2;
uniform vec2 u_Control3;
uniform vec2 u_Control4;

uniform mat4 u_Transform;

void main()
{
	mat4 spline = mat4(
		1, 0, 0, 0,
		-3, 3, 0, 0,
		3, -6, 3, 0,
		-1, 3, -3, 1
	);

	mat4x2 geometry;
	geometry[0] = u_Control1;
	geometry[1] = u_Control2;
	geometry[2] = u_Control3;
	geometry[3] = u_Control4;

	vec4 basis = vec4(1, t, t * t, t * t * t);
	gl_Position = u_Transform * vec4(geometry * spline * basis, 0, 1);
}

#type fragment
#version 410 core

out vec4 color;

void main()
{
	color = vec4(1, 0, 0, 1);
}