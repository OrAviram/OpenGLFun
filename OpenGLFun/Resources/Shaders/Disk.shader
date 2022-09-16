#type vertex
#version 410

layout(location = 0) in vec2 position;
layout(location = 1) in float radiusIn;
layout(location = 2) in vec4 colorIn;

layout(location = 0) out float radiusOut;
layout(location = 1) out vec4 colorOut;

void main()
{
	gl_Position = vec4(position, 0, 1);
	radiusOut = radiusIn;
	colorOut = colorIn;
}

#type geometry
#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) in float radiuses[1];
layout(location = 1) in vec4 colors[1];

layout(location = 0) out vec2 localPosition;
layout(location = 1) out float sqrRadius;
layout(location = 2) out vec4 color;

uniform mat4 u_Projection;

void main()
{
	float radius = radiuses[0];
	sqrRadius = radius * radius;
	color = colors[0];

	vec2 inPos = gl_in[0].gl_Position.xy;

	vec2 localPositions[4];
	localPositions[0] = vec2(-radius, -radius);
	localPositions[1] = vec2(radius, -radius);
	localPositions[2] = vec2(-radius, radius);
	localPositions[3] = vec2(radius, radius);

	for (int i = 0; i < 4; i++)
	{
		localPosition = localPositions[i];
		gl_Position = u_Projection * vec4(localPosition + inPos, 0, 1);
		EmitVertex();
	}
}

#type fragment
#version 410

layout(location = 0) in vec2 localPosition;
layout(location = 1) in float sqrRadius;
layout(location = 2) in vec4 color;

out vec4 outColor;

void main()
{
	if (dot(localPosition, localPosition) > sqrRadius)
		discard;

	outColor = color;
}