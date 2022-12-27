#ifndef JCP_CONTROL_PLOT_PANEL
#define JCP_CONTROL_PLOT_PANEL

#include <wx/wx.h>

#include "plot_panel.hpp"
#include "control_panel.hpp"

#include "profile_generator.hpp"

class ControlPlotViewPanel: public wxPanel, public cam_profile_view
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

        ctrl_panel_ = new ControlPanel(control_plot_splitter_, wxID_ANY, wxDefaultPosition, wxSize(200, 200));
        plot_panel_ = new PlotPanel(control_plot_splitter_, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

        control_plot_splitter_->SplitVertically(ctrl_panel_, plot_panel_);

        this->SetSizer(main_sizer_);
    }

    virtual void draw(const cam_profile_t& cam_profile)
    {
        plot_panel_->draw(cam_profile);
    }

    cam_generator_config_t get_config() const
    {
        return ctrl_panel_->get_config();
    }

    void init_defaults(const cam_generator_config_t& cam_generator_config, size_t resolution)
    {
        ctrl_panel_->init_defaults(cam_generator_config, resolution);
    }

private:
    double sash_gravity_ = 0.5;
    int min_pane_size_ = 50;

    ControlPanel* ctrl_panel_;
    PlotPanel* plot_panel_;
    wxSplitterWindow* control_plot_splitter_;
    wxSizer* main_sizer_;
};

#endif // JCP_CONTROL_PLOT_PANEL
