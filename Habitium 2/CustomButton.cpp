#include "CustomButton.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

CustomButton::CustomButton(wxWindow* parent, wxPoint position, wxSize size, wxString text, wxColor FCOLOR, wxColor BCOLOR, int BORDERRADIUS, std::function<void()> HANDLER)
	: wxPanel(parent, wxID_ANY, position, size)
{
	bcolor = BCOLOR;
	fcolor = FCOLOR;
	borderRadius = BORDERRADIUS;
	width = size.GetWidth();
	height = size.GetHeight();
	handler = HANDLER;

	x = position.x;
	y = position.y;

	wxFont font;
	font.MakeBold();
	font.SetPointSize(16);
	font.SetFamily(wxFONTFAMILY_SWISS);
	this->SetWindowStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &CustomButton::paint, this);
	this->Bind(wxEVT_SIZE, &CustomButton::OnChangeSize, this);
	this->SetBackgroundColour(this->GetParent()->GetBackgroundColour());

	textBox = new wxStaticText(this, wxID_ANY, text, position, wxDefaultSize, wxCENTER);
	textBox->CenterOnParent();
	textBox->SetForegroundColour(fcolor);
	textBox->SetBackgroundColour(bcolor);
	textBox->SetFont(font);
	textBox->Fit();

	this->Bind(wxEVT_ENTER_WINDOW, &CustomButton::OnEnter, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &CustomButton::OnLeave, this);
	this->Bind(wxEVT_LEFT_DOWN, &CustomButton::OnClick, this);
	this->Bind(wxEVT_LEFT_UP, &CustomButton::OnUnClick, this);

	textBox->Bind(wxEVT_ENTER_WINDOW, &CustomButton::OnEnter, this);
	textBox->Bind(wxEVT_LEAVE_WINDOW, &CustomButton::OnLeave, this);
	textBox->Bind(wxEVT_LEFT_DOWN, &CustomButton::OnClick, this);
	textBox->Bind(wxEVT_LEFT_UP, &CustomButton::OnUnClick, this);

	this->SetDoubleBuffered(true);
	Refresh();
}

void CustomButton::paint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);

	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc) {
		gc->SetPen(wxPen(fcolor, 0));
		gc->SetBrush(wxBrush(bcolor));
		width = this->GetSize().GetWidth();
		height = this->GetSize().GetHeight();
		textBox->CenterOnParent();
		textBox->SetBackgroundColour(bcolor);
		gc->DrawRoundedRectangle(x, y, width, height, borderRadius);

		delete gc;
	}
}

void CustomButton::OnChangeSize(wxSizeEvent& evt)
{
	this->Refresh();
}

void CustomButton::OnEnter(wxMouseEvent& evt)
{
	if(!this->HasCapture()) this->CaptureMouse();

	hovering = true;
	bcolor = HoverColor;
	Refresh();
	evt.Skip();
}

void CustomButton::OnLeave(wxMouseEvent& evt)
{
	if(this->HasCapture()) this->ReleaseMouse();

	hovering = false;
	bcolor = IdleColor;
	Refresh();
	evt.Skip();
}

void CustomButton::OnClick(wxMouseEvent& evt)
{
	if (!this->HasCapture()) this->CaptureMouse();

	bcolor = ClickColor;
	Refresh();
	handler();
	evt.Skip();
}

void CustomButton::OnUnClick(wxMouseEvent& evt)
{
	//if (this->HasCapture()) this->ReleaseMouse();

	bcolor = hovering ? HoverColor : IdleColor;
	Refresh();
	evt.Skip();
}
