#pragma once
#include <wx/wx.h>
#include "Habit.h"

struct MainFrame;

struct HabitPanel : public wxPanel
{
	Habit habit = Habit("New Habit", "no description", "./Icons/alarm-clock.png");
	wxBoxSizer* sizer;
	wxStaticText* text;
	wxStaticBitmap* icon;
	wxColor color;
	wxColor hoverBufferColor;
	wxFont Habitfont = wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD, false);

	MainFrame* mainframe = nullptr;
	wxCheckBox* checkBox;

	bool habitHover;
	bool isSelected = false;
	bool inBestStreak;
	wxPanel* panel;
	wxBoxSizer* checkboxSizer;


	HabitPanel(wxWindow* parent, wxPoint position, wxSize size, wxString Habitname, wxString Habitdescription, wxString Habiticonpath, int& index, MainFrame* frame);
	void OnHabitHover(wxMouseEvent& evt);
	void OnHabitUnhover(wxMouseEvent& evt);
	void OnHabitClick(wxMouseEvent& evt);
	void OnHabitUnclick(wxMouseEvent& evt);
	void UpdatePanel(wxString newName, wxString newDescpription, wxBitmap newBitmap, int newFrequency, bool newNotification);
	void UpdateColorBasedOnNewIndex(int index);
	void OnChildClick(wxMouseEvent& evt);
	void OnChildHover(wxMouseEvent& evt);
	void OnCheckBox(wxCommandEvent& evt);
};

