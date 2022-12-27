#ifndef JCP_PROFILE_GENERATOR_ID
#define JCP_PROFILE_GENERATOR_ID

#include <wykobi/wykobi.hpp>
#include <vector>

#include <algorithm>

struct cam_generator_config_t
{
    float cam_gimbal_range_;
    wykobi::point2d<float> cam_pivot_;
    // TODO: wykobi::point2d<float> cam_end_point_;

    wykobi::point2d<float> joystick_pivot_;
    float joystick_lever_length_;
    float joystick_gimbal_range_;

    static cam_generator_config_t get_default()
    {
        cam_generator_config_t conf = {};
        conf.cam_gimbal_range_ = 20.0f;
        conf.cam_pivot_ = wykobi::make_point<float>(120.0f, -105.0f);
        conf.joystick_gimbal_range_ = 30.0f;
        conf.joystick_lever_length_ = 100.0f;
        conf.joystick_pivot_ = wykobi::make_point<float>(0.0f, 0.0f);

        return conf;
    }
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

class cam_profile_view
{
public:
    virtual ~cam_profile_view() { }
    virtual void draw(const cam_profile_t& cam_profile) = 0;
};

#endif // JCP_PROFILE_GENERATOR_ID
