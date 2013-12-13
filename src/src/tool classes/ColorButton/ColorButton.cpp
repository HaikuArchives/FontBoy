#include "PaletteWindow.h"
#include "ColorButton.h"
#include <stdlib.h>
#include <string.h>
#include <Debug.h>

static PaletteWindow	*m_cc_window;

ColorButton::ColorButton(BRect frame, const char* name, const char* label, uint32 resize_mask, uint32 flags, const rgb_color *color, uint32 command)
			:BView(frame,name,resize_mask,flags)
{
	if (color == NULL)
	{
		rgb_color col = { 0, 0, 0 };
		color = &col;
	}

	m_command = command;
	m_divider = 100;
	m_cc_window = NULL;

//	m_background_color = ui_color(B_PANEL_BACKGROUND_COLOR);
	m_background_color.red = color->red;
	m_background_color.green = color->green;
	m_background_color.blue = color->blue;
	m_background_color.alpha = color->alpha;

	m_dark_1_color = tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_1_TINT);
	m_dark_2_color = tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),B_DARKEN_4_TINT);
	m_cached_bounds = Bounds();
	m_divider = m_cached_bounds.right / 2;
	m_cached_bounds.left = m_divider;

/*	m_offview = new BView(m_cached_bounds, "", B_FOLLOW_NONE, B_WILL_DRAW);
	m_offview->Hide();
	AddChild(m_offview);
	m_offview->ResizeTo(m_cached_bounds.Width(), m_cached_bounds.Height());
*/
	if(label)
	{
		m_label = new char[strlen(label)+1];
		strcpy((char*)m_label,label);
	}
	else
	{
		m_label = NULL;
	}
}

ColorButton::~ColorButton()
{
	if(m_label)
	{
		delete[] m_label;
	}
	if (m_cc_window != NULL)
	{
		m_cc_window->Quit();
	}
}

void ColorButton::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void ColorButton::SetDivider(float xCoordinate)
{
	m_divider = xCoordinate;
	m_cached_bounds.left = m_divider;
}

float ColorButton::Divider() const
{
	return m_divider;
}

rgb_color ColorButton::ValueAsColor() const
{
	return m_background_color;
}

void ColorButton::SetValue(rgb_color color)
{
	m_background_color = color;
	Invalidate(m_cached_bounds);
}

void ColorButton::SetValue(int32 color)
{
	m_background_color.red = (color >> 24);
	m_background_color.green = (color >> 16);
	m_background_color.blue = (color >> 8);
	m_background_color.alpha = color;
	Invalidate(m_cached_bounds);
}

void ColorButton::MouseDown(BPoint point)
{
	BRect			frame;

	if (m_cc_window == NULL)
	{
		point += BPoint(30, 30);
		ConvertToScreen(&point);
		frame.Set(point.x, point.y, point.x + 309.0, point.y + 209.0);
		m_cc_window = new PaletteWindow(point, this);
		m_cc_window->SetColor(m_background_color);
		m_cc_window->Show();
	}
	else
	{
		m_cc_window->Lock();
		m_cc_window->SetColor(m_background_color);
		m_cc_window->Unlock();
	}
	m_cc_window->Activate();
}

void ColorButton::MessageReceived(BMessage *msg)
{
	int32			color;

	switch (msg->what)
	{
		case M_PALETTE:
			msg->FindInt32("be:value", &color);
			SetValue(color);
			Looper()->PostMessage(m_command);
			break;

		case M_PALETTE_QUIT:
			m_cc_window = NULL;
			break;

		default:
			BView::MessageReceived(msg);
			break;
	}
}

