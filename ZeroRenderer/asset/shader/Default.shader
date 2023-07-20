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
uniform vec4 u_mixedColor;

void main() {
	vec4 texColor = texture(u_diffuseMap, v_texCoord);
	vec4 outColor = mix(texColor, u_mixedColor, 0.5);
	color = outColor;
}
