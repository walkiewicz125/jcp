#ifndef JCP_PROFILE_RANGE_HPP
#define JCP_PROFILE_RANGE_HPP

#include "cam_profile.hpp"

#include <wykobi/wykobi.hpp>

class profile_range
{

public:
    profile_range left_half(const cam_profile_t::profile_points_t& profile, size_t middle_point_index, bool include_middle_point = true)
    {
        size_t middle_point_offset = 0;

        if (include_middle_point)
        {
            middle_point_offset = 1;
        }

        if (profile.size() >= middle_point_index)
        {
            return profile_range(profile.cbegin(), profile.cbegin() + middle_point_index + middle_point_offset);
        }

        return profile_range();
    }
    
    profile_range right_half(const cam_profile_t::profile_points_t& profile, size_t middle_point_index, bool include_middle_point = true)
    {
        size_t middle_point_offset = 0;

        if (include_middle_point and middle_point_index > 0)
        {
            middle_point_offset = 1;
        }

        if (profile.size() >= middle_point_index)
        {
            return profile_range(profile.cbegin() + middle_point_index - middle_point_offset, profile.cend());
        }

        return profile_range();
    }

    cam_profile_t::profile_points_t::const_iterator cbegin() const
    {
        return begin_;
    }

    cam_profile_t::profile_points_t::const_iterator cend() const
    {
        return end_;
    }

    explicit operator bool() const
    {
        return begin_ != end_;
    }

private:
    profile_range()
    : begin_(), end_()
    {
    }

    profile_range(cam_profile_t::profile_points_t::const_iterator begin, cam_profile_t::profile_points_t::const_iterator end)
    : begin_(begin), end_(end)
    {
    }

    cam_profile_t::profile_points_t::const_iterator begin_;
    cam_profile_t::profile_points_t::const_iterator end_;
};

#endif // JCP_PROFILE_RANGE_HPP
