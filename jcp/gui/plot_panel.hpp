#ifndef JCP_PLOT_PANEL
#define JCP_PLOT_PANEL

#include <algorithm>

#include <wx/wx.h>
#include <mathplot/mathplot.h>

#include <cam/cam_profile.hpp>

class points_frame : public mpFXY
{
public:
    points_frame(const profile_t& profile)
        : mpFXY(wxT("profile_frame0")), index_(0),
          profile_(profile)
    {
    }

    virtual bool GetNextXY(double& x, double& y)
    {
        if (index_ < profile_.size())
        {
            x = profile_[index_].x;
            y = profile_[index_].y;
            index_++;
            return true;
        }
        else
        {
            return false;
        }

    }

    void update_min_max()
    {
        min_x_ = profile_[0].x;
        max_x_ = profile_[0].x;
        min_y_ = profile_[0].y;
        max_y_ = profile_[0].y;

        for (const auto& point : profile_)
        {
            min_x_ = std::min<double>(min_x_, point.x);
            max_x_ = std::max<double>(max_x_, point.x);
            min_y_ = std::min<double>(min_y_, point.y);
            max_y_ = std::max<double>(max_y_, point.y);
        }
    }

    virtual void Rewind() { index_ = 0; }
    virtual double GetMinX() { update_min_max(); return min_x_; }
    virtual double GetMaxX() { update_min_max(); return max_x_; }
    virtual double GetMinY() { update_min_max(); return min_y_; }
    virtual double GetMaxY() { update_min_max(); return max_y_; }

private:
    size_t index_;
    const profile_t& profile_;

    double min_x_;
    double max_x_;
    double min_y_;
    double max_y_;
};

class PlotPanel: public wxPanel
{
public:
    PlotPanel(const assembly_profiles_t& profile,
              wxWindow *parent,
              wxWindowID winid = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL | wxNO_BORDER)
        : wxPanel(parent, winid, pos, size, style),
          profile_(profile)
    {
        main_sizer_vertical_ = new wxBoxSizer(wxVERTICAL);
        neasted_sizer_horizontal_ = new wxBoxSizer(wxHORIZONTAL);

        mathplot_ = new mpWindow(this, wxID_ANY);


        wxPen mypen(*wxRED, 5, wxPENSTYLE_SOLID);
        joystick_points_frame_ = new points_frame(profile_.joystick_path_);
        joystick_points_frame_->SetPen(mypen);
        mathplot_->AddLayer(joystick_points_frame_);


        wxPen mypen2(*wxGREEN, 5, wxPENSTYLE_SOLID);
        cam_points_frame_ = new points_frame(profile_.cam_path_);
        cam_points_frame_->SetPen(mypen2);
        mathplot_->AddLayer(cam_points_frame_);

        joystick_points_frame_->SetName(wxT("joystick"));
        cam_points_frame_->SetName(wxT("cam"));

        neasted_sizer_horizontal_->Add(mathplot_, 1, wxEXPAND);
        main_sizer_vertical_->Add(neasted_sizer_horizontal_, 1, wxEXPAND);
        this->SetSizer(main_sizer_vertical_);
    }

    virtual void draw()
    {
        joystick_points_frame_->update_min_max();
        cam_points_frame_->update_min_max();
        mathplot_->Fit();
        mathplot_->LockAspect();
        mathplot_->ZoomOut();
    }

private:
    mpWindow* mathplot_;
    wxSizer* neasted_sizer_horizontal_;
    wxSizer* main_sizer_vertical_;
    points_frame* joystick_points_frame_;
    points_frame* cam_points_frame_;
    const assembly_profiles_t& profile_;
};

#endif // JCP_PLOT_PANEL
