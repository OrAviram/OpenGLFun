#type vertex
#version 410 core

layout(location = 0) in vec2 position;

void main()
{
	gl_Position = /*u_Matrix * */vec4(position, 0, 1);
}

#type tessellationControl
#version 410 core

layout(vertices = 2) out;
uniform float u_LevelX;
uniform float u_LevelY;

void main()
{
	gl_TessLevelOuter[0] = u_LevelY;
	gl_TessLevelOuter[1] = u_LevelX;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

#type tessellationEvaluation
#version 410 core

layout(isolines, equal_spacing, point_mode) in;
uniform mat4 u_Matrix;

void main()
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;

	vec4 v0 = p1 - p0;
	vec4 v1 = vec4(-v0.y, v0.x, v0.z, v0.w);

	gl_Position = p0 + gl_TessCoord.x * v0 + gl_TessCoord.y * v1;
	gl_Position = u_Matrix * gl_Position;
}

#type fragment
#version 410 core

out vec4 color;

void main()
{
	color = vec4(1, 1, 1, 1);
}