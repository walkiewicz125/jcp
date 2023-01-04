#ifndef JCP_CAM_PROFILE_HPP
#define JCP_CAM_PROFILE_HPP

#include "cam_generator_config.hpp"

#include <wykobi/wykobi.hpp>
#include <vector>

using point_t = wykobi::point2d<float>;

class profile_t
{
public:
    using iterator = std::vector<point_t>::iterator;
    using const_iterator = std::vector<point_t>::const_iterator;
    using reverse_iterator = std::vector<point_t>::reverse_iterator;
    using const_reverse_iterator = std::vector<point_t>::const_reverse_iterator;

    profile_t(size_t resolution)
    : points_(resolution + 1)
    {
    }
    profile_t(const profile_t& other) = default;
    profile_t(profile_t&& other) = default;
    profile_t& operator=(profile_t&& other) = default;

    std::vector<point_t>::iterator begin() 
    {
        return points_.begin();
    }

    std::vector<point_t>::iterator middle() 
    {   
        return points_.begin() + points_.size()/2;
    }

    std::vector<point_t>::iterator end() 
    {
        return points_.end();
    }

    std::vector<point_t>::const_iterator begin() const
    {
        return points_.cbegin();
    }

    std::vector<point_t>::const_iterator middle() const
    {   
        return points_.cbegin() + points_.size()/2;
    }

    std::vector<point_t>::const_iterator end() const
    {
        return points_.cend();
    }

    size_t size() const
    {
        return points_.size();
    }    

private:

    std::vector<point_t> points_;  
};

struct assembly_profiles_t
{
    assembly_profiles_t(const profile_t cam_path, 
                        const profile_t joystick_path,
                        const point_t cam_pivot, 
                        const point_t joystick_pivot)
    : cam_path_(std::move(cam_path)), 
      joystick_path_(joystick_path), 
      cam_pivot_(cam_pivot), 
      joystick_pivot_(joystick_pivot)
    {
    }

    const profile_t cam_path_;
    const profile_t joystick_path_;
 
    const point_t cam_pivot_;
    const point_t joystick_pivot_;    
};

#endif // JCP_CAM_PROFILE_HPP
