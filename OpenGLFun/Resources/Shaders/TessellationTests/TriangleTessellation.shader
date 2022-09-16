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

layout(vertices = 3) out;
uniform float u_InnerLevel;
uniform float u_OuterLevels[3];

void main()
{
	gl_TessLevelInner[0] = u_InnerLevel;
	gl_TessLevelOuter[0] = u_OuterLevels[0];
	gl_TessLevelOuter[1] = u_OuterLevels[1];
	gl_TessLevelOuter[2] = u_OuterLevels[2];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

#type tessellationEvaluation
#version 410 core

layout(triangles, equal_spacing, point_mode) in;

void main()
{
	gl_Position =
		(gl_TessCoord.x * gl_in[0].gl_Position) +
		(gl_TessCoord.y * gl_in[1].gl_Position) +
		(gl_TessCoord.z * gl_in[2].gl_Position);
}

#type fragment
#version 410 core

out vec4 color;

void main()
{
	color = vec4(1, 1, 1, 1);
}