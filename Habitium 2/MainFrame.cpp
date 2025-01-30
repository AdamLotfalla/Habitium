#include "MainFrame.h"
#include <wx/splitter.h>
#include "CustomButton.h"
#include "HabitPanel.h"
#include "RenameDialog.h"
#include <wx/statline.h>
#include <wx/wrapsizer.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>

enum
{
	ID_Quit = wxID_HIGHEST + 1
};

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
	//CreateStatusBar();
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
	notSidePanelSizer = new wxBoxSizer(wxVERTICAL);
	previewPanel = new wxPanel(notSidePanel);
	wxFont font = wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY);
	wxBoxSizer* previewSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBitmap trash = wxBitmap("./Icons/trash.png", wxBITMAP_TYPE_PNG);
	icon = (activeHabit == nullptr) ? wxBitmap("./Icons/grocery-bag.png", wxBITMAP_TYPE_PNG) : activeHabit->icon;
	trash.Rescale(trash, wxSize(20, 20));
	icon.Rescale(icon, wxSize(50, 50));

	iconBitmap = new wxStaticBitmap(previewPanel, wxID_ANY, icon);
	wxStaticBitmap* trashBitmap = new wxStaticBitmap(previewPanel, wxID_ANY, trash);
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
	headerSizer->AddSpacer(1);
	headerSizer->Add(trashBitmap, 0, wxCENTER | wxALL, 5);
	headerSizer->AddStretchSpacer();
	headerSizer->Add(streakVertical, 0, wxCENTER | wxALL, 5);

	previewSizer->Add(headerSizer, 0, wxEXPAND);

	HabitName->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
	trashBitmap->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
	iconBitmap->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);


	HabitName->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
	trashBitmap->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
	iconBitmap->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);

	trashBitmap->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnTrashClick, this);
	iconBitmap->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnIconClick, this);


	HabitName->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnNameDoubleClick, this);


	notSidePanelSizer->Add(previewPanel, 1, wxEXPAND | wxALL, 25);



	// Part 2 | Description

	wxStaticLine* line = new wxStaticLine(previewPanel/*, wxID_ANY, wxDefaultPosition, wxSize(650, -1)*/);
	wxStaticText* DescriptionTitle = new wxStaticText(previewPanel, wxID_ANY, "Description: ");
	font = wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY);
	descriptionPanel = new wxPanel(previewPanel);
	wxBoxSizer* descriptionSizer = new wxBoxSizer(wxVERTICAL);
	description = new wxStaticText(descriptionPanel, wxID_ANY, "No description", wxDefaultPosition, wxSize(300,-1));

	DescriptionTitle->SetFont(font);
	DescriptionTitle->SetBackgroundColour(previewPanel->GetBackgroundColour());
	line->SetBackgroundColour(previewPanel->GetBackgroundColour());
	DescriptionTitle->SetForegroundColour(wxColor("#A7A7A7"));
	description->SetBackgroundColour(wxColor("#FCFCFC"));
	descriptionSizer->Add(description, 0, wxEXPAND | wxALL, 10);  
	descriptionPanel->SetSizerAndFit(descriptionSizer);
	descriptionPanel->SetBackgroundColour(wxColor("#FCFCFC"));
	previewPanel->Refresh();


	previewSizer->AddSpacer(10);
	previewSizer->Add(line, 0, wxEXPAND | wxLEFT | wxRIGHT, 50);
	previewSizer->AddSpacer(30);
	previewSizer->Add(DescriptionTitle, 0, wxEXPAND);
	previewSizer->Add(descriptionPanel, 0, wxEXPAND | wxALL, 10);

	description->Update();
	description->Refresh();
	description->Layout();

	description->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
	descriptionPanel->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
	description->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
	descriptionPanel->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
	descriptionPanel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnDescriptionClick, this);
	description->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnDescriptionClick, this);

	previewSizer->AddStretchSpacer();


	// message
	messagePanel = new wxPanel(notSidePanel);
	wxBoxSizer* messageSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* headerMessage = new wxStaticText(messagePanel, wxID_ANY, "No habit selected..");
	wxStaticText* bodyMessage = new wxStaticText(messagePanel, wxID_ANY, "Select a habit from side panel to preview here");

	font = wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY);
	headerMessage->SetFont(font);
	headerMessage->SetForegroundColour(wxColor("#555555"));
	font = wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	bodyMessage->SetFont(font);
	bodyMessage->SetForegroundColour(wxColor("#a7a7a7"));

	messagePanel->SetBackgroundColour(notSidePanel->GetBackgroundColour());

	//messageSizer->AddStretchSpacer();
	messageSizer->Add(headerMessage, 0, wxCENTER);
	messageSizer->AddSpacer(5);
	messageSizer->Add(bodyMessage, 0, wxCENTER);
	//messageSizer->AddStretchSpacer();

	messagePanel->SetSizerAndFit(messageSizer);
	//messagePanel->CenterOnParent();
	//messagePanel->SetPosition(wxPoint(notSidePanel->GetSize().x / 2 - messagePanel->GetSize().x / 2, notSidePanel->GetSize().y / 2));
	//messagePanel->SetSize(notSidePanel->GetSize());
	notSidePanelSizer->AddStretchSpacer();
	notSidePanelSizer->Add(messagePanel, 0, wxCENTER);
	notSidePanelSizer->AddStretchSpacer();

	messagePanel->Refresh();
	messagePanel->Lower();


	notSidePanel->SetSizerAndFit(notSidePanelSizer);
	notSidePanel->Refresh();
	notSidePanel->Layout();
	previewPanel->Layout();
}

