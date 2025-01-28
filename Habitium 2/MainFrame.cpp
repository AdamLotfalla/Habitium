#include "MainFrame.h"
#include <wx/splitter.h>
#include "CustomButton.h"

MainFrame::MainFrame(wxString TITLE) : wxFrame(nullptr, wxID_ANY, TITLE) {
	wxInitAllImageHandlers();
	SetIcon(wxIcon("./Icons/alarm-clock.png", wxBITMAP_TYPE_PNG));

	makeSidePanel();
	sidePanelContents();

	this->Layout();
	this->Refresh();
}

void MainFrame::makeSidePanel()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE | wxSP_NO_XP_THEME);

	sidePanel = new wxPanel(splitter);
	notSidePanel = new wxPanel(splitter);

	splitter->SplitVertically(sidePanel, notSidePanel, 350);
	splitter->SetMinimumPaneSize(250);
	sidePanel->SetBackgroundColour(*wxWHITE);
	notSidePanel->SetBackgroundColour(wxColor("#F5F5F5"));

	sizer->Add(splitter, 1, wxEXPAND);
	this->SetSizerAndFit(sizer);
}

void MainFrame::sidePanelContents()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	CustomButton* addButton = new CustomButton(sidePanel, wxDefaultPosition, wxSize(-1, 35), "+", *wxWHITE, wxColor("#AD93CD"), 10, [this]() { this->AddHabit(); });

	wxFont font = wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	wxStaticText* projectTitle = new wxStaticText(sidePanel, wxID_ANY, "Habitium");
	projectTitle->SetBackgroundColour(sidePanel->GetBackgroundColour());
	projectTitle->SetForegroundColour(wxColor("#AD93CD"));
	projectTitle->SetFont(font);

	scrollPanel = new wxScrolledWindow(sidePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	scrollPanelSizer = new wxBoxSizer(wxVERTICAL);
	scrollPanel->SetSizerAndFit(scrollPanelSizer);
	scrollPanel->SetScrollRate(0, 10);
	sidePanel->SetDoubleBuffered(true);

	sidePanel->SetSizerAndFit(sizer);

	sizer->Add(projectTitle, 0, wxEXPAND | wxALL, 20);
	sizer->Add(scrollPanel, 1, wxEXPAND | wxALL, 5);	
	sizer->Add(addButton, 0, wxEXPAND | wxALL, 5);
	sizer->AddSpacer(10);
}

void MainFrame::loadHabits()
{
	scrollPanelSizer->Clear();

	count = 0;
	int height = 50;
	Habitfont = wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD, false);

	for (auto habit : habits) {
		AddHabitToPanel(habit);
	}

	scrollPanelSizer->AddStretchSpacer();
	scrollPanel->FitInside();
	scrollPanel->Layout();
}

void MainFrame::AddHabit()
{
	Habit newHabit = Habit("New Habit", "no description", "./Icons/alarm-clock.png");
	habits.push_back(newHabit);

	AddHabitToPanel(newHabit);
}

void MainFrame::AddHabitToPanel(Habit habit)
{
	color = wxColor(count % 2 ? "#eeeeee" : "#ffffff");

	wxPanel* backPanel = new wxPanel(scrollPanel);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* text = new wxStaticText(backPanel, wxID_ANY, habit.name, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
	wxStaticBitmap* icon = new wxStaticBitmap(backPanel, wxID_ANY, habit.icon);


	sizer->Add(icon, 0, wxSHAPED | wxCENTER | wxALL, 10);
	sizer->AddSpacer(3);
	sizer->Add(text, 1, wxEXPAND | wxALL, 5);

	backPanel->SetSizerAndFit(sizer);
	backPanel->SetBackgroundColour(color);
	text->SetBackgroundColour(color);
	text->SetForegroundColour(*wxBLACK);
	text->SetFont(Habitfont);
	text->Fit();

	scrollPanelSizer->Add(backPanel, 0, wxEXPAND);
	count++;

	scrollPanel->FitInside();
	scrollPanel->Layout();
}
