#pragma once
#include <wx/wx.h>
#include "Habit.h"
#include <deque>
#include "HabitPanel.h"
#include <wx/taskbar.h>
#include <wx/datetime.h>



using namespace std;

struct MainFrame : public wxFrame
{
	wxPanel* sidePanel;
	wxPanel* notSidePanel;

	wxPanel* previewPanel;
	wxPanel* messagePanel;
	wxStaticText* HabitName;
	wxBoxSizer* notSidePanelSizer;

	wxScrolledWindow* scrollPanel;
	wxBoxSizer* scrollPanelSizer;
	Habit* activeHabit = nullptr;
	HabitPanel* activeHabitPanel = nullptr;

	wxPanel* descriptionPanel;
	wxStaticText* description;

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
	bool hovering;

	deque<Habit> habits;
	deque<HabitPanel*> habitPanels;
	wxDialog* iconDialog;

	MainFrame(wxString TITLE);

	void ShowNotification(const wxString& message);
	void ScheduleNotification();

	wxTaskBarIcon* m_taskBarIcon;
	wxTimer m_timer;
	wxDateTime m_notificationTime;

	wxDialog* descriptionDialog;
	wxTextCtrl* descriptionDialogtextInput;

	void OnTimer(wxTimerEvent& event);
	void OnQuit(wxCommandEvent& event);

	//creating layout
	void makeSidePanel();
	void sidePanelContents();
	void loadHabits();
	void makePreviewPanel();

	// notifications
	void SetuptNotification();


	//events
	void AddHabit();
	void AddHabitToPanel(Habit& habit);
	void UpdateHabitPreview();

	//editing habits
	void OnHover(wxMouseEvent& evt);
	void OnLeave(wxMouseEvent& evt);
	void OnNameDoubleClick(wxMouseEvent& evt);
	void OnIconClick(wxMouseEvent& evt);
	void OnTrashClick(wxMouseEvent& evt);
	void OnDescriptionClick(wxMouseEvent& evt);
	void OnIconDialogChoose(wxMouseEvent& evt, wxString path);
	void onKeyEnter(wxKeyEvent& evt);

};

