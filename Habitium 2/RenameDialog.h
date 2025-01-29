#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

struct MainFrame;

struct RenameDialog : public wxDialog
{
	RenameDialog(wxWindow* parent, MainFrame* frame);

	MainFrame* frameptr = nullptr;

	wxBoxSizer* verticalSizer;
	wxBoxSizer* buttonSizer;
	wxTextCtrl* textInput;
	wxButton* RenameButton;
	wxButton* CancelButton;

	void onRenameButton(wxCommandEvent& evt);
	void onCancelButton(wxCommandEvent& evt);
	void onKeyEnter(wxKeyEvent& evt);
};

