#ifndef FILE_EXPORTER_HPP
#define FILE_EXPORTER_HPP


#include <wx/wx.h>
#include <wx/file.h>

#include "profile_generator.hpp"


class csv_file_exporter
{

public:
    csv_file_exporter(const wxString &filename)
    : filename_(filename), file_(filename, wxFile::write)
    {
    }

    csv_file_exporter& operator<<(const wxString& text)
    {
        write_value(text);

        return *this;
    }

private:
    size_t write_value(const wxString& text)
    {
        file_.Write(text);
        file_.Flush();
    
        return text.size();
    }

    wxString filename_;
    wxFile file_;
};

class csv_profile_exporter
{

public:
    csv_profile_exporter(const wxString &filename)
    : file_exporter_(filename)
    {

    }

    size_t write_point(const wykobi::point2d<float>& profile_point)
    {
        wxString point_as_text = wxString::Format(wxT("%f,%f,%f\n"), profile_point.x,  profile_point.y, 0.0f);
        file_exporter_ << point_as_text;

        return point_as_text.size();
    }

    size_t write_profile(const cam_profile_t::profile_points_t& profile_points)
    {
        size_t size;
        
        for (const auto& point : profile_points)
        {
            size += write_point(point);
        }

        return size;
    }

private:

    csv_file_exporter file_exporter_;
};

#endif // FILE_EXPORTER_HPP
