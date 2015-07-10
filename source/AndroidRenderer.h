#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include "common.h"
#include "Renderer.h"

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
class AndroidRenderer
: public Renderer{
public:
	AndroidRenderer();
	~AndroidRenderer();
    struct android_app* m_App;

    ASensorManager* m_sensorManager;
    const ASensor* m_accelerometerSensor;
    ASensorEventQueue* m_sensorEventQueue;

    int m_animating;
    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLContext m_context;
	bool m_screenPressed;
	struct saved_state m_state;

	void *m_clockText;
	bool m_screenReleased;
	void (*m_applicationInitCallback)(void);

public:
	void init(android_app *state,void (*applicationInitCallback)(void));

	int  initEGL();
	virtual void mainLoop();
	virtual void handleInput();

	/**
	 * Tear down the EGL context currently associated with the display.
	 */
	void termDisplay();
	void printInfoPath();
	void printGLContextInfo();
	void setAssetManager();
private:
};




/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd);


#endif