#include "RenameDialog.h"
#include "MainFrame.h"
#include "HabitPanel.h"

RenameDialog::RenameDialog(wxWindow* parent, MainFrame* frame) : wxDialog(parent, wxID_ANY, "Rename Habit", wxDefaultPosition, wxSize(-1, 150))
{
	frameptr = frame;

	verticalSizer = new wxBoxSizer(wxVERTICAL);
	buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	textInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, 25));
	textInput->SetHint("New name");
	textInput->SetWindowStyle(wxTE_PROCESS_ENTER);

	RenameButton = new wxButton(this, wxID_ANY, "Rename", wxDefaultPosition, wxSize(-1, 25));
	CancelButton = new wxButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxSize(-1, 25));


	verticalSizer->Add(textInput, 0, wxEXPAND | wxALL, 5);
	verticalSizer->AddStretchSpacer();
	verticalSizer->Add(buttonSizer, 0, wxEXPAND | wxRIGHT | wxLEFT | wxDOWN, 5);

	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(RenameButton, 0, wxEXPAND);
	buttonSizer->Add(CancelButton, 0, wxEXPAND);


	RenameButton->Bind(wxEVT_BUTTON, &RenameDialog::onRenameButton, this);
	CancelButton->Bind(wxEVT_BUTTON, &RenameDialog::onCancelButton, this);
	textInput->Bind(wxEVT_KEY_DOWN, &RenameDialog::onKeyEnter, this);

	this->SetSizerAndFit(verticalSizer);
	this->Layout();
	this->Refresh();
}

void RenameDialog::onRenameButton(wxCommandEvent& evt)
{
	frameptr->activeHabit->name = textInput->GetValue();

	frameptr->activeHabitPanel->UpdatePanel(textInput->GetValue(), frameptr->activeHabitPanel->habit.description, frameptr->activeHabitPanel->habit.icon, frameptr->activeHabitPanel->habit.frequency, frameptr->activeHabitPanel->habit.notification);
	frameptr->UpdateHabitPreview();
	frameptr->loadHabits();
	EndModal(true);
}


void RenameDialog::onCancelButton(wxCommandEvent& evt)
{
	EndModal(true);
}

void RenameDialog::onKeyEnter(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_NUMPAD_ENTER || evt.GetKeyCode() == WXK_RETURN) {
		wxCommandEvent dummyEvent;
		onRenameButton(dummyEvent);
	}

	evt.Skip();
}
