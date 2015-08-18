#ifndef Controller_h__
#define Controller_h__

#define GLM_FORCE_RADIANS

#include "common.h" 
#include "Camera.h" 
#include <cudaHelper/helper_timer.h>
#include "Timer.h"
#include "tapCamera.h"
#include "gestureDetector.h"

namespace Controller
{

	/**
	* Our saved state data.
	*/
	struct saved_state {
		float angle;
		int32_t x;
		int32_t y;
	};

	//using namespace placeholders;
	//namespace Cam = Camera;

	class AbstractController
	{
	public:
		android_app*	m_App;
		int				m_animating;

		EGLDisplay		m_display;
		EGLSurface		m_surface;
		EGLContext		m_context;
		bool			m_screenPressed;
		void*			m_clockText;
		bool			m_screenReleased;
		ASensorManager* m_sensorManager;
		const ASensor* m_accelerometerSensor;
		ASensorEventQueue* m_sensorEventQueue;
		struct saved_state m_state;
		glm::vec4 m_clearColor;
		GLbitfield m_clearMask;
		int m_screenWidth, m_screenHeight;
		float m_aspectRatio;

		ndk_helper::DoubletapDetector doubletap_detector_;
		ndk_helper::PinchDetector pinch_detector_;
		ndk_helper::DragDetector drag_detector_;
		ndk_helper::PerfMonitor monitor_;

		Timer				m_timer;
	protected: 
		double				d_delta_time; 
		double				d_delta_time_secs;
		double				d_old_time_since_start;
		double				d_global_clock;
		double				d_time_at_reset;
		bool				d_pause;
		int					d_frame_count;
		glm::mat4			d_projection_matrix;
		glm::mat4 			d_view_matrix;
		float				d_fps;
		ndk_helper::TapCamera			d_camera;
		//Cam::Camera*		d_camera;						//Freed in destructor
		std::string				d_window_name;

	public:
		explicit AbstractController(std::string window_name);
		virtual ~AbstractController();

		virtual void Init(android_app *state);
		virtual void Run();
		virtual void Draw() = 0;
		virtual void InitParams() = 0;

		int			 initEGL();
		/*	virtual void mainLoop();
		virtual void handleInput();*/

		 ndk_helper::TapCamera& GetCamera() {
			return d_camera;
		}

		/**
		* Tear down the EGL context currently associated with the display.
		*/
		void termDisplay();
		void printInfoPath();
		void printGLContextInfo();
		void setAssetManager();

		static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
		static void engine_handle_cmd(struct android_app* app, int32_t cmd);	  
		void TransformPosition(glm::vec2& v);
	/*	void updateTimer( );
		void calculateFps( );*/
	};



	///**
	//* Process the next input event.
	//*/
	//static 

	///**
	//* Process the next main command.
	//*/
	//static 
}


#endif // Controller_h__
