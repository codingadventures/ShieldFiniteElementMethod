#ifndef common_h__
#define common_h__

#define GLM_FORCE_RADIANS
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <thread>

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include "android_native_app_glue.h"
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/spline.hpp>
 
//SOFA
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/vector.h>


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "com.shield.fem", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "com.shield.fem", __VA_ARGS__))
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"com.shield.fem", __VA_ARGS__)


#define VIEWPORT_WIDTH 1280
#define VIEWPORT_HEIGHT 1024
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define VIEWPORT_RATIO (float)VIEWPORT_WIDTH/(float)VIEWPORT_HEIGHT
#define TOTAL_ENEMIES 5
#define INITIAL_POINTER_POSITION glm::vec3(50.0f, 50.0f, -5.0f)

#define CAMERA_OFFSET glm::vec3(0.0f,15.0f,10.0f)


#pragma region [ MODELS ]

#define RAPTOR_MODEL "models\\raptor.obj"
#define RAPTOR_NETGEN_MESH "models\\raptor-8418.mesh"

#pragma endregion [ MODELS ]

namespace Common{


	

	enum GameState
	{
		INTRO,
		GAME,
		PAUSE
	};



	//bool g_keyMappings[1024];
	//bool g_leftMouseButtonIsPressed;
	//bool g_rightMouseButtonIsPressed;

}
 

 






 
#endif // common_h__
