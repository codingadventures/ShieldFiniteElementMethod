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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

enum Direction{FORWARD, BACKWARD, LEFT, RIGHT, NO_DIRECTION/*FORWARD_LEFT, FORWARD_RIGHT, BACKWARD_LEFT, BACKWARD_RIGHT*/};

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

#define EPSILON 1e-6
#define APP_NAME std::string("CPU FEM")
//#define CLEAR_COLOR glm::vec4(0.46f, 0.53f, 0.6f, 1.0f)
#define CLEAR_COLOR glm::vec4(0, 0, 0, 1.0f)
#define CLEAR_MASK (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
#define DEBUG_VERTEX_SHADER_NAME "basicVS.glsl"
#define DEBUG_FRAGMENT_SHADER_NAME "basicFS.glsl"
#define INITIAL_CAMERA_POS glm::vec3(0.0f, 0.0f, 5.0f)
#define MOUSE_MOV_SPEED 0.006f
#define MOUSE_SENSITIVITY 0.25f
#define CAMERA_ZOOM 45.0f
#define CAMERA_FOV  45.0f
#define NEAR_CLIP_PLANE 0.001f
#define FAR_CLIP_PLANE 1000.0f

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#endif // common_h__
