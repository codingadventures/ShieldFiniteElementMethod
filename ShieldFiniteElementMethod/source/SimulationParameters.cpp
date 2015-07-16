#include "SimulationParameters.h"



SimulationParameters::SimulationParameters() : timeStep(0.04), 
	//  odeSolver(ODE_EulerExplicit),
	odeSolver(ODE_EulerImplicit),
	rayleighMass(0.01), 
	rayleighStiffness(0.01),
	maxIter(25), 
	tolerance(1e-3),
	youngModulusTop(100000), 
	youngModulusBottom(1000000), 
	poissonRatio(0.4), 
	massDensity(0.01),
	gravity(0,-20,0), 
	pushForce(75, 20, -15), 
	planeRepulsion(10000),
	sphereRepulsion(0),
	fixedHeight(0.05),
	simulation_time(0)
{

}

void SimulationParameters::Init(TCoord boundingBox[2])
{
	simulation_bbox[0] =boundingBox[0];
	simulation_bbox[1] = boundingBox[1];

	simulation_size = (simulation_bbox[1]-simulation_bbox[0]).norm();
	simulation_center = (simulation_bbox[0] + simulation_bbox[1]) * 0.5f;


	plane_position = simulation_center * 0.5f;
	plane_position[1] = simulation_bbox[0][1];
	plane_size = simulation_size*0.75f;

	sphere_position0[0] = simulation_bbox[1][0] + simulation_size*0.5f;
	sphere_position0[1] = simulation_center[1];
	sphere_position0[2] = simulation_bbox[0][2]*0.33f + simulation_bbox[1][2]*0.67f;
	sphere_position = sphere_position0;
	sphere_velocity[0] = -simulation_size*0.1f;
	sphere_radius = simulation_size*0.05f;
}

void SimulationParameters::Reset()
{
	simulation_mapping_needed = true;
	simulation_time = 0;

	sphere_position = sphere_position0;
}
