/*
* Copyright 2013 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

//----------------------------------------------------------
//  tapCamera.cpp
//  Camera control with tap
//
//----------------------------------------------------------

#define GLM_FORCE_RADIANS

#include <fstream>
#include "tapCamera.h" 
#include <glm/gtx/quaternion.hpp>

namespace ndk_helper
{

	const float TRANSFORM_FACTOR = 15.f;
	const float TRANSFORM_FACTORZ = 10.f;

	const float MOMENTUM_FACTOR_DECREASE = 0.85f;
	const float MOMENTUM_FACTOR_DECREASE_SHIFT = 0.9f;
	const float MOMENTUM_FACTOR = 0.8f;
	const float MOMENTUM_FACTOR_THRESHOLD = 0.001f;

	//----------------------------------------------------------
	//  Ctor
	//----------------------------------------------------------
	TapCamera::TapCamera() :
		dragging_( false ),
		pinching_( false ),
		momentum_( false ),
		ball_radius_( 0.75f ),
		pinch_start_distance_SQ_( 0.f ),
		camera_rotation_( 0.f ),
		camera_rotation_start_( 0.f ),
		camera_rotation_now_( 0.f ),
		momemtum_steps_( 0.f ),
		flip_z_( 0.f )
	{
		//Init offset
		InitParameters();

		vec_flip_ = glm::vec2( 1.f, -1.f );
		flip_z_ = -1.f;
		vec_pinch_transform_factor_ = glm::vec3( 1.f, 1.f, 1.f );

		vec_ball_center_ = glm::vec2( 0, 0 );
		vec_ball_now_ = glm::vec2( 0, 0 );
		vec_ball_down_ = glm::vec2( 0, 0 );

		vec_pinch_start_ = glm::vec2( 0, 0 );
		vec_pinch_start_center_ = glm::vec2( 0, 0 );

		vec_flip_ = glm::vec2( 0, 0 );

	}

	void TapCamera::InitParameters()
	{
		//Init parameters
		vec_offset_ = glm::vec3();
		vec_offset_now_ = glm::vec3();

		quat_ball_rot_ = glm::quat();
		quat_ball_now_ = glm::quat();
		mat_rotation_ = glm::toMat4(quat_ball_now_);
		camera_rotation_ = 0.f;

		vec_drag_delta_ = glm::vec2();
		vec_offset_delta_ = glm::vec3();

		momentum_ = false;

		mat_model_ = glm::translate(glm::mat4(),glm::vec3(0.0f,10.0f, -15.f ));

		/*auto mat =  glm::rotate(glm::mat4(),(float) M_PI / 3.0f, glm::vec3(1,0,0) );
		mat_model_ = mat * mat_model_;*/
	}

	//----------------------------------------------------------
	//  Dtor
	//----------------------------------------------------------
	TapCamera::~TapCamera()
	{

	}

	void TapCamera::Update()
	{
		if( momentum_ )
		{
			float momenttum_steps = momemtum_steps_;

			//Momentum rotation
			glm::vec2 v = vec_drag_delta_;
			BeginDrag( glm::vec2() ); //NOTE:This call reset _VDragDelta
			Drag( v * vec_flip_ );

			//Momentum shift
			vec_offset_ += vec_offset_delta_;

			BallUpdate();
			EndDrag();

			//Decrease deltas
			vec_drag_delta_ = v * MOMENTUM_FACTOR_DECREASE;
			vec_offset_delta_ = vec_offset_delta_ * MOMENTUM_FACTOR_DECREASE_SHIFT;

			//Count steps
			momemtum_steps_ = momenttum_steps * MOMENTUM_FACTOR_DECREASE;
			if( momemtum_steps_ < MOMENTUM_FACTOR_THRESHOLD )
			{
				momentum_ = false;
			}
		}
		else
		{
			vec_drag_delta_ *= MOMENTUM_FACTOR;
			vec_offset_delta_ = vec_offset_delta_ * MOMENTUM_FACTOR;
			BallUpdate();
		}

		glm::vec3 vec = vec_offset_ + vec_offset_now_;
		glm::vec3 vec_tmp( TRANSFORM_FACTOR, -TRANSFORM_FACTOR, TRANSFORM_FACTORZ );

		vec *= vec_tmp * vec_pinch_transform_factor_;

		mat_transform_ = glm::translate(glm::mat4(), vec );
	}

	glm::mat4& TapCamera::GetRotationMatrix()
	{
		return mat_rotation_;
	}

	glm::mat4& TapCamera::GetTransformMatrix()
	{
		return mat_transform_;
	}

	void TapCamera::Reset( const bool bAnimate )
	{
		InitParameters();
		Update();

	}

	//----------------------------------------------------------
	//Drag control
	//----------------------------------------------------------
	void TapCamera::BeginDrag( const glm::vec2& v )
	{
		if( dragging_ )
			EndDrag();

		if( pinching_ )
			EndPinch();

		glm::vec2 vec = v * vec_flip_;
		vec_ball_now_ = vec;
		vec_ball_down_ = vec_ball_now_;
		//LOGI("dragging: %s",dragging_ ? "true" : "false");
		dragging_ = true;
		//LOGI("dragging changed to: %s",dragging_ ? "true" : "false");

		momentum_ = false;
		vec_last_input_ = vec;
		vec_drag_delta_ = glm::vec2();
	}

	void TapCamera::EndDrag()
	{
		quat_ball_down_ = quat_ball_now_;
		quat_ball_rot_ = glm::quat();

		dragging_ = false;
		momentum_ = true;
		momemtum_steps_ = 1.0f;
	}

	void TapCamera::Drag( const glm::vec2& v )
	{
		if( !dragging_ )
			return;

		glm::vec2 vec = v * vec_flip_;
		vec_ball_now_ = vec;

		vec_drag_delta_ = vec_drag_delta_ * MOMENTUM_FACTOR + (vec - vec_last_input_);
		vec_last_input_ = vec;
	}

	//----------------------------------------------------------
	//Pinch controll
	//----------------------------------------------------------
	void TapCamera::BeginPinch( const glm::vec2& v1, const glm::vec2& v2 )
	{
		if( dragging_ )
			EndDrag();

		if( pinching_ )
			EndPinch();

		BeginDrag( glm::vec2() );

		vec_pinch_start_center_ = (v1 + v2) / 2.f;

		glm::vec2 vec = v1 - v2;
		float x_diff;
		float y_diff;
		x_diff = vec.x;
		y_diff = vec.y;

		pinch_start_distance_SQ_ = x_diff * x_diff + y_diff * y_diff;
		camera_rotation_start_ = atan2f( y_diff, x_diff );
		camera_rotation_now_ = 0;

		pinching_ = true;
		momentum_ = false;

		//Init momentum factors
		vec_offset_delta_ = glm::vec3();
	}

	void TapCamera::EndPinch()
	{
		pinching_ = false;
		momentum_ = true;
		momemtum_steps_ = 1.f;
		vec_offset_ += vec_offset_now_;
		camera_rotation_ += camera_rotation_now_;
		vec_offset_now_ = glm::vec3();

		camera_rotation_now_ = 0;

		EndDrag();
	}

	void TapCamera::Pinch( const glm::vec2& v1, const glm::vec2& v2 )
	{
		if( !pinching_ )
			return;

		//Update momentum factor
		vec_offset_last_ = vec_offset_now_;

		float x_diff, y_diff;
		glm::vec2 vec = v1 - v2;
		x_diff = vec.x;
		y_diff = vec.y;

		//vec.Value( x_diff, y_diff );

		float fDistanceSQ = x_diff * x_diff + y_diff * y_diff;

		float f = pinch_start_distance_SQ_ / fDistanceSQ;
		if( f < 1.f )
			f = -1.f / f + 1.0f;
		else
			f = f - 1.f;
		if( isnan( f ) )
			f = 0.f;

		vec = (v1 + v2) / 2.f - vec_pinch_start_center_;
		vec_offset_now_ = glm::vec3( vec, flip_z_ * f );

		//Update momentum factor
		vec_offset_delta_ = vec_offset_delta_ * MOMENTUM_FACTOR
			+ (vec_offset_now_ - vec_offset_last_);

		//
		//Update ration quaternion
		float fRotation = atan2f( y_diff, x_diff );
		camera_rotation_now_ = fRotation - camera_rotation_start_;

		//Trackball rotation
		quat_ball_rot_ = glm::quat( cosf( -camera_rotation_now_ * 0.5f ), 0.f, 0.f, sinf( -camera_rotation_now_ * 0.5f ) );
	}


	glm::mat4 TapCamera::GetViewMatrix()
	{
		const float CAM_X = 0.f;
		const float CAM_Y = 0.f;
		const float CAM_Z = 10.f;

		auto mat_view_ =  glm::lookAt(  glm::vec3( CAM_X, CAM_Y, CAM_Z ),
			glm::vec3( 0.f, 0.f, 0.f ),  glm::vec3( 0.f, 1.f, 0.f ) );


		mat_view_ = GetTransformMatrix() * mat_view_
			* GetRotationMatrix() * mat_model_;

		return mat_view_;

	}

	//----------------------------------------------------------
	//Trackball controll
	//----------------------------------------------------------
	void TapCamera::BallUpdate()
	{
		if( dragging_ )
		{
			glm::vec3 vec_from = PointOnSphere( vec_ball_down_ );
			glm::vec3 vec_to = PointOnSphere( vec_ball_now_ );

			glm::vec3 vec = glm::cross(vec_from, vec_to );
			float w = glm::dot(vec_from, vec_to );

			glm::quat qDrag = glm::quat();
			qDrag.x = vec.x;
			qDrag.y = vec.y;
			qDrag.z = vec.z;
			qDrag.w = w;

			qDrag = qDrag * quat_ball_down_;
			quat_ball_now_ = quat_ball_rot_ * qDrag;
		}
		mat_rotation_ = glm::toMat4(quat_ball_now_);
	}

	glm::vec3 TapCamera::PointOnSphere( glm::vec2& point )
	{
		glm::vec3 ball_mouse;
		float mag;
		glm::vec2 vec = (point - vec_ball_center_) / ball_radius_;
		mag = glm::dot(vec, vec );
		if( mag > 1.f )
		{
			float scale = 1.f / sqrtf( mag );
			vec *= scale;
			ball_mouse = glm::vec3( vec, 0.f );
		}
		else
		{
			ball_mouse = glm::vec3( vec, sqrtf( 1.f - mag ) );
		}
		return ball_mouse;
	}

} //namespace ndkHelper
