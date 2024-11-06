#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;


uniform mat4 world;//�� ��ü���� ��ȯ
uniform mat4 view;//ī�޶�
uniform mat4 projection;//����

out vec3 outColor;
void main()
{
	gl_Position = projection * view * world * vec4(vPos, 1.0);

	outColor = vColor;
}