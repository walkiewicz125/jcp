#ifndef JCP_CAM_PROFILE_GENERATOR_HPP
#define JCP_CAM_PROFILE_GENERATOR_HPP

#include "cam_generator_config.hpp"
#include "cam_profile.hpp"

#include <wykobi/wykobi.hpp>

class cam_profile_generator
{
public:
    cam_profile_generator(const cam_generator_config_t& config)
    : config_(config)
    {
    }

    void generate(cam_profile_t& cam_profile, size_t resolution) const
    {
        for (int i = -resolution; i <= int(resolution); i++)
        {
            generate_point(cam_profile, resolution, i);
        }

        cam_profile.cam_pivot_ = config_.cam_pivot_;
        cam_profile.joystick_pivot_ = config_.joystick_pivot_;
    }

private:
    void generate_point(cam_profile_t& cam_profile, size_t resolution, int i) const
    {
        const float displacement_ratio = static_cast<float>(i) / resolution;
        const float joystick_angle = config_.joystick_gimbal_range_ * displacement_ratio;
        const float cam_angle = config_.cam_gimbal_range_  * std::abs(displacement_ratio);

        
        const wykobi::point2d<float> joystick_start_point =  wykobi::make_point(config_.joystick_pivot_.x,
            config_.joystick_pivot_.y + config_.joystick_lever_length_);

        int vector_index = i + resolution;

        wykobi::point2d<float> joystick_end_point = wykobi::rotate(joystick_angle, joystick_start_point, config_.joystick_pivot_);
        joystick_end_point.y = -joystick_end_point.y;
        cam_profile.joystick_points_[vector_index] = joystick_end_point;

        wykobi::point2d<float> cam_contact_point = wykobi::rotate(-cam_angle, joystick_end_point, config_.cam_pivot_);
        cam_contact_point.y = cam_contact_point.y;
        cam_profile.cam_points_[vector_index] = cam_contact_point;
    }

    const cam_generator_config_t& config_;
};

#endif // JCP_CAM_PROFILE_GENERATOR_HPP
