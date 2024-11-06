#include "pch.h"
#include "CameraManager.h"
#include "mouseLine.h"
#include "Shader.h"



/// �� �Ʒ��� ��� ���� �߰�



///------��������
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> size{ 0.1f, 0.5f };
std::uniform_real_distribution<float> dis{ -1.f, 1.f };

///------��������
CameraManager* g_camera = nullptr;
mouseLine* g_mouseLine = nullptr;
///------ �Լ�
GLvoid drawScene(GLvoid);
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
///------ �Լ�
void main(int argc, char** argv)
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1600, 800);
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);


	{
		g_camera = new CameraManager;
		g_camera->SetCamera(glm::vec3(0, 5, 10),
			glm::vec3(0, 1, 0), YAW, -10.f, 45, SPEED, SENSITIVITY);

	}
	{
		
	}

	glutDisplayFunc(drawScene);
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

GLvoid gameLoop()
{
	//TODO: ��ü�� ������Ʈ �Լ� ȣ��
	glutPostRedisplay();
}

GLvoid MouseMotion(int x, int y) {
	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);
	if (g_mouseLine != nullptr)
	{
		if (g_mouseLine->isClicked())
		{
			math::mouse_convert_to_clip(xpos, ypos);
			g_mouseLine->drag(xpos, ypos);
		}
	}
	
	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y) {


	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);
	math::mouse_convert_to_clip(xpos, ypos);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		g_mouseLine->click(xpos, ypos, xpos, ypos);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		

	}

	
	glutPostRedisplay();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}
GLvoid drawScene()
{
	//--- ����� ���� ����
	//--- ȭ�� �����(invaildRect)
	glViewport(0, 0, 800, 800);
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::Instance()->Use();
	/*glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "projection")
		, 1, GL_FALSE
		, glm::value_ptr(g_camera->GetPerspectiveMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "view")
		, 1, GL_FALSE
		, glm::value_ptr(g_camera->GetViewMatrix()));*/
	{
		//TODO: ����� ��ü�� draw�Լ��� ȣ���Ͽ� �׸���
		if (g_mouseLine != nullptr)
		{
			g_mouseLine->draw();
		}
	}
	
	glutSwapBuffers();
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
