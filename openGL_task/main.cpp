#include "pch.h"
#include "CameraManager.h"
#include "mouseLine.h"
#include "Shader.h"



/// 요 아래에 헤더 파일 추가



///------전역변수
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> size{ 0.1f, 0.5f };
std::uniform_real_distribution<float> dis{ -1.f, 1.f };

///------전역변수
CameraManager* g_camera = nullptr;
mouseLine* g_mouseLine = nullptr;
///------ 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);
//	button (버튼 파라미터): GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
//	state(상태 파라미터) : GLUT_UP, GLUT_DOWN
//	x, y : 윈도우에서 마우스의위치
GLvoid TimerFunction(int value);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid mouseWheel(int button, int dir, int x, int y);
GLvoid gameLoop();
///------ 함수
void main(int argc, char** argv)
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1600, 800);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//--- 필요한 헤더파일 include
		//--- 윈도우 출력하고 콜백함수 설정
		//--- glut 초기화
		//--- 디스플레이 모드 설정
		//--- 윈도우의 위치 지정
		//--- 윈도우의 크기 지정
		//--- 윈도우 생성 (윈도우 이름)
		//--- glew 초기화
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
	//glutTimerFunc(60, TimerFunction, 1); //타이머 설정 콜백함수
	glutIdleFunc(gameLoop);
	glutMainLoop();
	//--- 출력 콜백함수의 지정
	//--- 다시 그리기 콜백함수 지정
	//--- 키보드 입력 콜백함수 지정
	//--- 이벤트 처리 시작
}
GLvoid mouseWheel(int button, int dir, int x, int y)
{
	
	glutPostRedisplay();
}

GLvoid gameLoop()
{
	//TODO: 객체의 업데이트 함수 호출
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
	//--- 변경된 배경색 설정
	//--- 화면 지우기(invaildRect)
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
		//TODO: 여기게 객체의 draw함수를 호출하여 그리기
		if (g_mouseLine != nullptr)
		{
			g_mouseLine->draw();
		}
	}
	
	glutSwapBuffers();
}
//타이머
GLvoid TimerFunction(int value)
{

	//타이머마다 1번만 타이머 함수 불러서 다시 불러줘야됨
	


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
