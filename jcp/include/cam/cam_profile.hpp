#ifndef JCP_CAM_PROFILE_HPP
#define JCP_CAM_PROFILE_HPP

#include "cam_generator_config.hpp"

#include <wykobi/wykobi.hpp>
#include <vector>

struct profile_t
{
    using profile_collection_t = std::vector<wykobi::point2d<float>>;

    profile_t(profile_collection_t points)
    : points_(points)
    {
    }

    profile_collection_t::const_iterator begin() const
    {
        return points_.cbegin();
    }

    profile_collection_t::const_iterator middle() const
    {   
        return points_.begin() + points_.size()/2;
    }

    profile_collection_t::const_iterator end() const
    {
        return points_.cend();
    }

    size_t size() const
    {
        return points_.size();
    }

private:

    profile_collection_t points_;   
};

struct cam_profile_t
{
    using profile_points_t = std::vector<wykobi::point2d<float>>;

    profile_points_t cam_points_;
    profile_points_t joystick_points_;
    
    wykobi::point2d<float> cam_pivot_;
    wykobi::point2d<float> joystick_pivot_;

    cam_generator_config_t cam_generator_config_;

    size_t middle_point_index_;

    void reserve(size_t resolution)
    {
        size_t points_count = 2 * resolution + 1;
        cam_points_.resize(points_count);
        joystick_points_.resize(points_count);
    }
};

class cam_profile_view
{
public:
    virtual ~cam_profile_view() { }
    virtual void draw(const cam_profile_t& cam_profile) = 0;
};

#endif // JCP_CAM_PROFILE_HPP
