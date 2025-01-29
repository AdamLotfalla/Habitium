#include "MainFrame.h"
#include <wx/splitter.h>
#include "CustomButton.h"
#include "HabitPanel.h"
#include "RenameDialog.h"

MainFrame::MainFrame(wxString TITLE) : wxFrame(nullptr, wxID_ANY, TITLE) {
	wxInitAllImageHandlers();
	SetIcon(wxIcon("./Icons/alarm-clock.png", wxBITMAP_TYPE_PNG));
	activeStreak = wxBitmap("./Icons/fire-colored.png", wxBITMAP_TYPE_PNG);
	dormantStreak = wxBitmap("./Icons/fire-pale.png", wxBITMAP_TYPE_PNG);
	activeStreak.Rescale(activeStreak, wxSize(30, 30));
	dormantStreak.Rescale(dormantStreak, wxSize(30, 30));

	makeSidePanel();
	sidePanelContents();
	makePreviewPanel();
	UpdateHabitPreview();

	this->Layout();
	this->Refresh();

	//	TEMPORARY!
	CreateStatusBar();
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

	addButton->SetToolTip("Add a new habit");

	wxFont font = wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY);

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
	sizer->AddSpacer(7);
}

void MainFrame::loadHabits()
{
	scrollPanelSizer->Clear();

	wxLogStatus("loaded habits");

	count = 0;
	int height = 50;

	for (auto& habitPanel : habitPanels) {
		habitPanel->UpdateColorBasedOnNewIndex(count);
		scrollPanelSizer->Add(habitPanel, 0, wxEXPAND);
		count++;
	}



	//scrollPanelSizer->AddStretchSpacer();
	scrollPanel->FitInside();
	scrollPanel->Layout();
	scrollPanel->Refresh();
}

void MainFrame::makePreviewPanel()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	previewPanel = new wxPanel(notSidePanel);
	wxFont font = wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY);
	wxBoxSizer* previewSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBitmap gear = wxBitmap("./Icons/gear.png", wxBITMAP_TYPE_PNG);
	icon = (activeHabit == nullptr) ? wxBitmap("./Icons/grocery-bag.png", wxBITMAP_TYPE_PNG) : activeHabit->icon;
	gear.Rescale(gear, wxSize(15, 15));
	icon.Rescale(icon, wxSize(50, 50));

	iconBitmap = new wxStaticBitmap(previewPanel, wxID_ANY, icon);
	wxStaticBitmap* gearBitmap = new wxStaticBitmap(previewPanel, wxID_ANY, gear);
	streakBitmap = new wxStaticBitmap(previewPanel, wxID_ANY, dormantStreak);
	HabitName = new wxStaticText(previewPanel, wxID_ANY, (activeHabit == nullptr)? "no Habit is selected" : activeHabit->name, wxDefaultPosition, wxSize(-1, 30), wxALIGN_CENTER_VERTICAL);
	wxBoxSizer* streakVertical = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* streakHorizontal = new wxBoxSizer(wxHORIZONTAL);

	bestStreakText = new wxStaticText(previewPanel, wxID_ANY, "Personal best: " + (activeHabit == nullptr) ? "nan" : to_string(activeHabit->bestStreak), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	streakText = new wxStaticText(previewPanel, wxID_ANY, (activeHabit == nullptr) ? "0" : to_string(activeHabit->currentStreak));
	streakText->SetFont(font);

	streakVertical->Add(streakHorizontal, 0, wxCENTER | wxUP, 3);
	streakVertical->Add(bestStreakText, 0, wxEXPAND);
	streakVertical->AddStretchSpacer();

	streakHorizontal->Add(streakText, 0, wxCENTER | wxUP, 7);
	streakHorizontal->AddSpacer(2);
	streakHorizontal->Add(streakBitmap, wxSizerFlags().Proportion(0).Expand().Border(wxDOWN, 7).Border(wxRIGHT | wxLEFT | wxUP, 3));

	HabitName->SetFont(font);

	previewPanel->SetSizerAndFit(previewSizer);
	previewPanel->SetBackgroundColour(notSidePanel->GetBackgroundColour());
	HabitName->SetBackgroundColour(notSidePanel->GetBackgroundColour());

	headerSizer->Add(iconBitmap, 0, wxCENTER | wxSHAPED | wxALL, 15);
	headerSizer->Add(HabitName, 0, wxALIGN_CENTER_VERTICAL);
	headerSizer->Add(gearBitmap, 0, wxCENTER | wxALL, 5);
	headerSizer->AddStretchSpacer();
	headerSizer->Add(streakVertical, 0, wxCENTER | wxALL, 5);

	previewSizer->Add(headerSizer, 0, wxEXPAND);
	previewSizer->AddStretchSpacer();

	HabitName->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
	iconBitmap->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);

	HabitName->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
	iconBitmap->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);

	HabitName->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnNameDoubleClick, this);


	sizer->Add(previewPanel, 1, wxEXPAND | wxALL, 25);

	notSidePanel->SetSizerAndFit(sizer);
	notSidePanel->Refresh();
	notSidePanel->Layout();
}

void MainFrame::AddHabit()
{
	Habit newHabit = Habit("New Habit", "no description", "./Icons/alarm-clock.png");

	habits.push_back(newHabit);
	AddHabitToPanel(habits.back());  // Pass reference to the last element
}

void MainFrame::AddHabitToPanel(Habit& habit)
{
	//if (activeHabit != nullptr) wxLogStatus("Last Habit name: " + activeHabit->name);
	//wxLogStatus(habit.name);
	HabitPanel* habitpanel = new HabitPanel(scrollPanel, wxDefaultPosition, wxDefaultSize, habit.name, habit.description, habit.iconpath, count, this);
	scrollPanelSizer->Add(habitpanel, 0, wxEXPAND);
	habitPanels.push_back(habitpanel);


	scrollPanel->FitInside();
	scrollPanel->Layout();
}

void MainFrame::UpdateHabitPreview()
{
	if (activeHabit == nullptr) {
		previewPanel->Show(false);
		wxLogStatus("Updated preview and it wass a nullptr");
	}
	else {
		previewPanel->Show(true);
		HabitName->SetLabel(activeHabit->name);
		icon = activeHabit->originalBitmap;
		icon.Rescale(icon, wxSize(50, 50));
		iconBitmap->SetBitmap(icon);
		streakText->SetLabel(to_string(activeHabit->currentStreak));
		bestStreakText->SetLabel("Current best: " + to_string(activeHabit->bestStreak));

		if (activeHabit->currentStreak > 0) {
			streakBitmap->SetBitmap(activeStreak);
		}
		else {
			streakBitmap->SetBitmap(dormantStreak);
		}

		wxLogStatus("Updated preview and it wass a thing, active habit was " + activeHabit->name);
	}

	previewPanel->Layout();
	previewPanel->Refresh();
	notSidePanel->Layout();  // Ensure parent updates too
	Refresh();
}

void MainFrame::OnHover(wxMouseEvent& evt)
{
	SetCursor(wxCURSOR_HAND);
	evt.Skip();
}

void MainFrame::OnLeave(wxMouseEvent& evt)
{
	SetCursor(wxCURSOR_DEFAULT);
	evt.Skip();
}

void MainFrame::OnNameDoubleClick(wxMouseEvent& evt)
{
	RenameDialog* dialog = new RenameDialog(previewPanel, this);

	dialog->ShowModal();
	evt.Skip();
}
