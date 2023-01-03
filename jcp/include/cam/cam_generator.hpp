#ifndef JCP_CAM_GENERATOR_HPP
#define JCP_CAM_GENERATOR_HPP

#include "cam_generator_config.hpp"
#include "cam_profile_generator.hpp"

#include <wykobi/wykobi.hpp>

class cam_generator
{
public:
    cam_generator(const cam_generator_config_t& config)
    : profile_generator_(config)
    {

    }

    const cam_profile_t& generate_profile(size_t resolution)
    {
        cam_profile_.reserve(resolution);
        profile_generator_.generate(cam_profile_, resolution);
        return cam_profile_;
    }

private:
    cam_profile_t cam_profile_;
    cam_profile_generator profile_generator_;
};

#endif // JCP_CAM_GENERATOR_HPP
