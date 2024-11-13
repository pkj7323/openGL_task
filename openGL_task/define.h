#pragma once
#define SINGLE(type) public:\
			static type* Instance()\
			{\
			static type instance;\
			return &instance;\
			}\
			private:\
			type();\
			~type();
struct Model
{
	Model(const string& filename) {
		if (filename == "NULL")
		{
			return;
		}

		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Error opening file" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string line;
		while (std::getline(file, line)) {
			if (line.substr(0, 2) == "v ") {
				glm::vec3 temp;
				std::istringstream s(line.substr(2));
				s >> temp.x >> temp.y >> temp.z;
				this->vertices.push_back(temp);
			}
			else if (line.substr(0, 2) == "vn")
			{
				glm::vec3 temp;
				std::istringstream s(line.substr(3));
				s >> temp.x >> temp.y >> temp.z;
				this->normals.push_back(temp);
			}
			else if (line.substr(0, 2) == "vt")
			{
				glm::vec3 temp;
				std::istringstream s(line.substr(3));
				s >> temp.x >> temp.y >> temp.z;
				this->texture_coords.push_back(temp);
			}
			else if (line.substr(0, 2) == "f ") {
				std::istringstream s(line.substr(2));
				std::vector<std::string> tokens;
				std::string token;
				while (s >> token) {
					tokens.push_back(token);
				}
				vector<UINT> v;
				vector<UINT> vt;
				vector<UINT> vn;
				for (int i = 0; i < tokens.size(); ++i)
				{


					// 1/2/3 1//3  1
					if (tokens[i].find("/") == std::string::npos)
					{
						UINT v1;
						std::stringstream ss(tokens[i]);
						ss >> v1;
						v.push_back(v1);
						continue;
					}
					else
					{
						tokens[i].replace(tokens[i].find("/"), 1, " ");
						std::stringstream ss(tokens[i]);

						if (tokens[i].find("  ") != std::string::npos)
						{
							UINT v1, vn1;
							ss >> v1 >> vn1;
							v.push_back(v1);
							vn.push_back(vn1);
						}
						else if (tokens[i].find(" ") != std::string::npos)
						{
							UINT v1, vn1, vt1;
							ss >> v1 >> vt1 >> vn1;
							v.push_back(v1);
							vt.push_back(vt1);
							vn.push_back(vn1);
						}
						else
						{
							UINT v1;
							ss >> v1;
							v.push_back(v1);
						}
					}

				}
				if (v.size() > 0)
				{
					this->faces.emplace_back(v[0] - 1, v[1] - 1, v[2] - 1);
				}
				if (vn.size() > 0)
				{
					this->normal_faces.emplace_back(vn[0] - 1, vn[1] - 1, vn[2] - 1);
				}
				if (vt.size() > 0)
				{
					this->texture_faces.emplace_back(vt[0] - 1, vt[1] - 1, vt[2] - 1);
				}

			}
		}
		file.close();
	}
	vector<glm::vec3>	vertices;
	vector<glm::vec3>	colors;
	vector<glm::vec3>	normals;
	vector<glm::vec3>	texture_coords;
	vector<glm::uvec3>	faces;
	vector<glm::uvec3>	normal_faces;
	vector<glm::uvec3>	texture_faces;
};
namespace math
{
	static float CCW(glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		return (c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x);
	}
	static bool intersect(glm::vec2 lineStart, glm::vec2 lineEnd, glm::vec2 p1, glm::vec2 p2)
	{
		return CCW(lineStart, p1, p2) * CCW(lineEnd, p1, p2) <= 0
		&& CCW(lineStart, lineEnd, p1) * CCW(lineStart, lineEnd, p2) <= 0;
	}
	static void mouse_convert_to_clip(float& x, float& y)
	{
		glm::mat4 windowmatrix = glm::mat4(1.0f);
		windowmatrix[1][1] = -1;
		windowmatrix = glm::scale(windowmatrix, glm::vec3(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, 1));
		windowmatrix = glm::translate(windowmatrix, glm::vec3(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, 0));
		auto clipMatrix = glm::inverse(windowmatrix);
		glm::vec4 clipPos = clipMatrix * glm::vec4(x, y, 0.0f, 1.0f);
		x = clipPos.x;
		y = clipPos.y;
	}
	static void old_mouse_convert_to_clip(float& x, float& y)
	{
		auto width = glutGet(GLUT_WINDOW_WIDTH);
		auto height = glutGet(GLUT_WINDOW_HEIGHT);
		x = (x - width / 2.f) / glm::abs(width / 2.f);
		y = (y - height / 2.f) / glm::abs(height / 2.f);
		y = -y;
	}
}
struct Model2D {
	vector<glm::vec3> vertices;
	vector<glm::vec3> colors;
	vector<glm::uvec3> faces;
};
struct intersection
{
	intersection(glm::vec2 point, UINT index1, UINT index2) : point(point), index1(index1), index2(index2) {}
	glm::vec2 point;
	UINT index1;
	UINT index2;
};

#define Gravity 9.8f
#define DT TimeManager::Instance()->GetDeltaTime()
#define PI 3.14159265358979323846
