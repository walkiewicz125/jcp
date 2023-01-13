#ifndef JCP_CAM_PROFILE_HPP
#define JCP_CAM_PROFILE_HPP

#include "cam_generator_config.hpp"

#include <wykobi/wykobi.hpp>
#include <vector>

using point_t = wykobi::point2d<float>;

struct resolution_t
{
    using value_type = size_t;

    value_type get_size() const
    {
        return 2 * resolution_ + 1;
    }

    value_type resolution_;
};

class profile_t
{
public:
    using iterator = std::vector<point_t>::iterator;
    using const_iterator = std::vector<point_t>::const_iterator;
    using reverse_iterator = std::vector<point_t>::reverse_iterator;
    using const_reverse_iterator = std::vector<point_t>::const_reverse_iterator;

    profile_t(size_t size = 0)
    : points_(size)
    {
    }

    explicit profile_t(resolution_t resolution)
    : points_(resolution.get_size())
    {
    }

    profile_t(const profile_t& other) = delete;
    profile_t& operator=(const profile_t& other) = delete;

    profile_t(profile_t&& other)
    : points_(std::move(other.points_))
    {
        other.points_.clear();
    }

    profile_t& operator=(profile_t&& other)
    {
        points_ = std::move(other.points_);
        other.points_.clear();
        return *this;
    }

    iterator begin()
    {
        return points_.begin();
    }

    iterator middle()
    {
        return points_.begin() + points_.size()/2;
    }

    iterator end()
    {
        return points_.end();
    }

    const_iterator begin() const
    {
        return points_.cbegin();
    }

    const_iterator middle() const
    {
        return points_.cbegin() + points_.size()/2;
    }

    const_iterator end() const
    {
        return points_.cend();
    }

    size_t size() const
    {
        return points_.size();
    }

    point_t& operator[](size_t index)
    {
        return points_[index];
    }

    const point_t& operator[](size_t index) const
    {
        return points_[index];
    }

private:
    std::vector<point_t> points_;
};

struct assembly_profiles_t
{
    assembly_profiles_t(profile_t&& cam_path,
                        profile_t&& joystick_path,
                        point_t cam_pivot,
                        point_t joystick_pivot)
    : cam_path_(std::move(cam_path)),
      joystick_path_(std::move(joystick_path)),
      cam_pivot_(cam_pivot),
      joystick_pivot_(joystick_pivot)
    {
    }

    assembly_profiles_t(assembly_profiles_t&& other)
    : cam_path_(std::move(other.cam_path_)),
      joystick_path_(std::move(other.joystick_path_)),
      cam_pivot_(other.cam_pivot_),
      joystick_pivot_(other.joystick_pivot_)
    {
    };

    profile_t get_pivots_as_path() const
    {
        profile_t pivots(2);

        pivots[0] = cam_pivot_;
        pivots[1] = joystick_pivot_;

        return pivots;
    }

    assembly_profiles_t& operator=(assembly_profiles_t&& other)
    {
        cam_path_ = std::move(other.cam_path_);
        joystick_path_ = std::move(other.joystick_path_);

        cam_pivot_ = other.cam_pivot_;
        joystick_pivot_ = other.joystick_pivot_;

        return *this;
    }

    assembly_profiles_t() = default;
    assembly_profiles_t(const assembly_profiles_t&) = delete;
    assembly_profiles_t& operator=(assembly_profiles_t&) = delete;

    profile_t cam_path_;
    profile_t joystick_path_;

    point_t cam_pivot_;
    point_t joystick_pivot_;
};

#endif // JCP_CAM_PROFILE_HPP