void MainFrame::SetuptNotification()
{
	m_taskBarIcon = new wxTaskBarIcon();
	m_taskBarIcon->SetIcon(wxIcon("./Icons/alarm-clock.png"), "Habitium");

	m_timer.SetOwner(this);

	// Bind events using the Bind method
	Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);  // Timer event
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, /*wxID_Quit*/ 6000);  // Quit event

	// Example: Schedule notification for 2 seconds from now (you can customize the time)
	m_notificationTime = wxDateTime::Now().Add(wxTimeSpan(0, 0, 2, 0)); // 2 minutes later
	ScheduleNotification();

	// Frame settings (can be hidden)
	SetSize(200, 100);
	Centre();
}

void MainFrame::ShowNotification(const wxString& message)
{
	m_taskBarIcon->ShowBalloon("Reminder", message, 10000, wxICON_INFORMATION);
}

void MainFrame::ScheduleNotification()
{
	// Calculate time until the next scheduled notification
	wxDateTime now = wxDateTime::Now();
	if (m_notificationTime.IsEarlierThan(now))
	{
		m_notificationTime = now.Add(wxTimeSpan(0, 0, 10, 0));  // Reset to 10 minutes from now if it's passed
	}

	wxTimeSpan timeToWait = m_notificationTime.Subtract(now);
	m_timer.Start((timeToWait.GetMilliseconds()).GetValue(), wxTIMER_ONE_SHOT);
}

void MainFrame::OnTimer(wxTimerEvent& event)
{
	ShowNotification("It's time for your scheduled task!");
	ScheduleNotification();  // Reschedule for the next notification
}

void MainFrame::OnQuit(wxCommandEvent& event)
{
	m_timer.Stop();
	m_taskBarIcon->RemoveIcon();
	Close(true);
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
	if (activeHabitPanel == nullptr) {
		previewPanel->Show(false);
		wxLogStatus("Updated preview and it wass a nullptr");
	}
	else {
		messagePanel->Show(false);
		notSidePanelSizer->Clear();
		notSidePanelSizer->Add(previewPanel, 1, wxEXPAND | wxALL, 25);
		previewPanel->Show(true);
		HabitName->SetLabel(activeHabitPanel->habit.name);
		description->SetLabel(activeHabitPanel->habit.description);
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

		descriptionPanel->Fit();
		descriptionPanel->Layout();
		descriptionPanel->GetSizer()->Layout();  // Ensure the sizer is updated
		descriptionPanel->Refresh();
		previewPanel->Layout();  // Update layout of the previewPanel as well
		previewPanel->Refresh();  // Redraw the previewPanel



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
	hovering = true;
	evt.Skip();
}

void MainFrame::OnLeave(wxMouseEvent& evt)
{
	//if(!hovering) 
	SetCursor(wxCURSOR_DEFAULT);
	hovering = false;
	evt.Skip();
}

void MainFrame::OnNameDoubleClick(wxMouseEvent& evt)
{
	RenameDialog* dialog = new RenameDialog(previewPanel, this);

	dialog->ShowModal();
	evt.Skip();
}

void MainFrame::OnIconClick(wxMouseEvent& evt)
{
	iconDialog = new wxDialog(this, wxID_ANY, "Change Icon", wxDefaultPosition, wxSize(332,250));
	wxScrolledWindow* iconScroll = new wxScrolledWindow(iconDialog, wxID_ANY);
	wxWrapSizer* iconSizer = new wxWrapSizer(wxHORIZONTAL);

	wxString folderPath = "./Icons";

	wxDir dir(folderPath);
	if (!dir.IsOpened()) {
		wxLogError("Failed to open directory: %s", folderPath);
		return;
	}

	wxString filename;
	bool cont = dir.GetFirst(&filename, "*.png", wxDIR_FILES);
	while (cont) {
		wxLogStatus(filename);
		wxString path = "./Icons/" + filename;
		wxBitmap bitmap = wxBitmap("./Icons/" + filename, wxBITMAP_TYPE_PNG);
		bitmap.Rescale(bitmap, wxSize(35, 35));
		wxStaticBitmap* staticBitmap = new wxStaticBitmap(iconScroll, wxID_ANY, bitmap);
		staticBitmap->SetToolTip(filename);
		staticBitmap->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnHover, this);
		staticBitmap->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnLeave, this);
		staticBitmap->Bind(wxEVT_LEFT_DOWN, [this, path](wxMouseEvent& evt) {
			OnIconDialogChoose(evt, path);
			});
		iconSizer->Add(staticBitmap, wxCENTER | wxALL, 5);
		cont = dir.GetNext(&filename);
	}

	iconScroll->SetSizerAndFit(iconSizer);
	iconDialog->Layout();
	iconDialog->ShowModal();
}