void ColorButton::Draw(BRect update_rect)
{
	BPoint	text_point;
	BFont	label_font;

	if(m_label)
	{
		//Draw the label
		SetDrawingMode(B_OP_OVER);
		GetFont(&label_font);
		text_point.Set(0, 11);			
		DrawString(m_label,text_point);
	}

/*	if (!m_pict) {
_sPrintf("begin pict\n");
		m_pict = new BPicture();
		BeginPicture(m_pict);
		SetHighColor(m_dark_1_color);
		StrokeLine(BPoint(m_cached_bounds.left,m_cached_bounds.top),BPoint(m_cached_bounds.right,
			m_cached_bounds.top));
		StrokeLine(BPoint(m_cached_bounds.left,m_cached_bounds.top+1),BPoint(m_cached_bounds.left,
			m_cached_bounds.bottom));
		SetHighColor(White);
		StrokeLine(BPoint(m_cached_bounds.right,m_cached_bounds.top+1),BPoint(m_cached_bounds.right,
			m_cached_bounds.bottom-1));
		StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.bottom),BPoint(m_cached_bounds.right,
			m_cached_bounds.bottom));
		SetHighColor(m_dark_2_color);
		StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.top+1),BPoint(m_cached_bounds.right-1,
			m_cached_bounds.top+1));
		StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.top+2),BPoint(m_cached_bounds.left+1,
			m_cached_bounds.bottom-1));
		SetHighColor(m_background_color);
	
		StrokeLine(BPoint(m_cached_bounds.left+2,m_cached_bounds.bottom-1),BPoint(m_cached_bounds.right-1,
			m_cached_bounds.bottom-1));
		StrokeLine(BPoint(m_cached_bounds.right-1,m_cached_bounds.top+2),BPoint(m_cached_bounds.right-1,
			m_cached_bounds.bottom-2));
		FillRect(BRect(m_cached_bounds.left+2,m_cached_bounds.top+2, m_cached_bounds.right-2, m_cached_bounds.bottom-2));
	}
	else {
_sPrintf("use pict\n");
		DrawPicture(m_pict);
	}
*/
	SetHighColor(m_dark_1_color);
	StrokeLine(BPoint(m_cached_bounds.left,m_cached_bounds.top),BPoint(m_cached_bounds.right,
		m_cached_bounds.top));
	StrokeLine(BPoint(m_cached_bounds.left,m_cached_bounds.top+1),BPoint(m_cached_bounds.left,
		m_cached_bounds.bottom));
	SetHighColor(White);
	StrokeLine(BPoint(m_cached_bounds.right,m_cached_bounds.top+1),BPoint(m_cached_bounds.right,
		m_cached_bounds.bottom-1));
	StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.bottom),BPoint(m_cached_bounds.right,
		m_cached_bounds.bottom));
	SetHighColor(m_dark_2_color);
	StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.top+1),BPoint(m_cached_bounds.right-1,
		m_cached_bounds.top+1));
	StrokeLine(BPoint(m_cached_bounds.left+1,m_cached_bounds.top+2),BPoint(m_cached_bounds.left+1,
		m_cached_bounds.bottom-1));
	SetHighColor(m_background_color);
	StrokeLine(BPoint(m_cached_bounds.left+2,m_cached_bounds.bottom-1),BPoint(m_cached_bounds.right-1,
		m_cached_bounds.bottom-1));
	StrokeLine(BPoint(m_cached_bounds.right-1,m_cached_bounds.top+2),BPoint(m_cached_bounds.right-1,
		m_cached_bounds.bottom-2));
	FillRect(BRect(m_cached_bounds.left+2,m_cached_bounds.top+2, m_cached_bounds.right-2, m_cached_bounds.bottom-2));
}	

void ColorButton::FrameResized(float width, float height)
{
	BRect new_bounds = Bounds();
	new_bounds.left = m_divider;
	float min_x = new_bounds.right;
	if(min_x > m_cached_bounds.right)
		min_x = m_cached_bounds.right;
	float max_x = new_bounds.right;
	if(max_x < m_cached_bounds.right)
		max_x = m_cached_bounds.right;
	float min_y = new_bounds.bottom;
	if(min_y > m_cached_bounds.bottom)
		min_y = m_cached_bounds.bottom;
	float max_y = new_bounds.bottom;
	if(max_y < m_cached_bounds.bottom)
		max_y = m_cached_bounds.bottom;
	if(min_x != max_x)
		Invalidate(BRect(min_x-1,new_bounds.top,max_x,max_y));
	if(min_y != max_y)
		Invalidate(BRect(new_bounds.left,min_y-1,max_x,max_y));
}
