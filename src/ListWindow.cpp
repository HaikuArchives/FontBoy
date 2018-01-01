// ListWindow

#include "Fontboy.h"
#include "ListView.h"
#include "ListWindow.h"
#include "MsgVals.h"


ListWindow::ListWindow(BRect rect)
			  :BWindow(rect, "Fontboy", B_DOCUMENT_WINDOW, 0),
	fontview(NULL)
{
	BMenu		*menu;
	BMenuBar	*menu_bar;
	BMenuItem	*item;

	rect.Set(0, 0, 32767, 15);
	menu_bar = new BMenuBar(rect, "Menu");

	menu = new BMenu("File");
	menu->AddItem(item = new BMenuItem("Page Setup" B_UTF8_ELLIPSIS, new BMessage(M_PRINT), 'P', B_SHIFT_KEY));
	item->SetEnabled(false);
	menu->AddItem(item = new BMenuItem("Print" B_UTF8_ELLIPSIS, new BMessage(M_PRINT), 'P'));
	item->SetEnabled(false);
	menu->AddSeparatorItem();
	menu->AddItem(item = new BMenuItem("About Fontboy" B_UTF8_ELLIPSIS, new BMessage(B_ABOUT_REQUESTED)));
	menu->AddSeparatorItem();
	menu->AddItem(item = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	item->SetTarget(be_app);
	menu_bar->AddItem(menu);

	menu = new BMenu("Edit");

	menu->AddItem(item = new BMenuItem("Settings" B_UTF8_ELLIPSIS, new BMessage(M_SETTINGS), ','));
	item->SetTarget(be_app);
	menu_bar->AddItem(menu);


	menu = new BMenu("View");
	menu->AddItem(item = new BMenuItem("Details" B_UTF8_ELLIPSIS, new BMessage(M_DETAILS), 'D'));
	item->SetTarget(be_app);
	menu->AddSeparatorItem();
	menu->AddItem(item = new BMenuItem("Refresh", new BMessage(M_REFRESH), 'R'));
	menu_bar->AddItem(menu);

	AddChild(menu_bar);

	float mbHeight = menu_bar->Bounds().bottom;
	BRect fontframe = Bounds();
	fontframe.top = mbHeight + 1.0;
	fontframe.right -= B_V_SCROLL_BAR_WIDTH;
	fontframe.bottom -= B_H_SCROLL_BAR_HEIGHT;
	fontview = new ListView(fontframe);
	AddChild(new BScrollView(B_EMPTY_STRING, fontview, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, true, true));

	fontview->Init();
	ResizeTo(Bounds().Width(), mbHeight + fontview->Height());
}

ListWindow::~ListWindow()
{
}

bool ListWindow::QuitRequested()
{
	BMessage msg(M_MAINQUIT);
	msg.AddRect("rect", Frame());
	
	be_app->PostMessage(&msg);

	return true;
}

void ListWindow::Show()
{
	fontview->UpdateScrollBar();
	BWindow::Show();
}

void ListWindow::MessageReceived(BMessage* msg)
{

	switch(msg->what) {
		case B_ABOUT_REQUESTED:
			dynamic_cast<Fontboy*>(be_app)->ShowSplashScreen(false);
			break;

		case M_REFRESH:
			dynamic_cast<Fontboy*>(be_app)->RefreshFontList();
		case M_APPLY:
			fontview->Init();
			fontview->Invalidate();
			break;

		default:
			BWindow::MessageReceived(msg);
	}
}
