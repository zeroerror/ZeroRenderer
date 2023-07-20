#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modRotationMatrix;
out vec2 v_texCoord;

void main() {
	gl_Position = u_mvp * u_modRotationMatrix * position;
	v_texCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform vec3 u_mixedColor;
uniform float u_mixedFactor;

void main() {
	vec4 diffuseColor = texture(u_diffuseMap, v_texCoord);
	vec4 specularColor = texture(u_specularMap, v_texCoord);
	vec4 texColor = diffuseColor + specularColor;
	vec4 outColor = texColor + vec4(u_mixedColor, 1.0) * u_mixedFactor;

	color = outColor;
}
