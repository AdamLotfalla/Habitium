#pragma once
#include <wx/wx.h>

struct Habit {
	wxString name;
	wxString description;
	int currentStreak;
	int bestStreak;
	int frequency;
	bool notification;
	wxBitmap icon;

	Habit(wxString NAME, wxString DESCRIPTION, wxString ICONPATH) {
		name = name;
		description = DESCRIPTION;
		currentStreak = 0;
		bestStreak = 0;
		frequency = 1;
		notification = true;

		icon = wxBitmap(ICONPATH, wxBITMAP_TYPE_ANY);
		icon.Rescale(icon, wxSize(25, 25));
	}
};