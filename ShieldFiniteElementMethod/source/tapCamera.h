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

#pragma once
#include <vector>
#include <string> 
#include "common.h"
//#include "JNIHelper.h"
#include "vecmath.h"
#include "interpolator.h"

namespace ndk_helper
{

/******************************************************************
 * Camera control helper class with a tap gesture
 * This class is mainly used for 3D space camera control in samples.
 *
 */
class TapCamera
{
private:
    //Trackball
    glm::vec2 vec_ball_center_;
    float ball_radius_;
    glm::quat quat_ball_now_;
    glm::quat quat_ball_down_;
    glm::vec2 vec_ball_now_;
    glm::vec2 vec_ball_down_;
    glm::quat quat_ball_rot_;

    bool dragging_;
    bool pinching_;

    //Pinch related info
    glm::vec2 vec_pinch_start_;
    glm::vec2 vec_pinch_start_center_;
    float pinch_start_distance_SQ_;

    //Camera shift
    glm::vec3 vec_offset_;
    glm::vec3 vec_offset_now_;

    //Camera Rotation
    float camera_rotation_;
    float camera_rotation_start_;
    float camera_rotation_now_;

    //Momentum support
    bool momentum_;
    glm::vec2 vec_drag_delta_;
    glm::vec2 vec_last_input_;
    glm::vec3 vec_offset_last_;
    glm::vec3 vec_offset_delta_;
    float momemtum_steps_;

    glm::vec2 vec_flip_;
    float flip_z_;

    glm::mat4 mat_rotation_;
    glm::mat4 mat_transform_;

    glm::vec3 vec_pinch_transform_factor_;
	glm::mat4 mat_model_;
    glm::vec3 PointOnSphere( glm::vec2& point );
    void BallUpdate();
    void InitParameters();
public:
    TapCamera();
    virtual ~TapCamera();
    void BeginDrag( const glm::vec2& vec );
    void EndDrag();
    void Drag( const glm::vec2& vec );
    void Update();

    glm::mat4& GetRotationMatrix();
    glm::mat4& GetTransformMatrix();

    void BeginPinch( const glm::vec2& v1, const glm::vec2& v2 );
    void EndPinch();
    void Pinch( const glm::vec2& v1, const glm::vec2& v2 );

	glm::mat4 GetViewMatrix();


    void SetFlip( const float x, const float y, const float z )
    {
        vec_flip_ = glm::vec2( x, y );
        flip_z_ = z;
    }

    void SetPinchTransformFactor( const float x, const float y, const float z )
    {
        vec_pinch_transform_factor_ = glm::vec3( x, y, z );
    }

    void Reset( const bool bAnimate );

};

} //namespace ndkHelper
