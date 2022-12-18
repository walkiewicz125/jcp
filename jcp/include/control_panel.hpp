#ifndef JCP_CONTROL_PANEL
#define JCP_CONTROL_PANEL

#include <wx/wx.h>
#include "window_id.hpp"

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
        wxPropertyGrid* pg = new wxPropertyGrid(
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
        pg->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS );
        pg->Sort(wxPG_KEEP_STRUCTURE|wxPG_FORCE);
        
        wxPGProperty* cam_pivot_pg = pg->Append(
            new wxStringProperty("Cam pivot", wxPG_LABEL, "<composed>"));
        pg->AppendIn(cam_pivot_pg, new wxFloatProperty("X", "x", 0.0));
        pg->AppendIn(cam_pivot_pg, new wxFloatProperty("y", "y", 0.0));
        
        wxPGProperty* joystick_pivot_pg = pg->Append(
            new wxStringProperty("Joystick pivot", wxPG_LABEL, "<composed>"));
        pg->AppendIn(joystick_pivot_pg, new wxFloatProperty("X", "x", 0.0));
        pg->AppendIn(joystick_pivot_pg, new wxFloatProperty("y", "y", 0.0));
        
        pg->Append(new wxFloatProperty("Cam gimbal range", "cam_gimbal_range", 0.0));        
        pg->Append(new wxFloatProperty("Joystick gimbal range", "joystick_gimbal_range", 0.0));
        pg->Append(new wxFloatProperty("Joystick leaver length", "joystick_lever_length", 0.0));        
        pg->Append(new wxIntProperty("Resolution", "resolution", 0.0));

        pg->SetMinSize(wxSize(200, 50));
        
        button_sizer_ = new wxBoxSizer(wxHORIZONTAL);
        apply_button_ = new wxButton(this, JCP_APPLY_BTN, wxString("Apply label"));
        button_sizer_->Add(apply_button_, 1, wxEXPAND);

        main_sizer_ = new wxBoxSizer(wxVERTICAL);
        main_sizer_->Add(pg, 1, wxEXPAND);
        main_sizer_->Add(button_sizer_, 0, wxEXPAND);
        this->SetSizer(main_sizer_);      
    }

private:
    wxSizer* button_sizer_; 
    wxButton* apply_button_;
    wxSizer* main_sizer_;
};

#endif // JCP_CONTROL_PANEL
