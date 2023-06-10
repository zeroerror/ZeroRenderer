#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModRotationMatrix;
uniform vec3 u_ModPosition;

void main() {
	gl_Position = u_MVP * u_ModRotationMatrix * position;
	gl_Position = u_ModRotationMatrix * position;
	v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_BlendColor;
uniform sampler2D u_Texture;

void main() {
	color = vec4(v_TexCoord, 1.0f, 1.0f);
}