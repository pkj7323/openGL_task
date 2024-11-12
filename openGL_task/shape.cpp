#include "pch.h"
#include "shape.h"
#include "Shader.h"



shape::shape() : VAO{ NULL }, VBO_pos{ NULL }, VBO_color{ NULL }
, EBO{ NULL }, uniformWorld{ NULL }, uniformView{ NULL }, uniformProjection{ NULL }, model{ nullptr }
, mode{ GL_TRIANGLES }
{
	model = new Model2D;
	worldTransform = glm::mat4(1.0f);
	parentTransform = glm::mat4(1.0f);
	scale_factor = glm::vec3(1, 1, 1);
	translate_factor = glm::vec3(0, 0, 0);
	rotate_factor = glm::vec3(0, 0, 0);
	size = glm::vec2(0.25, 0.25);
	center = glm::vec3(0, 0, 0);

	uniformWorld = glGetUniformLocation(Shader::Instance()->GetID(), "world");


	InitBuffer();
}

shape::shape(const string& path) : VAO{ NULL }, VBO_pos{ NULL }, VBO_color{ NULL }
, EBO{ NULL }, uniformWorld{ NULL }, uniformView{ NULL }, uniformProjection{ NULL }, model{ nullptr }
, mode{ GL_TRIANGLES }
{
	model = new Model2D;
	worldTransform = glm::mat4(1.0f);
	parentTransform = glm::mat4(1.0f);
	scale_factor = glm::vec3(1, 1, 1);
	translate_factor = glm::vec3(0, 0, 0);
	rotate_factor = glm::vec3(0, 0, 0);


	uniformWorld = glGetUniformLocation(Shader::Instance()->GetID(), "world");


	InitBuffer();
}

void shape::InitBuffer()
{
	update();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_pos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), model->vertices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->faces.size() * sizeof(glm::uvec3), model->faces.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//각 정점의 데이터 (타입)를 저장
   //void glVertexAttribPointer(
   //	GLuint index,        // 속성 인덱스
   //	GLint size,          // 요소의 개수 (예: 3이면 x, y, z)
   //	GLenum type,         // 각 요소의? 데이터 타입 (예: GL_FLOAT)
   //	GLboolean normalized,// 정규화 여부 (GL_TRUE 또는 GL_FALSE)
   //	GLsizei stride,      // 다음 요소까지의 바이트 간격
   //	const void* pointer  // 버퍼 데이터의 시작 위치
   //);

	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, model->colors.size() * sizeof(glm::vec3), model->colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}


shape::~shape()
{
	
	glDeleteBuffers(1, &VBO_pos);
	glDeleteBuffers(1, &VBO_color);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void shape::UpdateBuffer()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), model->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->faces.size() * sizeof(glm::uvec3), model->faces.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, model->colors.size() * sizeof(glm::vec3), model->colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void shape::update()
{
	glm::mat4 scaleTransform = glm::scale(glm::mat4(1.0f), scale_factor);
	glm::mat4 translateTransform = glm::translate(glm::mat4(1.0f), translate_factor);
	glm::mat4 rotateTransform = glm::rotate(glm::mat4(1.0f), rotate_factor.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateTransform = glm::rotate(rotateTransform, rotate_factor.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateTransform = glm::rotate(rotateTransform, rotate_factor.z, glm::vec3(0.0f, 0.0f, 1.0f));
	worldTransform = translateTransform * rotateTransform * scaleTransform;
	worldTransform = parentTransform * worldTransform;
}

void shape::draw()
{
	//GLuint shaderProgram = Shader::Instance()->GetID();
	glUniformMatrix4fv(uniformWorld, 1, GL_FALSE, glm::value_ptr(worldTransform));
	Shader::Instance()->Use();


	assert(VAO != NULL && "VAO 설정안됨");
	assert(VBO_pos != NULL && "VBO_pos 설정안됨");
	assert(VBO_color != NULL && "VBO_color 설정안됨");
	assert(EBO != NULL && "EBO 설정안됨");

	glBindVertexArray(VAO);
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(get_bb().z, get_bb().y,0);
		glVertex3f(get_bb().x, get_bb().y,0);
		glVertex3f(get_bb().x, get_bb().w,0);
		glVertex3f(get_bb().z, get_bb().w,0);
		glEnd();
	}
	glDrawElements(mode, 3 * model->faces.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void shape::RotateSelf(float degree, glm::vec3 basis)
{
	basis = glm::normalize(basis);
	rotate_factor.x += glm::radians(degree) * basis.x;
	rotate_factor.y += glm::radians(degree) * basis.y;
	rotate_factor.z += glm::radians(degree) * basis.z;
	update();
}

void shape::Rotate(float degree, glm::vec3 basis)
{
	translate_factor = glm::rotate(glm::mat4(1.0f), glm::radians(degree), basis) * glm::vec4(translate_factor, 1.0f);
	update();
}

void shape::Translate(glm::vec3 scalar)
{
	translate_factor.x += scalar.x;
	translate_factor.y += scalar.y;
	translate_factor.z += scalar.z;
	update();
}
void shape::Translate(float x, float y, float z)
{
	Translate(glm::vec3(x, y, z));
}

void shape::Scale(glm::vec3 scale)
{
	translate_factor *= scale;
	scale_factor *= scale;
	update();
}

void shape::ScaleSelf(glm::vec3 scale)
{
	scale_factor *= scale;
	update();
}

void shape::CalculateCenter()
{
	center = glm::vec3(0, 0, 0);
	for (auto vertex : model->vertices)
	{
		center += vertex;
	}
	center /= model->vertices.size();
	
	
}

void shape::CalculateSize()
{
	CalculateCenter();
	size = glm::vec2(0,0);
	for (auto vertex : model->vertices)
	{
		size.x += glm::abs(vertex.x - center.x);
		size.y += glm::abs(vertex.y - center.y);
	}
	size /= model->vertices.size();
	size *= 2;
}

void shape::Move(glm::vec3 target, float scalar)
{


	auto dis = glm::distance(target, translate_factor);
	if (dis == 0) {
		return;
	}
	auto dir = glm::normalize(target - translate_factor) * scalar;

	translate_factor = glm::translate(glm::mat4(1.0f), dir) * glm::vec4(translate_factor, 1.0f);
	update();
}

void shape::Move(float x, float y, float z, float scalar)
{
	Move(glm::vec3(x, y, z), scalar);
}


glm::vec4 shape::get_bb()
{
	CalculateCenter();
	glm::vec4 bb = glm::vec4(
		center.x - size.x / 2.f, //left
		center.y + size.y / 2.f, //top
		center.x + size.x / 2.f, //right
		center.y - size.y / 2.f);//bottom
	
	return bb;
}
