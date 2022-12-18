
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/msgdlg.h>

#include "control_plot_panel.hpp"
#include "profile_generator.hpp"

class MyFrame: public wxFrame, public cam_profile_view
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        panel_ = new ControlPlotViewPanel(this, wxID_ANY);

        sizer_ = new wxBoxSizer(wxHORIZONTAL);
        sizer_->Add(panel_, 1, wxEXPAND, 0);

        this->SetSizer(sizer_);
        sizer_->SetSizeHints(this);

        CreateStatusBar();
        SetStatusText( "JoystickCamProfiler" );
    }

    virtual void draw(const cam_profile_t& cam_profile)
    {
        panel_->draw(cam_profile);
    }

private:
    wxSizer* sizer_;
    ControlPlotViewPanel* panel_;
};

class MyApp: public wxApp
{
public:
    MyApp()
    : cam_generator_(cam_generator_config_)
    {
    }

    virtual bool OnInit()
    {
        frame_ = new MyFrame( "JoystickCamProfiler", wxPoint(50, 50), wxSize(450, 340) );
        frame_->Show( true );

        Bind(wxEVT_BUTTON, &MyApp::OnApplyButton, this, JCP_APPLY_BTN);

        cam_generator_config_ = cam_generator_config_t::get_default();

        return true;
    }

    void OnApplyButton(wxCommandEvent& event)
    {
        frame_->draw(cam_generator_.generate_profile(resolution_));
    }

private:
    MyFrame* frame_;
    size_t resolution_ = 100;
    cam_generator_config_t cam_generator_config_;
    cam_generator cam_generator_;
};

wxIMPLEMENT_APP(MyApp);
