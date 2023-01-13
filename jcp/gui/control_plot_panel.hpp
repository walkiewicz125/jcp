#ifndef JCP_CONTROL_PLOT_PANEL
#define JCP_CONTROL_PLOT_PANEL

#include <wx/wx.h>
#include <wx/splitter.h>

#include "plot_panel.hpp"
#include "control_panel.hpp"

#include <cam/cam_profile.hpp>

class ControlPlotViewPanel: public wxPanel
{
public:
    ControlPlotViewPanel(wxWindow *parent,
                         wxWindowID winid = wxID_ANY,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxTAB_TRAVERSAL | wxNO_BORDER)
        : wxPanel(parent, winid, pos, size, style)
    {
        control_plot_splitter_ = new wxSplitterWindow(this, wxID_ANY);
        control_plot_splitter_->SetSashGravity(sash_gravity_);
        control_plot_splitter_->SetMinimumPaneSize(min_pane_size_);

        main_sizer_ = new wxBoxSizer(wxVERTICAL);
        main_sizer_->Add(control_plot_splitter_, 1, wxEXPAND, 0);

        ctrl_panel_ = new ControlPanel(control_plot_splitter_, wxID_ANY, wxDefaultPosition);
        plot_panel_ = new PlotPanel(control_plot_splitter_, wxID_ANY, wxDefaultPosition);

        control_plot_splitter_->SplitVertically(ctrl_panel_, plot_panel_);

        this->SetSizer(main_sizer_);
    }

    virtual void draw(const assembly_profiles_t& profiles)
    {
        plot_panel_->draw(profiles);
    }

    cam_generator_config_t get_config() const
    {
        return ctrl_panel_->get_config();
    }

    void init_defaults(const cam_generator_config_t& cam_generator_config)
    {
        ctrl_panel_->init_defaults(cam_generator_config);
    }

private:
    double sash_gravity_ = 0.25;
    int min_pane_size_ = 50;

    ControlPanel* ctrl_panel_;
    PlotPanel* plot_panel_;
    wxSplitterWindow* control_plot_splitter_;
    wxSizer* main_sizer_;
};

#endif // JCP_CONTROL_PLOT_PANEL
