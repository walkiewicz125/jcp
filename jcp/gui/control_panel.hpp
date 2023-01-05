#ifndef JCP_CONTROL_PANEL
#define JCP_CONTROL_PANEL

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include "window_id.hpp"

struct property_grid_props
{
    wxPGProperty* cam_pivot_;
    wxPGProperty* cam_pivot_x_;
    wxPGProperty* cam_pivot_y_;

    wxPGProperty* joystick_pivot_;
    wxPGProperty* joystick_pivot_x_;
    wxPGProperty* joystick_pivot_y_;

    wxPGProperty* cam_gimbal_range_;
    wxPGProperty* joystick_gimbal_range_;
    wxPGProperty* joystick_lever_length_;
    wxPGProperty* resolution_;
};

class ControlPanel: public wxPanel
{
public:
    ControlPanel(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxTAB_TRAVERSAL | wxNO_BORDER)
        : wxPanel(parent, winid, pos, size, style)
    {
        property_grid_ = new wxPropertyGrid(
            this, // parent
            wxID_ANY, // id
            wxDefaultPosition, // position
            wxDefaultSize, // size
            // Here are just some of the supported window styles

            wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
            // Default style
            wxPG_DEFAULT_STYLE );
        // Window style flags are at premium, so some less often needed ones are
        // available as extra window styles (wxPG_EX_xxx) which must be set using
        // SetExtraStyle member function. wxPG_EX_HELP_AS_TOOLTIPS, for instance,
        // allows displaying help strings as tool tips.
        property_grid_->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS );
        property_grid_->Sort(wxPG_KEEP_STRUCTURE|wxPG_FORCE);

        props_.cam_pivot_ = property_grid_->Append(
            new wxStringProperty("Cam pivot", wxPG_LABEL, "<composed>"));
        props_.cam_pivot_x_ = property_grid_->AppendIn(props_.cam_pivot_, new wxFloatProperty("X", "x", 0.0));
        props_.cam_pivot_y_ = property_grid_->AppendIn(props_.cam_pivot_, new wxFloatProperty("y", "y", 0.0));

        props_.joystick_pivot_ = property_grid_->Append(
            new wxStringProperty("Joystick pivot", wxPG_LABEL, "<composed>"));
        props_.joystick_pivot_x_ = property_grid_->AppendIn(props_.joystick_pivot_, new wxFloatProperty("X", "x", 0.0));
        props_.joystick_pivot_y_ = property_grid_->AppendIn(props_.joystick_pivot_, new wxFloatProperty("y", "y", 0.0));

        props_.cam_gimbal_range_ = property_grid_->Append(new wxFloatProperty("Cam gimbal range", "cam_gimbal_range", 0.0));
        props_.joystick_gimbal_range_ = property_grid_->Append(new wxFloatProperty("Joystick gimbal range", "joystick_gimbal_range", 0.0));
        props_.joystick_lever_length_ = property_grid_->Append(new wxFloatProperty("Joystick leaver length", "joystick_lever_length", 0.0));
        props_.resolution_ = property_grid_->Append(new wxUIntProperty("Resolution", "resolution", 0.0));

        property_grid_->SetMinSize(wxSize(200, 50));

        button_sizer_ = new wxBoxSizer(wxVERTICAL);
        apply_button_ = new wxButton(this, JCP_APPLY_BTN, wxString("Apply label"));
        button_sizer_->Add(apply_button_, 1, wxEXPAND);

        export_button_ = new wxButton(this, JCP_EXPORT_BTN, wxString("Export to file"));
        button_sizer_->Add(export_button_, 1, wxEXPAND);

        main_sizer_ = new wxBoxSizer(wxVERTICAL);
        main_sizer_->Add(property_grid_, 1, wxEXPAND);
        main_sizer_->Add(button_sizer_, 0, wxEXPAND);
        this->SetSizer(main_sizer_);
    }

    cam_generator_config_t get_config() const
    {
        cam_generator_config_t new_config = {};

        new_config.cam_pivot_.x = property_grid_->GetPropertyValue(props_.cam_pivot_x_).GetDouble();
        new_config.cam_pivot_.y = property_grid_->GetPropertyValue(props_.cam_pivot_y_).GetDouble();

        new_config.joystick_pivot_.x = property_grid_->GetPropertyValue(props_.joystick_pivot_x_).GetDouble();
        new_config.joystick_pivot_.y = property_grid_->GetPropertyValue(props_.joystick_pivot_y_).GetDouble();


        new_config.cam_gimbal_range_ = property_grid_->GetPropertyValue(props_.cam_gimbal_range_).GetDouble();
        new_config.joystick_gimbal_range_ = property_grid_->GetPropertyValue(props_.joystick_gimbal_range_).GetDouble();
        new_config.joystick_lever_length_ = property_grid_->GetPropertyValue(props_.joystick_lever_length_).GetDouble();

        new_config.resolution_ = property_grid_->GetPropertyValue(props_.resolution_).GetInteger();

        return new_config;
    }

    void init_defaults(const cam_generator_config_t& config, size_t resolution)
    {
        property_grid_->SetPropertyValue(props_.cam_pivot_x_, config.cam_pivot_.x);
        property_grid_->SetPropertyValue(props_.cam_pivot_y_, config.cam_pivot_.y);
        property_grid_->SetPropertyValue(props_.joystick_pivot_x_, config.joystick_pivot_.x);
        property_grid_->SetPropertyValue(props_.joystick_pivot_y_, config.joystick_pivot_.y);
        property_grid_->SetPropertyValue(props_.cam_gimbal_range_, config.cam_gimbal_range_);
        property_grid_->SetPropertyValue(props_.joystick_gimbal_range_, config.joystick_gimbal_range_);
        property_grid_->SetPropertyValue(props_.joystick_lever_length_, config.joystick_lever_length_);
        property_grid_->SetPropertyValue(props_.resolution_, resolution); // TODO: fix
    }

private:
    wxPropertyGrid* property_grid_;
    property_grid_props props_;
    wxSizer* button_sizer_;
    wxButton* apply_button_;
    wxButton* export_button_;
    wxSizer* main_sizer_;
};

#endif // JCP_CONTROL_PANEL