void MainFrame::OnTrashClick(wxMouseEvent& evt)
{
	wxMessageDialog* confirmDialog = new wxMessageDialog(this, "Are you sure you want to delete this habit?", "Delete habit", wxYES_NO);
	int result = confirmDialog->ShowModal();
	if (result == wxID_YES) {
		auto it = find(habitPanels.begin(), habitPanels.end(), activeHabitPanel);
		habitPanels.erase(it);

		loadHabits();
		previewPanel->Show(false);
		notSidePanelSizer->Clear();
		notSidePanelSizer->AddStretchSpacer();
		notSidePanelSizer->Add(messagePanel, 0, wxCENTER);
		notSidePanelSizer->AddStretchSpacer();
		messagePanel->Show(true);
		activeHabitPanel->Destroy();
		activeHabitPanel = nullptr;
	}
	else {
		confirmDialog->EndModal(true);
	}
}

void MainFrame::OnDescriptionClick(wxMouseEvent& evt)
{
	descriptionDialog = new wxDialog(this, wxID_ANY, "New description");
	descriptionDialogtextInput = new wxTextCtrl(descriptionDialog, wxID_ANY, "", wxDefaultPosition, wxSize(400, 25));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* confirmButton = new wxButton(descriptionDialog, wxID_OK, "Confirm");
	wxButton* cancelButton = new wxButton(descriptionDialog, wxID_CANCEL, "Cancel");

	descriptionDialogtextInput->SetWindowStyle(wxTE_PROCESS_ENTER);
	descriptionDialogtextInput->Bind(wxEVT_KEY_DOWN, &MainFrame::onKeyEnter, this);

	descriptionDialogtextInput->SetHint("Enter new description..");

	if (description->GetLabel() != "no description") {
		descriptionDialogtextInput->SetValue(description->GetLabel());
		descriptionDialogtextInput->Update();
	}

	sizer->Add(descriptionDialogtextInput, 1, wxEXPAND | wxALL, 5);
	sizer->Add(buttonSizer, 0, wxEXPAND | wxRIGHT | wxDOWN, 5);
	
	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(confirmButton, 0, wxEXPAND);
	buttonSizer->Add(cancelButton, 0, wxEXPAND);

	descriptionDialog->SetSizerAndFit(sizer);

	descriptionDialog->Layout();
	int result = descriptionDialog->ShowModal();

	if (result == wxID_OK) {

		activeHabitPanel->UpdatePanel(
			activeHabitPanel->habit.name,
			descriptionDialogtextInput->GetValue(),
			activeHabitPanel->habit.originalBitmap,
			activeHabitPanel->habit.frequency,
			activeHabitPanel->habit.notification
		);

		UpdateHabitPreview();
		descriptionDialog->Destroy();
	}
	else if (result == wxID_CANCEL) {

		descriptionDialog->Destroy();
	}

	wxLogStatus("New description: " + activeHabitPanel->habit.description);
}

void MainFrame::OnIconDialogChoose(wxMouseEvent& evt, wxString path)
{
	activeHabitPanel->UpdatePanel(
		activeHabitPanel->habit.name,
		activeHabitPanel->habit.description,
		wxBitmap(path, wxBITMAP_TYPE_PNG),
		activeHabitPanel->habit.frequency,
		activeHabitPanel->habit.notification
	);

	iconDialog->Destroy();

	UpdateHabitPreview();
}

void MainFrame::onKeyEnter(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_NUMPAD_ENTER || evt.GetKeyCode() == WXK_RETURN) {


		activeHabitPanel->UpdatePanel(
			activeHabitPanel->habit.name,
			descriptionDialogtextInput->GetValue(),
			activeHabitPanel->habit.originalBitmap,
			activeHabitPanel->habit.frequency,
			activeHabitPanel->habit.notification
		);

		UpdateHabitPreview();
		descriptionDialog->Destroy();

	}

	evt.Skip();
}
