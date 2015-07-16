#ifndef Camera_h__
#define Camera_h__


// GL Includes 
#include <glm/gtc/matrix_transform.hpp>
#include "common.h"
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Type{
	THIRD_PERSON,
	FREE_FLY
};

// Beware, brain-compiled code ahead! 
Camera_Type& operator++(Camera_Type& cameraType);

Camera_Type operator++(Camera_Type& cameraType, int);


static const float SPEED_STEP = 0.3f;
// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
namespace Camera
{
	class Camera
	{
	public:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		glm::quat Rotation;
		glm::quat ModelRotation;
		glm::vec3 Offset;
		glm::vec3 Direction;
		// Euler Angles
		GLfloat Yaw;
		GLfloat Pitch;
		// Camera options
		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom; 
		GLboolean HasMoved;
		Camera_Type CameraType;
		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);
		// Constructor with scalar values
		Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	
		// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix();
	
		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	
		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset );
	
		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(GLfloat yoffset);
	
	
		// Moves/alters the camera positions based on user input
		void MoveCamera();
	
		void SetTarget(glm::vec3 target);
	private:
	
		glm::vec3 Target;
		GLfloat deltaTime;
		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
	};
}
#endif // Camera_h__