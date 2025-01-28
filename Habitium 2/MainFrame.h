#pragma once
#include <wx/wx.h>
#include "Habit.h"
#include <deque>

using namespace std;

struct MainFrame : public wxFrame
{
	wxPanel* sidePanel;
	wxPanel* notSidePanel;

	wxScrolledWindow* scrollPanel;
	wxBoxSizer* scrollPanelSizer;

	int count;
	wxFont Habitfont;
	wxColor color;

	deque<Habit> habits;

	MainFrame(wxString TITLE);

	//creating layout
	void makeSidePanel();
	void sidePanelContents();
	void loadHabits();


	//events
	void AddHabit();
	void AddHabitToPanel(Habit habit);
};

