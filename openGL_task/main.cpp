#include "pch.h"

#include "Bar.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "mouseLine.h"
#include "object.h"
#include "Shader.h"
#include "TimeManager.h"


/// �� �Ʒ��� ��� ���� �߰�



///------��������
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> size{ 0.1f, 0.5f };
std::uniform_real_distribution<float> dis{ -1.f, 1.f };

///------��������
CameraManager* g_camera = nullptr;
mouseLine* g_mouseLine = nullptr;
vector<object*> g_objects;
Bar* g_bar;
bool to_init = true;
float timer = 0;
///------ �Լ�

GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);
//	button (��ư �Ķ����): GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
//	state(���� �Ķ����) : GLUT_UP, GLUT_DOWN
//	x, y : �����쿡�� ���콺����ġ
GLvoid TimerFunction(int value);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid mouseWheel(int button, int dir, int x, int y);
GLvoid gameLoop();
void update();
void draw();
void RemoveObject();
///------ �Լ�
void main(int argc, char** argv)
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//--- �ʿ��� ������� include
		//--- ������ ����ϰ� �ݹ��Լ� ����
		//--- glut �ʱ�ȭ
		//--- ���÷��� ��� ����
		//--- �������� ��ġ ����
		//--- �������� ũ�� ����
		//--- ������ ���� (������ �̸�)
		//--- glew �ʱ�ȭ
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW Initialized\n";
	}

	Shader::Instance()->make_ShaderProgram("vertex.glsl", "fragment.glsl");
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);


	{
		g_camera = new CameraManager;
		g_camera->SetCamera(glm::vec3(0, 0, 5),
			glm::vec3(0, 1, 0), YAW, 0.f, 45, SPEED, SENSITIVITY);
		g_camera->SetPerspective(45,
			glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);

	}
	{
		g_bar = new Bar;
		g_objects.push_back(new object);
		CollisionManager::Instance()->add_collision_pair("Bar:Object", g_bar, g_objects[0]);
	}
	glViewport(0, 0, 1600, 800);
	glutDisplayFunc(draw);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(mouseWheel);
	//glutTimerFunc(60, TimerFunction, 1); //Ÿ�̸� ���� �ݹ��Լ�
	glutIdleFunc(gameLoop);
	glutMainLoop();
	//--- ��� �ݹ��Լ��� ����
	//--- �ٽ� �׸��� �ݹ��Լ� ����
	//--- Ű���� �Է� �ݹ��Լ� ����
	//--- �̺�Ʈ ó�� ����
}
GLvoid mouseWheel(int button, int dir, int x, int y)
{
	
	glutPostRedisplay();
}

void update()
{
	timer += DT;
	if (timer >= 1.5f)
	{
		g_objects.push_back(new object);
		CollisionManager::Instance()->add_collision_pair("Bar:Object", nullptr, g_objects.back());
		timer = 0;

	}

	g_bar->update();
	for (auto object : g_objects)
	{
		object->update();
	}
	RemoveObject();
	CollisionManager::Instance()->handle_collision();
}

void draw()
{
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Shader::Instance()->Use();
	

	//��� ��ȯ ������
	{
		//TODO: ����� ��ü�� draw�Լ��� ȣ���Ͽ� �׸���
		if (g_mouseLine != nullptr)
		{
			glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "projection")
				, 1, GL_FALSE
				, glm::value_ptr(glm::mat4(1.0f)));
			glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "view")
				, 1, GL_FALSE
				, glm::value_ptr(glm::mat4(1.0f)));
			g_mouseLine->draw();
		}

	}
	glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "projection")
		, 1, GL_FALSE
		, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "view")
		, 1, GL_FALSE
		, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "world")
		, 1, GL_FALSE
		, glm::value_ptr(glm::mat4(1.0f)));
	{
		for (auto object : g_objects)
		{
			object->draw();
		}
	}
	g_bar->draw();

	glutSwapBuffers();
}

void RemoveObject()
{
	for (auto& obj : g_objects)
	{
		if (obj->is_out())
		{
			g_objects.erase(std::remove(g_objects.begin(), g_objects.end(), obj), g_objects.end());
		}
	}
}

GLvoid gameLoop()
{
	if (to_init)
	{
		TimeManager::Instance()->init();
		to_init = false;
	}
	
	update();
	draw();
	TimeManager::Instance()->update();
}

GLvoid MouseMotion(int x, int y) {

	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);
	math::old_mouse_convert_to_clip(xpos, ypos);
	if (g_mouseLine != nullptr)
	{
		if (g_mouseLine->isClicked())
		{
			
			glm::vec2 pos = glm::vec2(xpos, ypos);
			g_mouseLine->drag(pos.x, pos.y);
		}
	}
	
	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y) {


	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);
	math::old_mouse_convert_to_clip(xpos, ypos);
	glm::vec2 pos = glm::vec2(xpos, ypos);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (g_mouseLine == nullptr)
		{
			g_mouseLine = new mouseLine;
		}
		
		g_mouseLine->click(pos.x, pos.y, pos.x, pos.y);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		
		g_mouseLine->collisionCheck(g_objects);
		if (g_mouseLine != nullptr)
		{
			glm::vec2 realPos = g_mouseLine->getP1();
			glm::vec2 realPos2 = g_mouseLine->getP2();
			cout <<"(" << realPos.x << "," << realPos.y << "), (" << realPos2.x << "," << realPos2.y << ")" << endl;
			delete g_mouseLine;
			g_mouseLine = nullptr;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}

	
	glutPostRedisplay();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

//Ÿ�̸�
GLvoid TimerFunction(int value)
{

	//Ÿ�̸Ӹ��� 1���� Ÿ�̸� �Լ� �ҷ��� �ٽ� �ҷ���ߵ�
	


	glutTimerFunc(60, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'q':
		glutDestroyWindow(glutGetWindow());
		break;
	case 'l':
		for (auto& obj : g_objects)
		{
			obj->SetMode(GL_LINE_STRIP);
		}
		break;
	case 't':
		for (auto& obj : g_objects)
		{
			obj->SetMode(GL_TRIANGLES);
		}
		break;
	case 'o':
		{
			for (auto& obj : g_objects)
			{
				obj->onTrace(true);
			}
		}
		break;
	case 'f':
		{
			for (auto& obj : g_objects)
			{
				obj->onTrace(false);
			}
		}
		break;
	case 's':
	{
		for (auto& obj : g_objects)
		{
			obj->StopMove(true);
		}
	}
	default:
		break;
		//.....
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	default:
		break;
	}
	glutPostRedisplay();
}
