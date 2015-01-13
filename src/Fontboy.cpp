/***************************************************
 Fontboy by Oliver Kohl


***************************************************/

#include "Fontboy.h"
#include "View.h"
#include "PrefsWindow.h"
#include "ListWindow.h"
#include "PropWindow.h"
#include "Prefs.h"
#include "About.h"
#include "MsgVals.h"
#include "FontList.h"

#include <Debug.h>

Fontboy::Fontboy(void)
		:BApplication(APP_SIG),
	mainwindow(NULL),
	prefwindow(NULL),
	proplist(NULL),
	fontlist(NULL)
{
/*	uint32 cache_id;
	font_cache_info cache_info;

struct font_cache_info {
    int32    sheared_font_penalty;
    int32    rotated_font_penalty;
	float    oversize_threshold;
	int32    oversize_penalty;
	int32    cache_size;
	float    spacing_size_threshold;
};

	cache_info.sheared_font_penalty = 0;
	cache_id = B_DEFAULT_CACHE_SETTING;
	
	if (get_font_cache_info(cache_id, &cache_info)) {
		cache_id = B_SCREEN_FONT_CACHE;
	}
*/
	proplist = new BList();
	prefs = new Prefs();

	fontlist = new FontList();

	BRect mrect;
	prefs->GetMainRect(mrect);
	mainwindow = new ListWindow(mrect);
	mainwindow->Show();
};

Fontboy::~Fontboy(void)
{
	PropWindow *pwin;
	BRect rect;

	// Quit all opened detail windows
	if ((proplist != NULL) &&
		(!proplist->IsEmpty())) {
		for (int32 i = 0; (pwin = (PropWindow *)proplist->ItemAt(i)); i++) {
			if ((pwin != NULL) &&
				(B_OK == pwin->Lock())) {
				rect = pwin->Frame();
				pwin->Quit();
//				pwin->Unlock();
			}
		}
		// Save last details window rect
		if (rect.IsValid())
			prefs->SetPropsRect(&rect);
	}

	if (mainwindow != NULL) {
		// Save window positions
		rect = mainwindow->Frame();
		prefs->SetMainRect(&rect);

		if (mainwindow->Lock()) {
			mainwindow->Quit();
//			mainwindow->Unlock();
		}
	}

	if (prefwindow != NULL) {
		rect = prefwindow->Frame();
		prefs->SetPrefsRect(&rect);

		if (prefwindow->Lock()) {
			prefwindow->Quit();
//			prefwindow->Unlock();
		}
	}

	prefs->SavePrefs();

	delete proplist;
	delete fontlist;
	delete prefs;
};

bool Fontboy::QuitRequested(void)
{
	return true;
}

void Fontboy::MessageReceived(BMessage* msg)
{
	PropWindow	*pwin;
	BHandler	*msghandler;
	BRect		rect;
	void		*item;
	int16		sp_pos;
	
	msg->FindPointer("source", (void**)&msghandler);
	switch (msg->what) {
		case M_SETTINGS:
			if (prefwindow == NULL) {
				prefs->GetPrefsRect(rect);
				prefwindow = new PrefsWindow(rect);
				prefwindow->Show();
			}
			else
				prefwindow->Activate();
			break;

		case M_DETAILS:
			//	get frame dimensions
			if ((pwin = (PropWindow *)proplist->LastItem()) != NULL) {
				rect = pwin->Frame();
				rect = rect | BRect(rect.left, rect.top, rect.left + PropWindow::MINIMUM_WIDTH, rect.top + PropWindow::MINIMUM_HEIGHT);
				rect.OffsetBy(22, 22);
			}
			else {
				prefs->GetPropsRect(rect);
				rect = rect | BRect(rect.left, rect.top, rect.left + PropWindow::MINIMUM_WIDTH, rect.top + PropWindow::MINIMUM_HEIGHT);	
			}

			OpenNewPropWindow(rect);
			break;

		case M_APPLYSETTINGS:
			ApplySettings(Prefs::AREA_ALL);
			break;

		case M_MAINQUIT:
			rect = msg->FindRect("rect");
			if (rect.IsValid())
				prefs->SetMainRect(&rect);
			mainwindow = NULL;
			break;

		case M_SETTINGSQUIT:
			msg->FindRect("rect", &rect);
			if (rect.IsValid())
				prefs->SetPrefsRect(&rect);
			prefwindow = NULL;
			break;

		case M_DETAILSQUIT:
			msg->FindRect("rect", &rect);
			if (rect.IsValid()) {
				prefs->SetPropsRect(&rect);
			}

			msg->FindPointer("winptr", &item);
			if (proplist != NULL)
				proplist->RemoveItem(item);

			msg->FindInt16("sppos", &sp_pos);
			prefs->SetSplitPanePos(sp_pos);
			break;

		default:
			BApplication::MessageReceived(msg);
			break;
	}
	
	// Quit when main window was closed
	if (mainwindow == NULL)
		Quit();
}


// create new Property window and put it to the liste of the others
bool Fontboy::OpenNewPropWindow(BRect rect)
{
	PropWindow	*propwindow = NULL;
	fnode		*fontptr;

	if (fontlist == NULL)
		return false;

	fontptr = fontlist->FontAt(fontlist->GetSelected());

	if (fontptr != NULL) {
		propwindow = new PropWindow(rect, fontptr->family, fontptr->style);
		proplist->AddItem(propwindow);
		propwindow->SelectChar(64); //uint8("@"));
		propwindow->Show();
		propwindow->Update();
		return true;
	}
	else {
		(new BAlert("Fontboy", "You have to select a font first!","OK"))->Go();
		return false;
	}
}

// Show centered, borderless Aboutwindow
void Fontboy::ShowSplashScreen(bool mode)
{
	AboutWindow *fmabout = new AboutWindow(mode);
	
	if (fmabout != NULL) {
		fmabout->Center();
		fmabout->Show();
	}
}

FontList* Fontboy::GetFontList()
{
	return fontlist;
}

void Fontboy::RefreshFontList()
{
	if (fontlist != NULL)
		fontlist->Init();
}

void Fontboy::ApplySettings(int prefs_area)
{
	PropWindow	*pwin;

	// recalc settings
	prefs->Update();

	// send update message to fontlist
	if (prefs->IsListWindowForUpdate())
		mainwindow->PostMessage(M_APPLY);

	// send update message to all properties windows
	if (prefs->IsDetailWindowForUpdate()) {
		if ((proplist != NULL) &&
			(!proplist->IsEmpty())) {
			for (int32 i = 0; (pwin = (PropWindow *)proplist->ItemAt(i)); i++)
				pwin->PostMessage(M_PROPWIN_UPDATE);
		}
	}
	prefs->FinishUpdate();
}

int main(void)
{
	Fontboy *app;
	
	app = new Fontboy();
	app->Run();
	
	delete app;

	return B_NO_ERROR;
};

