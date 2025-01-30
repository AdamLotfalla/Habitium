#pragma once
#include <wx/wx.h>

struct Habit {
	wxString name;
	wxString description;
	wxString iconpath;
	int currentStreak;
	int bestStreak;
	int frequency;
	bool notification;
	bool checked = false;
	wxBitmap originalBitmap;
	wxBitmap icon;

	Habit(wxString NAME, wxString DESCRIPTION, wxString ICONPATH) {
		name = NAME;
		description = DESCRIPTION;
		iconpath = ICONPATH;
		currentStreak = 0;
		bestStreak = 0;
		frequency = 1;
		notification = true;

		originalBitmap = wxBitmap(ICONPATH, wxBITMAP_TYPE_ANY);
		icon = originalBitmap;
		icon.Rescale(icon, wxSize(25, 25));
	}
};