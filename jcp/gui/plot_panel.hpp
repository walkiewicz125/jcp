#ifndef JCP_PLOT_PANEL
#define JCP_PLOT_PANEL

#include <algorithm>

#include <wx/wx.h>
#include <mathplot/mathplot.h>

#include <cam/cam_profile.hpp>

class profile_frame : public mpFXYVector
{
public:
    profile_frame(wxString name = wxEmptyString,
                  int flags = mpALIGN_NE)
    : mpFXYVector(name, flags)
    {
    }

    void update(const profile_t& profile)
    {
        xs_.clear();
        ys_.clear();
        xs_.resize(profile.size());
        ys_.resize(profile.size());

        for (size_t i = 0; i < profile.size(); i++)
        {
            xs_[i] = profile[i].x;
            ys_[i] = profile[i].y;
        }

        SetData(xs_, ys_);
    }

private:
    std::vector<double> xs_;
    std::vector<double> ys_;
};

class PlotPanel: public wxPanel
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
        joystick_points_frame_ = new profile_frame();
        joystick_points_frame_->SetPen(mypen);
        mathplot_->AddLayer(joystick_points_frame_);


        wxPen mypen2(*wxGREEN, 5, wxPENSTYLE_SOLID);
        cam_points_frame_ = new profile_frame();
        cam_points_frame_->SetPen(mypen2);
        mathplot_->AddLayer(cam_points_frame_);

        joystick_points_frame_->SetName(wxT("joystick"));
        cam_points_frame_->SetName(wxT("cam"));


        wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        xaxis_layer_ = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, false, mpX_NORMAL);
        yaxis_layer_ = new mpScaleY(wxT("Y"), mpALIGN_LEFT, false);
        xaxis_layer_->SetFont(graphFont);
        yaxis_layer_->SetFont(graphFont);
        xaxis_layer_->SetDrawOutsideMargins(false);
        yaxis_layer_->SetDrawOutsideMargins(false);
        // Fake axes formatting to test arbitrary format string
        // xaxis_layer_->SetLabelFormat(wxT("%.2f €"));
        // yaxis_layer_->SetLabelFormat(wxT("%p"));
        mathplot_->SetMargins(30, 30, 50, 100);
    //     m_plot->SetMargins(50, 50, 200, 150);
        mathplot_->AddLayer(xaxis_layer_);
        mathplot_->AddLayer(yaxis_layer_);

        neasted_sizer_horizontal_->Add(mathplot_, 1, wxEXPAND);
        main_sizer_vertical_->Add(neasted_sizer_horizontal_, 1, wxEXPAND);
        this->SetSizer(main_sizer_vertical_);
    }

    virtual void draw(const assembly_profiles_t& profiles)
    {
        joystick_points_frame_->update(profiles.joystick_path_);
        cam_points_frame_->update(profiles.cam_path_);
        mathplot_->Refresh();
        mathplot_->LockAspect();
        mathplot_->Fit();
    }

private:
    mpWindow* mathplot_;
    wxSizer* neasted_sizer_horizontal_;
    wxSizer* main_sizer_vertical_;
    profile_frame* joystick_points_frame_;
    profile_frame* cam_points_frame_;
    mpScaleX* xaxis_layer_;
    mpScaleY* yaxis_layer_;
};

#endif // JCP_PLOT_PANEL
