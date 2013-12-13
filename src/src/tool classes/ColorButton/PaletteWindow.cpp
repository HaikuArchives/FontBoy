#include "PaletteWindow.h"
#include <string.h>
#include <Debug.h>

#define Debug 0

PaletteWindow::PaletteWindow(BPoint point, BHandler *target_i)
			 :BWindow(BRect(point.x, point.y, point.x+100, point.y+100), "Palette", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS),
			  target(target_i)
{
	BBox *box = new BBox(Bounds(), NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP, B_PLAIN_BORDER);
	AddChild(box);

	color_ctrl = new CControl(BPoint(5, 5), new BMessage(M_PALETTE));
	box->AddChild(color_ctrl);
	color_ctrl->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	color_ctrl->SetTarget(target);
	int32 c = (ui_color(B_PANEL_BACKGROUND_COLOR).red << 24)
			+ (ui_color(B_PANEL_BACKGROUND_COLOR).green << 16)
			+ (ui_color(B_PANEL_BACKGROUND_COLOR).blue << 8);
	color_ctrl->SetValue(c);
	ResizeTo(color_ctrl->Bounds().Width() + 10, color_ctrl->Bounds().Height() + 10);

	BScreen screen(this);
	if (!screen.Frame().Contains(Frame())) {
		BRect scrrect = screen.Frame();
		BRect winrect = Frame();
		float offx = 0;
		float offy = 0;
		if (winrect.left < scrrect.left) {
			offx = scrrect.left - winrect.left;
		}
		else {
			if (winrect.right > scrrect.right) {
				offx = scrrect.right - winrect.right;
			}
		}
		if (winrect.top < scrrect.top) {
			offy = scrrect.top - winrect.top;
		}
		else {
			if (winrect.bottom > scrrect.bottom) {
				offy = scrrect.bottom - winrect.bottom;
			}
		}
//		MoveTo(winrect.left + offx, offy);
		MoveTo(winrect.left + offx, winrect.top + offy);
		if (Debug) {
			_sPrintf("intersection!\n");
			_sPrintf("offx: %f!\n", offx);
			_sPrintf("offy: %f!\n", offy);
			_sPrintf("old winrect - x1: %f, y1: %f, x2: %f, y2: %f\n", winrect.left, winrect.top, winrect.right, winrect.bottom);
			_sPrintf("new winrect - x1: %f, y1: %f, x2: %f, y2: %f\n", winrect.left + offx, winrect.top + offy, winrect.right + offx, winrect.bottom + offy);
		}
	}
}

bool PaletteWindow::QuitRequested(void)
{
	status_t status;

	BMessenger msgr(target);
	status = msgr.SendMessage(M_PALETTE_QUIT);
	if (status != B_OK) {
		_sPrintf("Error sendging M_CONFIG_QUIT: %s\n", strerror(status));
	}
	return true;
}

void PaletteWindow::SetColor(rgb_color color)
{
	int32 c = (color.red << 24)
			+ (color.green << 16)
			+ (color.blue << 8);
	color_ctrl->SetValue(c);
}

/* Straightforward constructor */
CControl::CControl(BPoint start, BMessage *message) :
			BColorControl(start, B_CELLS_32x8, 8.0, "", message)
{
}

/* We overwrite SetValue to send a message to the target everytime
   the setting change and not only at the end. */
void CControl::SetValue(int32 color_value)
{
	BHandler		*handler;

	BColorControl::SetValue(color_value);
	handler = Target();
	if (handler) {
		BMessage msg;
		msg.AddInt32("be:value", color_value);
		handler->Looper()->PostMessage(&msg, handler);
	}
}					

