#ifndef JCP_CAM_PROFILE_GENERATOR_HPP
#define JCP_CAM_PROFILE_GENERATOR_HPP

#include "cam_generator_config.hpp"
#include "cam_profile.hpp"

#include <wykobi/wykobi.hpp>

class profiles_generator
{
public:
    assembly_profiles_t generate(const cam_generator_config_t& config) const
    {
        profile_t cam_path(config.resolution_);
        profile_t joystick_path(config.resolution_);

        profile_t::iterator cam_profile_point = cam_path.begin();
        profile_t::iterator joystick_point = joystick_path.begin();

        for (int step = -config.resolution_; step <= int(config.resolution_); step++)
        {
            generate_point(*cam_profile_point, *joystick_point, config, step);
            
        }
        return assembly_profiles_t(cam_path, joystick_path, config.cam_pivot_, config.joystick_pivot_);
    }

private:
    void generate_point(point_t& cam_profile_point, 
                        point_t& joystick_point, 
                        const cam_generator_config_t& config, 
                        const int step) const
    {
        const float displacement_ratio = static_cast<float>(step) / config.resolution_;
        const float joystick_angle = config.joystick_gimbal_range_ * displacement_ratio;
        const float cam_angle = config.cam_gimbal_range_  * std::abs(displacement_ratio);

        
        const wykobi::point2d<float> joystick_start_point =  wykobi::make_point(config.joystick_pivot_.x,
            config.joystick_pivot_.y + config.joystick_lever_length_);

        int vector_index = step + config.resolution_;

        wykobi::point2d<float> joystick_end_point = wykobi::rotate(joystick_angle, joystick_start_point, config.joystick_pivot_);
        joystick_end_point.y = -joystick_end_point.y;
        cam_profile_point = joystick_end_point;

        wykobi::point2d<float> cam_contact_point = wykobi::rotate(-cam_angle, joystick_end_point, config.cam_pivot_);
        cam_contact_point.y = cam_contact_point.y;
        cam_profile_point = cam_contact_point;
    }
};

#endif // JCP_CAM_PROFILE_GENERATOR_HPP
