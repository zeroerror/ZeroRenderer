#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModRotationMatrix;

void main() {
	vec4 finalPos = u_MVP * u_ModRotationMatrix * position;
	finalPos.z = 0;

	gl_Position = finalPos;
	v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_BlendColor;
uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = mix(texColor, u_BlendColor, 0.618f);
}