#pragma once
#include <wx/wx.h>
#define HoverColor wxColor("#8F78AB")
#define IdleColor wxColor("#AD93CD")
#define ClickColor wxColor("#373040")

struct CustomButton : public wxPanel
{
	wxColor bcolor;
	wxColor fcolor;
	int borderRadius;
	int width, height;
	int x, y;

	std::function<void()> handler;

	bool hovering;

	wxStaticText* textBox;

	CustomButton(wxWindow* parent, wxPoint position, wxSize size, wxString text, wxColor FCOLOR, wxColor BCOLOR, int BORDERRADIUS, std::function<void()> handler);
	void paint(wxPaintEvent& evt);
	void OnChangeSize(wxSizeEvent& evt);
	void OnEnter(wxMouseEvent& evt);
	void OnLeave(wxMouseEvent& evt);
	void OnClick(wxMouseEvent& evt);
	void OnUnClick(wxMouseEvent& evt);

};

