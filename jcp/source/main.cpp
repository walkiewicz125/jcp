
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

#include <sstream>

#include <gui/control_plot_panel.hpp>
#include <file_exporter/file_exporter.hpp>

#include <cam/cam_generator_config.hpp>
#include <cam/cam_generator.hpp>

class MyFrame: public wxFrame
{
public:
    MyFrame(const assembly_profiles_t& profiles,
            const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        panel_ = new ControlPlotViewPanel(profiles, this, wxID_ANY);

        sizer_ = new wxBoxSizer(wxHORIZONTAL);
        sizer_->Add(panel_, 1, wxEXPAND, 0);

        this->SetSizer(sizer_);
        sizer_->SetSizeHints(this);

        CreateStatusBar();
        SetStatusText( "JoystickCamProfiler" );
        SetMinSize(wxSize(600, 400));
    }

    virtual void draw()
    {
        panel_->draw();
    }

    cam_generator_config_t get_config() const
    {
        return panel_->get_config();
    }

    void init_defaults(const cam_generator_config_t& cam_generator_config, size_t resolution)
    {
        panel_->init_defaults(cam_generator_config, resolution);
    }

private:
    wxSizer* sizer_;
    ControlPlotViewPanel* panel_;
};

class MyApp: public wxApp
{
public:
    MyApp()
    {
    }

    virtual bool OnInit()
    {
        frame_ = new MyFrame(profiles_,
                             "JoystickCamProfiler",
                             wxPoint(50, 50), wxSize(450, 340));
        frame_->Show( true );

        Bind(wxEVT_BUTTON, &MyApp::OnApplyButton, this, JCP_APPLY_BTN);
        Bind(wxEVT_BUTTON, &MyApp::OnExportButton, this, JCP_EXPORT_BTN);

        cam_generator_config_ = cam_generator_config_t::get_default();
        frame_->init_defaults(cam_generator_config_, resolution_);

        update();

        return true;
    }

    void OnApplyButton(wxCommandEvent& event)
    {
        update();
    }

    void OnExportButton(wxCommandEvent& event)
    {
        wxString CurrentDocPath;

        wxDirDialog* OpenDialog = new wxDirDialog(
            nullptr, "Choose a destination directory");

        if (OpenDialog->ShowModal() == wxID_OK)
        {
            CurrentDocPath = OpenDialog->GetPath();
        }

        wxString file_name_left(CurrentDocPath + "\\cam_profile_left.csv");
        wxString file_name_right(CurrentDocPath + "\\cam_profile_right.csv");

        csv_profile_exporter exporter(file_name_left, file_name_right);

        exporter.write_profile(profiles_.cam_path_);

        OpenDialog->Destroy();


        wxMessageDialog open_dir_box(nullptr, "Open containing folder", "Files export complete", wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
        open_dir_box.SetYesNoLabels(_("&Show in folder"), _("&Done"));
        if (open_dir_box.ShowModal() == wxID_YES)
        {
            wxLaunchDefaultApplication(CurrentDocPath);
        }
    }

private:
    void update()
    {
        cam_generator_config_ = frame_->get_config();
        profiles_ = std::move(cam_generator_.generate(cam_generator_config_));

        frame_->draw();
    }

    MyFrame* frame_;
    size_t resolution_ = 100;
    cam_generator_config_t cam_generator_config_;
    profiles_generator cam_generator_;
    assembly_profiles_t profiles_;
};

wxIMPLEMENT_APP(MyApp);
