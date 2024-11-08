#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;


uniform mat4 world;//각 객체별로 변환
uniform mat4 view;//카메라
uniform mat4 projection;//투영

out vec3 outColor;
void main()
{
	gl_Position = projection * view * world * vec4(vPos, 1.0);

	outColor = vColor;
}