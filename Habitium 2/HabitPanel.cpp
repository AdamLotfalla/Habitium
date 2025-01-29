#include "HabitPanel.h"
#include "Habit.h"
#include "MainFrame.h"

HabitPanel::HabitPanel(wxWindow* parent, wxPoint position, wxSize size, wxString Habitname, wxString Habitdescription, wxString Habiticonpath, int& index, MainFrame* frame)
    : wxPanel(parent, wxID_ANY, position, size), habit(Habitname, Habitdescription, Habiticonpath)
{
    // Store mainframe reference
    mainframe = frame;

    sizer = new wxBoxSizer(wxHORIZONTAL);
    text = new wxStaticText(this, wxID_ANY, habit.name, wxDefaultPosition, wxSize(150, -1), wxALIGN_CENTER_VERTICAL | wxST_ELLIPSIZE_END);
    icon = new wxStaticBitmap(this, wxID_ANY, habit.icon);
    color = wxColor((index % 2 == 0) ? "#eeeeee" : "#ffffff");

    this->SetToolTip(habit.description);
    this->Bind(wxEVT_ENTER_WINDOW, &HabitPanel::OnHabitHover, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &HabitPanel::OnHabitUnhover, this);
    this->Bind(wxEVT_LEFT_DOWN, &HabitPanel::OnHabitClick, this);
    this->Bind(wxEVT_LEFT_UP, &HabitPanel::OnHabitUnclick, this);

    sizer->Add(icon, 0, wxSHAPED | wxCENTER | wxALL, 10);
    sizer->AddSpacer(3);
    sizer->Add(text, 1, wxCENTER | wxALL, 5);

    this->SetSizerAndFit(sizer);
    this->SetBackgroundColour(color);
    text->SetBackgroundColour(color);
    text->SetForegroundColour(wxColor("#555555"));
    text->SetFont(Habitfont);
    text->Fit();
    text->CenterOnParent();

    index++;

    this->Refresh();
    parent->FitInside();
    parent->Layout();
}

void HabitPanel::OnHabitHover(wxMouseEvent& evt)
{
    SetCursor(wxCURSOR_HAND);
    if (!this->HasCapture()) this->CaptureMouse();
    habitHover = true;
    if (!isSelected) {
        this->SetBackgroundColour(wxColor("#aaaaaa"));
        text->SetBackgroundColour(wxColor("#aaaaaa"));
    }

    this->Refresh();
    evt.Skip();
}

void HabitPanel::OnHabitUnhover(wxMouseEvent& evt)
{
    if (this->HasCapture()) this->ReleaseMouse();
    habitHover = false;

    if (!isSelected) {
        this->SetBackgroundColour(color);
        text->SetBackgroundColour(color);
    }

    this->Refresh();
    evt.Skip();
}

void HabitPanel::OnHabitClick(wxMouseEvent& evt)
{
    if (!isSelected) {
        this->SetBackgroundColour(wxColor("#999999"));
        text->SetBackgroundColour(wxColor("#999999"));
    }
    else {
        if(mainframe->activeHabitPanel != nullptr){
            mainframe->activeHabitPanel->SetBackgroundColour(mainframe->activeHabitPanel->color);
            mainframe->activeHabitPanel->text->SetBackgroundColour(mainframe->activeHabitPanel->color);
        }
        this->SetBackgroundColour(wxColor("#BE90D4"));
        text->SetBackgroundColour(wxColor("#BE90D4"));
    }
    mainframe->activeHabit = &habit;
    mainframe->activeHabitPanel = this;

    mainframe->UpdateHabitPreview();
    this->Refresh();
    evt.Skip();
}

void HabitPanel::OnHabitUnclick(wxMouseEvent& evt)
{
    if (!isSelected) {
        this->SetBackgroundColour(habitHover ? wxColor("#aaaaaa") : color);
        text->SetBackgroundColour(habitHover ? wxColor("#aaaaaa") : color);
    }
    this->Refresh();
    evt.Skip();
}

void HabitPanel::UpdatePanel(wxString newName, wxString newDescpription, wxBitmap newBitmap, int newFrequency, bool newNotification) {
    habit.name = newName;
    habit.description = newDescpription;
    habit.originalBitmap = newBitmap;
    habit.frequency = newFrequency;
    habit.notification = newNotification;

    text->SetLabel(newName);
    icon->SetBitmap(newBitmap);
}

void HabitPanel::UpdateColorBasedOnNewIndex(int index)
{
    color = wxColor((index % 2 == 0) ? "#eeeeee" : "#ffffff");
}
