#ifndef FILE_EXPORTER_HPP
#define FILE_EXPORTER_HPP


#include <wx/wx.h>
#include <wx/file.h>

#include <cam/cam_profile.hpp>


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
    csv_profile_exporter(const wxString &filename_left, const wxString &filename_right)
    : file_exporter_left_half_(filename_left),
      file_exporter_right_half_(filename_right)
    {

    }

    size_t write_profile(const profile_t& profile_points)
    {
        size_t size{0};

        wxString txt;
        for (auto point_it = profile_points.begin(); point_it != (profile_points.middle() + 1); point_it++)
        {
            txt = wxString::Format(wxT("%f,%f,%f\n"), point_it->x,  point_it->y, 0.0f);
            file_exporter_left_half_ << txt;
            size += txt.size();
        }

        for (auto point_it = profile_points.middle(); point_it != profile_points.end(); point_it++)
        {
            txt = wxString::Format(wxT("%f,%f,%f\n"), point_it->x,  point_it->y, 0.0f);
            file_exporter_right_half_ << txt;
            size += txt.size();
        }

        return size;
    }

private:
    csv_file_exporter file_exporter_left_half_;
    csv_file_exporter file_exporter_right_half_;
};

#endif // FILE_EXPORTER_HPP
