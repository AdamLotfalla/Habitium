#include "HabitPanel.h"
#include "Habit.h"
#include "MainFrame.h"

HabitPanel::HabitPanel(wxWindow* parent, wxPoint position, wxSize size, wxString Habitname, wxString Habitdescription, wxString Habiticonpath, int& index, MainFrame* frame)
    : wxPanel(parent, wxID_ANY, position, size), habit(Habitname, Habitdescription, Habiticonpath)
{
    // Store mainframe reference
    mainframe = frame;

    sizer = new wxBoxSizer(wxHORIZONTAL);
    checkboxSizer = new wxBoxSizer(wxHORIZONTAL);
    panel = new wxPanel(this);
    text = new wxStaticText(panel, wxID_ANY, habit.name, wxDefaultPosition, wxSize(150, -1), wxALIGN_CENTER_VERTICAL | wxST_ELLIPSIZE_END);
    icon = new wxStaticBitmap(panel, wxID_ANY, habit.icon);
    checkBox = new wxCheckBox(this, wxID_ANY, "", wxDefaultPosition, wxSize(25,25));
    checkBox->SetValue(habit.checked);
    color = wxColor((index % 2 == 0) ? "#FCFCFC" : "#ffffff");

    this->SetToolTip(habit.description);
    panel->SetToolTip(habit.description);
    //text->SetToolTip(habit.description);
    //icon->SetToolTip(habit.description);

    panel->Bind(wxEVT_ENTER_WINDOW, &HabitPanel::OnHabitHover, this);
    panel->Bind(wxEVT_LEAVE_WINDOW, &HabitPanel::OnHabitUnhover, this);
    panel->Bind(wxEVT_LEFT_DOWN, &HabitPanel::OnHabitClick, this);
    panel->Bind(wxEVT_LEFT_UP, &HabitPanel::OnHabitUnclick, this);

    checkBox->Bind(wxEVT_CHECKBOX, &HabitPanel::OnCheckBox, this);

    sizer->Add(icon, 0, wxSHAPED | wxCENTER | wxALL, 10);
    sizer->AddSpacer(3);
    sizer->Add(text, 1, wxCENTER | wxALL, 5);
    //sizer->AddStretchSpacer();
    checkboxSizer->Add(panel, 1, wxEXPAND);
    checkboxSizer->Add(checkBox, 0, wxCENTER | wxALL, 2);

    panel->SetSizerAndFit(sizer);
    this->SetSizerAndFit(checkboxSizer);
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
    if (!panel->HasCapture()) panel->CaptureMouse();
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
    habitHover = false;
    if (panel->HasCapture()) panel->ReleaseMouse();
    if (!isSelected) {
        this->SetBackgroundColour(color);
        text->SetBackgroundColour(color);
    }
    this->Refresh();
    evt.Skip();
}

void HabitPanel::OnHabitClick(wxMouseEvent& evt)
{
    if (mainframe->activeHabitPanel != nullptr) {
        mainframe->activeHabitPanel->isSelected = false;
    }

    isSelected = true;

    if (!isSelected) {
        this->SetBackgroundColour(wxColor("#999999"));
        text->SetBackgroundColour(wxColor("#999999"));
    }
    else {
        if(mainframe->activeHabitPanel != nullptr){
            mainframe->activeHabitPanel->SetBackgroundColour(mainframe->activeHabitPanel->color);
            mainframe->activeHabitPanel->text->SetBackgroundColour(mainframe->activeHabitPanel->color);
        }
        this->SetBackgroundColour(wxColor("#AE9CC4"));
        text->SetBackgroundColour(wxColor("#AE9CC4"));
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

    habit.icon = habit.originalBitmap;
    habit.icon.Rescale(habit.icon, wxSize(25, 25));


    this->SetToolTip(habit.description);
    panel->SetToolTip(habit.description);
    //text->SetToolTip(habit.description);
    //icon->SetToolTip(habit.description);

    text->SetLabel(newName);
    icon->SetBitmap(habit.icon);
}

void HabitPanel::UpdateColorBasedOnNewIndex(int index)
{
    color = wxColor((index % 2 == 0) ? "#FCFCFC" : "#ffffff");
    if (!isSelected) {
    this->SetBackgroundColour(color);
    text->SetBackgroundColour(color);
    }
    this->Refresh();
}

void HabitPanel::OnCheckBox(wxCommandEvent& evt)
{
    if (checkBox->GetValue()) {
        habit.currentStreak++;
        if (habit.currentStreak > habit.bestStreak) {
            inBestStreak = true;
            habit.bestStreak = habit.currentStreak;
        }
    }
    else {
        habit.currentStreak--;
        if (inBestStreak) {
            habit.bestStreak--;
        }
    }

    mainframe->UpdateHabitPreview();
    Refresh();
    evt.Skip();
}

