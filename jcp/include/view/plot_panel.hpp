#ifndef JCP_PLOT_PANEL
#define JCP_PLOT_PANEL

#include <algorithm>

#include <wx/wx.h>
#include <mathplot/mathplot.h>

#include <cam/cam_profile.hpp>

class points_frame : public mpFXY
{   
public:
    points_frame()
        : mpFXY(wxT("profile_frame0")), index_(0)
    {
    }

    virtual bool GetNextXY(double& x, double& y)
    {
        if (index_ < profile_points_.size())
        {
            x = profile_points_[index_].x;
            y = profile_points_[index_].y;
            index_++;
            return true;
        }
        else
        {
            return false;
        }
    
    }

    void set_points(const cam_profile_t::profile_points_t& points)
    {
        profile_points_ = points;

        min_x_ = points[0].x;
        max_x_ = points[0].x;
        min_y_ = points[0].y;
        max_y_ = points[0].y;

        for (const auto& point : points)
        {
            min_x_ = std::min<double>(min_x_, point.x);
            max_x_ = std::max<double>(max_x_, point.x);
            min_y_ = std::min<double>(min_y_, point.y);
            max_y_ = std::max<double>(max_y_, point.y);
        }
    }

   virtual void Rewind() { index_ = 0; }
   virtual double GetMinX() { return min_x_; }
   virtual double GetMaxX() { return max_x_; }
   virtual double GetMinY() { return min_y_; }
   virtual double GetMaxY() { return max_y_; }

private:
    size_t index_;
    cam_profile_t::profile_points_t profile_points_;

    double min_x_;
    double max_x_;
    double min_y_;
    double max_y_;
};

class PlotPanel: public wxPanel, public cam_profile_view
{
public:
    PlotPanel(wxWindow *parent,
              wxWindowID winid = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL | wxNO_BORDER)
        : wxPanel(parent, winid, pos, size, style)
    {
        main_sizer_vertical_ = new wxBoxSizer(wxVERTICAL);
        neasted_sizer_horizontal_ = new wxBoxSizer(wxHORIZONTAL);

        mathplot_ = new mpWindow(this, wxID_ANY);
        
        
        wxPen mypen(*wxRED, 5, wxPENSTYLE_SOLID);
        joystick_points_frame_ = new points_frame();
        joystick_points_frame_->SetPen(mypen);
        mathplot_->AddLayer(joystick_points_frame_);

        
        wxPen mypen2(*wxGREEN, 5, wxPENSTYLE_SOLID);
        cam_points_frame_ = new points_frame();
        cam_points_frame_->SetPen(mypen2);
        mathplot_->AddLayer(cam_points_frame_);


        pivots_.reserve(2);
        wxPen mypen3(*wxBLUE, 5, wxPENSTYLE_SOLID);
        pivots_frame_ = new points_frame();
        pivots_frame_->SetPen(mypen3);
        mathplot_->AddLayer(pivots_frame_);

        joystick_points_frame_->SetName(wxT("joystick"));
        cam_points_frame_->SetName(wxT("cam"));
        pivots_frame_->SetName(wxT("pivots"));

        neasted_sizer_horizontal_->Add(mathplot_, 1, wxEXPAND);        
        main_sizer_vertical_->Add(neasted_sizer_horizontal_, 1, wxEXPAND);
        this->SetSizer(main_sizer_vertical_);      
    }

    virtual void draw(const cam_profile_t& cam_profile)
    {
        joystick_points_frame_->set_points(cam_profile.joystick_points_);
        cam_points_frame_->set_points(cam_profile.cam_points_);
        
        pivots_.clear();
        pivots_.push_back(cam_profile.cam_pivot_);
        pivots_.push_back(cam_profile.joystick_pivot_);
        pivots_frame_->set_points(pivots_);

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
    cam_profile_t::profile_points_t pivots_;
    points_frame* pivots_frame_;
};

#endif // JCP_PLOT_PANEL
