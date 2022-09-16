#type vertex
#version 410 core

layout(location = 0) in vec2 position;
uniform mat4 u_Matrix;

void main()
{
	gl_Position = u_Matrix * vec4(position, 0, 1);
}

#type tessellationControl
#version 410 core

layout(vertices = 4) out;
uniform float u_InnerLevels[2];
uniform float u_OuterLevels[4];

void main()
{
	gl_TessLevelInner[0] = u_InnerLevels[0];
	gl_TessLevelInner[1] = u_InnerLevels[1];

	gl_TessLevelOuter[0] = u_OuterLevels[0];
	gl_TessLevelOuter[1] = u_OuterLevels[1];
	gl_TessLevelOuter[2] = u_OuterLevels[2];
	gl_TessLevelOuter[3] = u_OuterLevels[3];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

#type tessellationEvaluation
#version 410 core

layout(quads, equal_spacing, point_mode) in;

void main()
{
	vec4 v1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 v2 = gl_in[3].gl_Position - gl_in[2].gl_Position;

	vec4 p1 = gl_in[0].gl_Position + gl_TessCoord.x * v1;
	vec4 p2 = gl_in[2].gl_Position + gl_TessCoord.x * v2;

	gl_Position = p1 + gl_TessCoord.y * (p2 - p1);
}

#type fragment
#version 410 core

out vec4 color;

void main()
{
	color = vec4(1, 1, 1, 1);
}