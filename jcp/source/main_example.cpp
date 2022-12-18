
#include <wx/wx.h>

class MyApp: public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame *frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
        frame->Show( true );
        return true;
    }
};





class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxMenu *menuFile = new wxMenu;
        menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                        "Help string shown in status bar for this menu item");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);
        wxMenu *menuHelp = new wxMenu;
        menuHelp->Append(wxID_ABOUT);
        wxMenuBar *menuBar = new wxMenuBar;
        menuBar->Append( menuFile, "&File" );
        menuBar->Append( menuHelp, "&Help" );
        SetMenuBar( menuBar );
        CreateStatusBar();
        SetStatusText( "Welcome to wxWidgets!" );
    }
private:

    wxDECLARE_EVENT_TABLE();
};


wxIMPLEMENT_APP(MyApp);
