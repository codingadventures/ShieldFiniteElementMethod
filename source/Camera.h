 #ifndef Camera_h__
#define Camera_h__

#include "common.h"
// GL Includes 


/** A Concrete camera class that processes input and calculates the corresponding Euler Angles,
Vectors and Matrices for use in OpenGL
*/
class Camera
{
public:
	Camera();
	Camera(const glm::vec3 &position, 
		const glm::vec3 &up,
		GLfloat yaw, 
		GLfloat pitch);
    
	/** Constructor with scalar values */
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
		GLfloat upX, GLfloat upY, GLfloat upZ,
		GLfloat yaw, GLfloat pitch);

    void setPosition(const glm::vec3 &position);
	glm::vec3 getPosition() const;
    /** Returns the view matrix calculated using Eular Angles and the LookAt Matrix */
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
    /** @brief Processes input received from any keyboard-like input system.
		@details Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	*/
	void ProcessKeyboard(Direction direction, GLfloat deltaTime);

	/** Processes input received from a mouse input system.
		Expects the offset value in both the x and y direction.
	*/
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    /** @brief Processes input received from a mouse scroll-wheel event.
		@details Only requires input on the vertical wheel-axis
	*/
	void ProcessMouseScroll(GLfloat yoffset);

private:
    //!< Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
public:
	glm::mat4 projection;


	//direction vectors
	glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

private:
	// Camera Attributes
	glm::vec3 Position;
};
#endif // Camera_h__