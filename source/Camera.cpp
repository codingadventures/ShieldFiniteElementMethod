#include "Camera.h"

#include <vector>

#include "../3rdparty/glm/glm/gtc/matrix_transform.hpp"


Camera::Camera()
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Position = INITIAL_CAMERA_POS;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	MovementSpeed = MOUSE_MOV_SPEED;
	MouseSensitivity = MOUSE_SENSITIVITY;
	Zoom = CAMERA_ZOOM;
	Yaw = -90.0f;
	Pitch = 0.0f;
	this->updateCameraVectors();

}
Camera::Camera(const glm::vec3 &position,
				const glm::vec3 &up, 
				GLfloat yaw
				, GLfloat pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(MOUSE_MOV_SPEED), MouseSensitivity(MOUSE_SENSITIVITY), Zoom(CAMERA_ZOOM),
	Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch)
{

	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.25f), Zoom(45.0f),
	Position(glm::vec3(posX, posY, posZ)), WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch)
{
	this->updateCameraVectors();
}



void Camera::setPosition(const glm::vec3 &position)
{
	Position = position;
	this->updateCameraVectors();

}

glm::vec3 Camera::getPosition() const
{
	return Position;
}

glm::mat4 Camera::GetViewMatrix() const
{
	//glm::mat4 view = glm::lookAt(glm::vec3(0,2, -5), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 view = glm::lookAt(this->Position, this->Position + this->Front, this->WorldUp);
	return view;
}

void Camera::ProcessKeyboard(Direction direction, GLfloat deltaTime)
{
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == Direction::FORWARD)
		this->Position += this->Front * velocity;
	if (direction == Direction::BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == Direction::LEFT)
		this->Position -= this->Right * velocity;
	if (direction == Direction::RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch /*= true*/)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Front, this->Right));
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return projection;
	//return glm::perspective(CAMERA_FOV, (float)1920 / (float)1200, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);
}
