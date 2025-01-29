#pragma once
#include <wx/wx.h>
#include "Habit.h"
#include <deque>
#include "HabitPanel.h"

using namespace std;

struct MainFrame : public wxFrame
{
	wxPanel* sidePanel;
	wxPanel* notSidePanel;

	wxPanel* previewPanel;
	wxStaticText* HabitName;

	wxScrolledWindow* scrollPanel;
	wxBoxSizer* scrollPanelSizer;
	Habit* activeHabit = nullptr;
	HabitPanel* activeHabitPanel = nullptr;

	wxBitmap activeStreak;
	wxBitmap dormantStreak;

	wxStaticBitmap* iconBitmap;
	wxStaticText* bestStreakText;
	wxStaticText* streakText;
	wxStaticBitmap* streakBitmap;
	wxBitmap icon;

	int count = 0;
	wxFont Habitfont = wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD, false);
	wxColor color;
	wxColor hoverBufferColor;
	bool habitHover;

	deque<Habit> habits;
	deque<HabitPanel*> habitPanels;

	MainFrame(wxString TITLE);

	//creating layout
	void makeSidePanel();
	void sidePanelContents();
	void loadHabits();
	void makePreviewPanel();


	//events
	void AddHabit();
	void AddHabitToPanel(Habit& habit);
	void UpdateHabitPreview();

	//editing habits
	void OnHover(wxMouseEvent& evt);
	void OnLeave(wxMouseEvent& evt);
	void OnNameDoubleClick(wxMouseEvent& evt);
	void OnIconClick(wxMouseEvent& evt);

};

