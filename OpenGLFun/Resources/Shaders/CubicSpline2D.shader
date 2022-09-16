#type vertex
#version 410 core

layout(location = 0) in vec2 controlPosition;

void main()
{
	gl_Position = vec4(controlPosition, 0, 1);
}

#type tessellationEvaluation
#version 410 core

layout(isolines, equal_spacing) in;
uniform mat4 u_Projection;
uniform mat4 u_SplineMatrix;

void main()
{
	mat4x4 geometry;
	geometry[0] = gl_in[0].gl_Position;
	geometry[1] = gl_in[1].gl_Position;
	geometry[2] = gl_in[2].gl_Position;
	geometry[3] = gl_in[3].gl_Position;

	float t = gl_TessCoord.x;
	float t2 = t * t;
	vec4 basis = vec4(1, t, t2, t2 * t);

	gl_Position = u_Projection * geometry * u_SplineMatrix * basis;
}

#type fragment
#version 410 core

out vec4 color;

void main()
{
	color = vec4(1, 1, 1, 1);
}