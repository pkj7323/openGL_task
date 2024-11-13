#pragma once


class shape
{
protected:
	void InitBuffer();//ó�� ���� ����, Matrix ������Ʈ ȣ��


public:
	shape();
	shape(const string& path);
	virtual ~shape();
	void UpdateBuffer();//���� ��ġ�� �ٲٷ��� matrix �ٲ㼭 ȣ�� �� �Լ��� �Ƹ� ȣ���� �� ������

	virtual void update();//������� ���
	virtual void draw();//uniform ���� ����, �׸���

	void SetParentTransform(glm::mat4 matrix) { this->parentTransform = matrix; }
	glm::mat4 GetParentTransform() { return this->parentTransform; }


	void RotateSelf(float theta, glm::vec3 basis);
	void Rotate(float degree, glm::vec3 basis);
	void Translate(glm::vec3 direction);
	void Translate(float x, float y, float z);
	void Scale(glm::vec3 scale);
	void ScaleSelf(glm::vec3 scale);

	void CalculateCenter();
	void CalculateSize();
	void Move(glm::vec3 target, float scalar);
	void Move(float x, float y, float z, float scalar);

	virtual void handle_collision(const string& group, shape* other) = 0;

	glm::vec4 get_bb();
	

	//seter, geter
	void set_size(glm::vec2 size) { this->size = size; }
	glm::vec2 get_size() { return size; }

	GLuint GetVAO() { return VAO; }

	void SetWorldTrans(glm::mat4 worldTrans) { this->worldTransform = worldTrans; }
	glm::mat4 GetWorldTrans() { return worldTransform; }


	Model2D* GetModel() { return model; }
	void SetModel(Model2D* model) { this->model = model; }


	void SetScale(glm::vec3 scale) { this->scale_factor = scale; }
	glm::vec3& GetScale() { return this->scale_factor; }

	void SetTranslate(glm::vec3 translate) { this->translate_factor = translate; }
	glm::vec3& GetTranslate() { return this->translate_factor; }

	void SetRotate(glm::vec3 rotate) { this->rotate_factor = rotate; }
	glm::vec3& GetRotate() { return this->rotate_factor; }

	//GL_TRIANGLES, GL_LINE_STRIP, GL_LINE_LOOP ��
	void SetMode(unsigned int mode) { this->mode = mode; }
	unsigned int GetMode() const { return this->mode; }

	GLint GetUniformWorld() const { return uniformWorld; }
	GLint GetUniformView() const { return uniformView; }
	GLint GetUniformProjection() const { return uniformProjection; }

	void SetOffset(glm::vec2 offset) { this->m_offset = offset; }
	glm::vec2 GetOffset() { return m_offset; }
private:
	GLuint VAO;
	GLuint VBO_pos;
	GLuint VBO_color;
	GLuint EBO;

	GLint uniformWorld;
	GLint uniformView;
	GLint uniformProjection;

	Model2D* model;

	unsigned int mode;

	glm::mat4 worldTransform;
	glm::mat4 parentTransform;
	glm::vec3 scale_factor;
	glm::vec3 translate_factor;
	glm::vec3 rotate_factor;

	glm::vec3 center;
	glm::vec2 size;

	glm::vec2 m_offset;
};

