#type vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 color; 
layout(location = 2) in vec2 textureCoordinates;

out vec4 vertexColor;
out vec2 uv;

uniform vec2 u_PositionOffset;
uniform mat4 u_Rotation;
uniform mat4 u_Translation;
uniform mat4 u_Scale;
uniform mat4 u_Projection;

void main()
{
	vec4 position = vec4(aPos, 0, 1);
	vec4 transformed = u_Translation * u_Rotation * u_Scale * position;
	transformed = transformed + vec4(u_PositionOffset, 0, 0);
	gl_Position = u_Projection * transformed;
	vertexColor = vec4(color, 1);
	uv = textureCoordinates;
}

#type fragment
#version 330 core

in vec4 vertexColor;
in vec2 uv;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

void main()
{
	FragColor = u_Color * vertexColor;
	if (u_UseTexture)
		FragColor *= texture(u_Texture, uv);
}