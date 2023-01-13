#ifndef JCP_CAM_GENERATOR_CONFIG_HPP
#define JCP_CAM_GENERATOR_CONFIG_HPP

#include <wykobi/wykobi.hpp>

using point_t = wykobi::point2d<float>;

struct cam_generator_config_t
{
    float cam_gimbal_range_;
    point_t cam_pivot_;
    // TODO: wykobi::point2d<float> cam_end_point_;

    point_t joystick_pivot_;
    float joystick_lever_length_;
    float joystick_gimbal_range_;

    size_t resolution_;

    static cam_generator_config_t get_default()
    {
        cam_generator_config_t conf = {};
        conf.cam_gimbal_range_ = 20.0f;
        conf.cam_pivot_ = wykobi::make_point<float>(120.0f, -105.0f);
        conf.joystick_gimbal_range_ = 30.0f;
        conf.joystick_lever_length_ = 100.0f;
        conf.joystick_pivot_ = wykobi::make_point<float>(0.0f, 0.0f);
        conf.resolution_ = 100;
        return conf;
    }
};

#endif // JCP_CAM_GENERATOR_CONFIG_HPP
