#ifndef FemController_h__
#define FemController_h__

#include "AbstractController.h"  
#include "simulation.h"
#include "Shader.h"
#include "Model.h"

namespace Controller
{ 
	class FemController : public AbstractController
	{
	public: 
		virtual void InitParams() override; 
		virtual void Draw();  
		virtual void Init(android_app *state) override;

		FemController();
		~FemController();
	private:
		int MeshLoad();
		void text_to_screen(); 
		void tweak_bar_setup();
		void setup_current_instance();
	private:

		Shaders::Shader*			d_shader;
		Rendering::Model*			d_model;
		Simulation*					d_simulation;

	};
}

#endif // FemController_h__