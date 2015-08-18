#include "FemController.h"
#include "common.h"
#include "Line.h"

using namespace Rendering;

namespace Controller
{ 
	FemController::FemController() : AbstractController("Fem GEM CPU")
	{

	}


	FemController::~FemController()
	{
		LOGI("Disposing FEM Controller");
		delete d_shader;
		delete d_simulation;
		//delete d_model;
	}


	int FemController::MeshLoad()
	{

		LOGI("Load meshes");
		if (!d_simulation->simulation_load_fem_mesh(RAPTOR_NETGEN_MESH))
		{
			//return 1;
		}
		/*	for (unsigned int a = 0; a < render_filenames.size(); ++a)
		d_simulation->simulation_load_render_mesh(render_filenames[a].c_str());*/
		/*	if (reorder)
		d_simulation->simulation_reorder_fem_mesh();*/

		LOGI("Init simulation");
		if (!d_simulation->simulation_init())
			return 1;
		return 0;
	}

	void FemController::Init(android_app *state)
	{
		AbstractController::Init(state);

		auto path = std::string(state->activity->internalDataPath);
		d_simulation = new Simulation(path,0,true);
	}
	void FemController::InitParams()
	{  
		d_model = new Model(RAPTOR_MODEL);

		vector<Mesh>* meshes = d_model->GetMeshes( );

		d_simulation->SetMeshes(meshes);

		d_floor	    = new Model(FLOOR_MODEL);
		d_floor->Translate(glm::vec3(0.0f,0.4f,0.0f));
		 
		

		if (!d_simulation->simulation_preload())
		{
			LOGE("Error starting the simulation...");
		}	

		if (MeshLoad())
		{
			LOGE("Error initializing the meshes...");

		} 

		vector<string> v_shader;
		v_shader.push_back("vertex.vert");
		v_shader.push_back("common.vert");
		vector<string> f_shader;
		f_shader.push_back("fragment.frag");
		f_shader.push_back("common.frag");
		d_shader = new Shader(v_shader,f_shader);

	 
		glEnable( GL_DEPTH_TEST ); 

	 
		d_model->Rotate(glm::vec3(0,0,1),glm::radians(180.0f));
	}

	void FemController::Draw()
	{
		glm::mat4 projection_view;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		d_simulation->simulation_animate();

		d_simulation->simulation_mapping(); // make sure the surfaces are up-to-date


		d_projection_matrix = glm::perspective(30.0f, m_screenWidth / (m_screenHeight * 1.0f), 0.1f, 1000.0f);  

		//d_camera->MoveCamera();

		d_camera.Update();

		d_view_matrix = d_camera.GetViewMatrix(); 



		projection_view = d_projection_matrix * d_view_matrix;  

		d_shader->Use();
		d_shader->SetUniform("mvp", projection_view * d_model->GetModelMatrix());
		d_shader->SetUniform("mv",   d_view_matrix * d_model->GetModelMatrix());
		d_shader->SetUniform("model_matrix", d_model->GetModelMatrix());
		d_shader->SetUniform("model_transpose_inverse",  glm::transpose(glm::inverse(d_model->GetModelMatrix())));  

		d_model->Draw(*d_shader);

		if (d_frame_count % 200 ==0 )
		{
			float x = rand() % 100 - 200;
			float y = rand() % 100 - 200;
			float z = rand() % 100 - 200;
 			d_simulation->fem_mesh->setPushRandomForce(TDeriv(x,y,z));
			//cout << "FPS: " << d_fps << endl;
		}
		if (d_simulation->fem_mesh->externalForce.index != -1)
		{
			TCoord p1 = d_simulation->fem_mesh->positions[d_simulation->fem_mesh->externalForce.index];
			TCoord p2 = p1 + d_simulation->fem_mesh->externalForce.value * 0.001;
			auto v1 = Vertex(glm::vec3(p1.x(),p1.y(),p1.z()), glm::vec4(1,0,0,0));
			auto v2 = Vertex(glm::vec3(p2.x(),p2.y(),p2.z()), glm::vec4(1,0,0,0));

			Line l(v1,v2);
			glLineWidth(3);
			l.Draw();
			glLineWidth(1);
		}

		d_shader->Use();
		d_shader->SetUniform("mvp", projection_view * d_floor->GetModelMatrix());
		d_shader->SetUniform("mv",   d_view_matrix * d_floor->GetModelMatrix());
		d_shader->SetUniform("model_matrix", d_floor->GetModelMatrix());
		d_shader->SetUniform("model_transpose_inverse",  glm::transpose(glm::inverse(d_floor->GetModelMatrix())));  
		d_floor->Draw(*d_shader);

		if (d_frame_count >= 1500)
			ANativeActivity_finish(m_App->activity);
	}


}