// Fontboy About Window

#include "Fontboy.h"
#include "About.h"
#include <Debug.h>

AboutWindow::AboutWindow(int32 needpulse)
:BWindow(BRect(0.0, 0.0, 399.0, 239.0), "Fontboy", B_NO_BORDER_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_MOVABLE | B_NOT_RESIZABLE)
{
	AddChild(new AboutView(Bounds()));

	if (needpulse) {
		const bigtime_t apulse = 1000000;
		SetPulseRate(apulse);
	}
	else
		SetPulseRate(0);
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what) {
		default:
			BWindow::MessageReceived(msg);
	}
}

bool AboutWindow::QuitRequested()
{
	return TRUE;
}

void AboutWindow::Center()
{
	BRect rscreen = BScreen().Frame();
	BRect rabout = Bounds();
	MoveTo((rscreen.right - rabout.right) / 2.0, (rscreen.bottom - rabout.bottom) / 2.0);
}

AboutView::AboutView(BRect frame)
:BView(frame, "About", B_FOLLOW_ALL_SIDES, B_PULSE_NEEDED | B_WILL_DRAW | B_FRAME_EVENTS)
{
	PulseCnt = 0;
}

void AboutView::Draw(BRect urect)
{
	char	atext[32];

	SetLowColor(255, 255, 255, 255);
	SetHighColor(0, 0, 0, 255);
	SetDrawingMode(B_OP_COPY);

/*  mBitmap = BTranslationUtils::GetBitmapFile(path.Path());
  if (mBitmap == NULL)
    return;
*/
	BBitmap *aboutbmp = BTranslationUtils::GetBitmap("aboutpic", NULL);
	if (aboutbmp != NULL) {
		if (aboutbmp->IsValid()) {
			BPoint point(0.0, 70.0);
			DrawBitmap(aboutbmp, point);
			Sync();
		}
		else {
			printf("About Box: Error getting Image!\n");
		}
	}
	BFont font(be_bold_font); 
	font.SetSize(35.0); 
	SetFont(&font);
	SetDrawingMode(B_OP_ALPHA);
	float left = font.StringWidth("F  O  N  T  B  O  Y");
	left = (Bounds().Width() - left) / 2;
	BPoint point(left, 50.0);
	DrawString("F  O  N  T  B  O  Y", point);
	SetFont(be_plain_font);
	GetFont(&font); 
	font.SetSize(12.0); 
	SetFont(&font);
	point.Set(14.0, Bounds().Height() - 23);
	sprintf(atext, "Version %s", VERSION);
	DrawString(atext , point);
	point.Set(14.0, Bounds().Height() - 10);
	DrawString("Copyright 1999-2003 by Oliver 'Madison' Kohl", point);
	StrokeRect(Frame());
}

void AboutView::Pulse()
{
	if (PulseCnt++ > 1)
		Window()->PostMessage(B_QUIT_REQUESTED);
}

void AboutView::MouseDown(BPoint point)
{
	Window()->Quit();
}
