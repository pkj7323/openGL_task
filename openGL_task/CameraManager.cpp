#include "pch.h"
#include "CameraManager.h"


CameraManager::CameraManager()
{
    Position = glm::vec3(0, 5, 10);
    WorldUp = glm::vec3(0, 1, 0);
    Yaw = YAW;
    Pitch = PITCH;
    Zoom = ZOOM;
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    SetPerspective(Zoom, glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100);
    updateCameraVectors();
}
//CameraManager::CameraManager(glm::vec3 Position, glm::vec3 WorldUp, float Yawdegree, float pitchdegree, float zoom,
//    float speed, float sensitivity)
//{
//    this->Position = Position;
//    this->WorldUp = WorldUp;
//    this->Yaw = Yawdegree;
//    this->Pitch = pitchdegree;
//    this->Zoom = zoom;
//    this->MovementSpeed = speed;
//    this->MouseSensitivity = sensitivity;
//    updateCameraVectors();
//}
CameraManager::~CameraManager()
= default;

void CameraManager::CameraYaw(float degreee)
{
    Yaw += degreee;
    updateCameraVectors();
}

void CameraManager::SetPosition(glm::vec3 position)
{
    Position = position;
    updateCameraVectors();
}

void CameraManager::SetCamera(glm::vec3 Position, glm::vec3 WorldUp, float Yawdegree, float pitchdegree, float zoom,
    float speed, float sensitivity)
{
    this->Position = Position;
    this->WorldUp = WorldUp;
    this->Yaw = Yawdegree;
    this->Pitch = pitchdegree;
    this->Zoom = zoom;
    this->MovementSpeed = speed;
    this->MouseSensitivity = sensitivity;
    updateCameraVectors();
}



void CameraManager::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}




glm::mat4 CameraManager::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void CameraManager::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void CameraManager::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void CameraManager::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}



void CameraManager::SetPerspective(float zoom, float _aspect, float _near, float _far)
{
    Zoom = zoom;
    m_aspect = _aspect;
    m_Near = _near;
    m_Far = _far;
}

glm::mat4 CameraManager::GetOrtho()
{
    return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
}

glm::mat4 CameraManager::GetPerspectiveMatrix()
{
    return glm::perspective(glm::radians(Zoom), m_aspect, m_Near, m_Far);
}
