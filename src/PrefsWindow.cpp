/* Preference Window */

#include "Fontboy.h"
#include "Prefs.h"
#include "PrefsWindow.h"
#include "MsgVals.h"
#include <Debug.h>

PrefsWindow::PrefsWindow(BRect rect)
:BWindow(rect, "Preferences", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS)
{
	BFont			font(be_plain_font);
	BRect			aRect;
	BStringItem		*lv_item;
	const int		ncols = 8;
	char			atext[64];
	const rgb_color	sfillcolor = { 160, 200, 240, 0 };

	fontboy = dynamic_cast<Fontboy*>(be_app);
	prefs = fontboy->prefs;

	ResizeTo(400.0, 300.0);

	aRect = Bounds();
	outbox = new BBox(aRect, NULL, 0, B_WILL_DRAW, B_PLAIN_BORDER);
	AddChild(outbox);

	aRect.InsetBy(10, 10);
	aRect.bottom -= 50;

	BRect lRect = aRect;
	lRect.right = lRect.left + 80;
	lView = new BOutlineListView(lRect, "ListView");

	lView->AddItem(new BStringItem("General"));
	lView->AddItem(lv_item = new BStringItem("List"));
	lView->AddUnder(new BStringItem("Colors"), lv_item);
	lView->AddUnder(new BStringItem("Display"), lv_item);
	lView->AddItem(lv_item = new BStringItem("Details"));
	lView->AddUnder(new BStringItem("Colors"), lv_item);
	lView->SetSelectionMessage(new BMessage(M_PREFS_LV_SELECT));
	outbox->AddChild(new BScrollView("scroll_cities", lView, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, false));
	lView->Select(0);
	lView->MakeFocus(true);

//	olView->SetViewColor(def_viewcolor); 
//	outbox->AddChild(lView);

	aRect.left += lView->Frame().Width() + 10;
	BRect boxRect = aRect;

	float rightb = aRect.right - 10;
	float alignl = aRect.left + 10;
	float alignr = (aRect.right - aRect.left) / 2 + 10;

	/* View for general preferences
	 *******************************/
	gbox = new BBox(boxRect, NULL, 0, B_WILL_DRAW, B_FANCY_BORDER);
	gbox->SetLabel("General");
	outbox->AddChild(gbox);
	cprefsview = gbox;

	aRect = gbox->Bounds();
	aRect.InsetBy(10, 15);

	aRect.bottom = aRect.top + 20;
	aRect.right = aRect.left + font.StringWidth("Show splashscreen") + 50;
	splashscreen = new BCheckBox(aRect, "", "Show splashscreen",  new BMessage(M_SPLASHSCREEN));
	gbox->AddChild(splashscreen);

	aRect.top = aRect.bottom + 8;
	aRect.bottom = aRect.top + 20;
	aRect.right = aRect.left + font.StringWidth("Show Tool Tips") + 50;
	tooltips = new BCheckBox(aRect, "", "Show Tool Tips",  new BMessage(M_TOOLTIPS));
	gbox->AddChild(tooltips);


	/* View for color preferences in fontlist
	 ****************************************/
	c1box = new BBox(boxRect, NULL, 0, B_WILL_DRAW, B_FANCY_BORDER);
	c1box->SetLabel("Colors");
	outbox->AddChild(c1box);
	c1box->Hide();

	// Mainwindow
	aRect = c1box->Bounds();
	aRect.InsetBy(25, 30);

//	aRect.bottom = aRect.top + 3 * (CB_HEIGHT + CB_VOFFSET) + 15;
//	mbox = new BBox(aRect, "List", 0, B_WILL_DRAW, B_FANCY_BORDER);
//	mbox->SetLabel("List");
//	c1box->AddChild(mbox);

//	aRect = mbox->Bounds();
//	aRect.InsetBy(15, 15);
	aRect.bottom = aRect.top + CB_HEIGHT;

	float left_rbound = (aRect.right - aRect.left - 14) / 2 + aRect.left;
	float right_lbound = (aRect.right - aRect.left - 14) / 2 + 14 +aRect.left;
	rightb = aRect.right;
	alignl = aRect.left;
	alignr = (aRect.right - aRect.left) / 2 + aRect.left;

	BRect r = aRect;
	r.right = left_rbound;
	cmbg = new ColorButton(r, B_EMPTY_STRING, "Background", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cmbg->SetDivider(90);
	c1box->AddChild(cmbg);

	r = aRect;
	r.left = right_lbound;
	cmdisplay = new ColorButton(r, B_EMPTY_STRING, "Display Text", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cmdisplay->SetDivider(90);
	c1box->AddChild(cmdisplay);

	aRect.OffsetBy(0, CB_HEIGHT + CB_VOFFSET);

	r = aRect;
	r.right = left_rbound;
	cmselect = new ColorButton(r, B_EMPTY_STRING, "Selection", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cmselect->SetDivider(90);
	c1box->AddChild(cmselect);

	r = aRect;
	r.left = right_lbound;
	cmstroke = new ColorButton(r, B_EMPTY_STRING, "Borders", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cmstroke->SetDivider(90);
	c1box->AddChild(cmstroke);

	aRect.OffsetBy(0, CB_HEIGHT + CB_VOFFSET);

	r = aRect;
	r.right = left_rbound;
	cmheights = new ColorButton(r, B_EMPTY_STRING, "Height Lines", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cmheights->SetDivider(90);
	c1box->AddChild(cmheights);

	r = aRect;
	r.left = right_lbound;
	cminfo = new ColorButton(r, B_EMPTY_STRING, "Info Text", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_LIST);
	cminfo->SetDivider(90);
	c1box->AddChild(cminfo);

	aRect = c1box->Bounds();
	aRect.top = aRect.bottom - 43;
	aRect.bottom = aRect.top + 20;
	aRect.right -= 20;
	aRect.left = aRect.right - font.StringWidth("Default Colors") - 30;
	cdefault = new BButton(aRect, "", "Default Colors", new BMessage(M_C1DEFAULT));
	cdefault->SetEnabled(true);
	c1box->AddChild(cdefault);

	/* View for color preferences in details
	 ****************************************/
	c2box = new BBox(boxRect, NULL, 0, B_WILL_DRAW, B_FANCY_BORDER);
	c2box->SetLabel("Colors");
	outbox->AddChild(c2box);
	c2box->Hide();

	// Propertieswindow
	aRect = c2box->Bounds();
	aRect.InsetBy(25, 30);
//	aRect.bottom = aRect.top + 2 * (CB_HEIGHT + CB_VOFFSET) + 15;
//	pbox = new BBox(aRect, "Details", 0, B_WILL_DRAW, B_FANCY_BORDER);
//	pbox->SetLabel("Details");
//	c2box->AddChild(pbox);

//	aRect = pbox->Bounds();
//	aRect.InsetBy(15, 15);
	aRect.bottom = aRect.top + CB_HEIGHT;

	rightb = aRect.right;
	alignl = aRect.left;
	alignr = (aRect.right - aRect.left) / 2 + aRect.left;

	r = aRect;
	r.right = left_rbound;
	cpbg = new ColorButton(r, B_EMPTY_STRING, "Background", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_DETAIL);
	cpbg->SetDivider(90);
	c2box->AddChild(cpbg);

	r = aRect;
	r.left = right_lbound;
	cpdisplay = new ColorButton(r, B_EMPTY_STRING, "Display Text", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_DETAIL);
	cpdisplay->SetDivider(90);
	c2box->AddChild(cpdisplay);

	aRect.OffsetBy(0, CB_HEIGHT + CB_VOFFSET);

	r = aRect;
	r.right = left_rbound;
	cpselect = new ColorButton(r, B_EMPTY_STRING, "Selection", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_DETAIL);
	cpselect->SetDivider(90);
	c2box->AddChild(cpselect);

	r = aRect;
	r.left = right_lbound;
	cpstroke = new ColorButton(r, B_EMPTY_STRING, "Borders", B_FOLLOW_NONE, B_WILL_DRAW, NULL, M_COLCHANGE_DETAIL);
	cpstroke->SetDivider(90);
	c2box->AddChild(cpstroke);

	aRect = c2box->Bounds();
	aRect.top = aRect.bottom - 43;
	aRect.bottom = aRect.top + 20;
	aRect.right -= 20;
	aRect.left = aRect.right - font.StringWidth("Default Colors") - 30;
	cdefault = new BButton(aRect, "", "Default Colors", new BMessage(M_C2DEFAULT));
	cdefault->SetEnabled(true);
	c2box->AddChild(cdefault);

	/* View for fontlist settings
	 ********************************/
	dbox = new BBox(boxRect, NULL, 0, B_WILL_DRAW, B_FANCY_BORDER);
	dbox->SetLabel("Display Parameters");
	outbox->AddChild(dbox);
	dbox->Hide();
	
	aRect = dbox->Bounds();
	aRect.InsetBy(10, 15);

	left_rbound = (aRect.right - aRect.left - 14) / 2 + aRect.left;
	right_lbound = (aRect.right - aRect.left - 14) / 2 + 14 +aRect.left;
	rightb = aRect.right;
	alignl = aRect.left;
	alignr = (aRect.right - aRect.left) / 2 + aRect.left;

	aRect.bottom = aRect.top + 50;
	slider = new StatusSlider(aRect, "FontSize", "Font Size:", new BMessage(M_FONTSIZE), 6, 1000, B_TRIANGLE_THUMB);
	slider->UseFillColor(true, &sfillcolor);
	slider->SetUpdateText("%upt");
	slider->SetHashMarks(B_HASH_MARKS_BOTH);
	slider->SetValue(int32(prefs->GetFontSize()));
	slider->SetHashMarkCount(13);
	slider->SetKeyIncrementValue(1);
	slider->SetLimitLabels("6pt", "1000pt");
	slider->SetModificationMessage(new BMessage(M_FONTSIZE));
	dbox->AddChild(slider);

	aRect.left = alignl;
	aRect.right = aRect.left + font.StringWidth("Number of Columns:") + 100;
	aRect.top = aRect.bottom + 16;
	aRect.bottom = aRect.top + 20;
	numcols = new BPopUpMenu("Columns");
	for (int i = 0; i <= ncols; i++) {

		if (i == 0)
			sprintf(atext, "Auto-adjust");
		else
			sprintf(atext, "%d", i);

		numcols->AddItem(item = new BMenuItem(atext, new BMessage(M_NUMCOLS)));
	}
	aafield = new BMenuField(aRect, "", "Columns:", numcols);
	aafield->SetFont(&font);
	aafield->SetDivider(font.StringWidth("Example Text:") + 12.0);
	dbox->AddChild(aafield);

	aRect.left = alignl;
	aRect.right = rightb;
	aRect.top = aRect.bottom + 12;
	aRect.bottom = aRect.top + 25;

	aRect.left = alignl;
	aRect.right = rightb - 8;
	text = new BTextControl(aRect, "", NULL, "blubber", new BMessage(M_VIEWTEXTINV));
	text->SetModificationMessage(new BMessage(M_VIEWTEXTMOD));
	text->SetDivider(font.StringWidth("Example Text:") + 12.0);
	text->SetLabel("Example Text:");
	text->SetViewColor(def_viewcolor);
	dbox->AddChild(text);

	aRect.left = alignl;
	aRect.right = aRect.left + font.StringWidth("Draw Border") + 50;
	aRect.top = aRect.bottom + 20;
	aRect.bottom = aRect.top + 20;
	drawborder = new BCheckBox(aRect, "", "Draw Border",  new BMessage(M_DRAWBORDER));
	dbox->AddChild(drawborder);

	aRect.left = alignl;
	aRect.right = aRect.left + font.StringWidth("Draw Heights") + 50;
	aRect.top = aRect.bottom + 8;
	aRect.bottom = aRect.top + 20;
	drawheights = new BCheckBox(aRect, "", "Draw Heights",  new BMessage(M_DRAWHEIGHTS));
	dbox->AddChild(drawheights);

	aRect = outbox->Bounds();
	aRect.InsetBy(10, 20);
	aRect.top = aRect.bottom - 20;
	aRect.right = aRect.left + font.StringWidth("Live update!") + 50;
	liveupdate = new BCheckBox(aRect, "", "Live update!",  new BMessage(M_LIVEUPDATE));
	outbox->AddChild(liveupdate);

	aRect = outbox->Bounds();
	aRect.InsetBy(10, 20);
	aRect.top = aRect.bottom - 20;
	aRect.left = aRect.right - font.StringWidth("Revert") - 30;
	revert = new BButton(aRect, "", "Revert", new BMessage(M_REVERT));
	revert->SetEnabled(false);
	outbox->AddChild(revert);

	aRect.right = aRect.left - 20;
	aRect.left = aRect.right - font.StringWidth("Revert") - 30;
	apply = new BButton(aRect, "", "Apply", new BMessage(M_APPLY));
	apply->MakeDefault(true);
	apply->SetEnabled(false);
	outbox->AddChild(apply);

	AddToolTips();
	GetPreferences();
}

PrefsWindow::~PrefsWindow(void)
{
	RemoveToolTips();
}

void PrefsWindow::AddToolTips()
{
	if ((bhelper = fontboy->bhelper) != NULL) {
		bhelper->SetHelp(splashscreen, "Show splashscreen when starting Fontboy");
		bhelper->SetHelp(cmbg, "Select color for background");
		bhelper->SetHelp(cmdisplay, "Select color for fontinformation");
		bhelper->SetHelp(cminfo, "Select color for infotext");
		bhelper->SetHelp(cmstroke, "Select color for borders");
		bhelper->SetHelp(cmheights, "Select color for height lines");
		bhelper->SetHelp(cmselect, "Select color for selection");
		bhelper->SetHelp(cpbg, "Select color for background");
		bhelper->SetHelp(cpdisplay, "Select color for font");
		bhelper->SetHelp(cpselect, "Select color for selection");
		bhelper->SetHelp(cpstroke, "Select color for borders");
		bhelper->SetHelp(cdefault, "Set default colors");
		bhelper->SetHelp(slider, "Change size of fonts");
		bhelper->SetHelp(aafield, "Change number of columns");
		bhelper->SetHelp(text, "Change example text");
		bhelper->SetHelp(drawborder, "Draw borders for each font");
		bhelper->SetHelp(drawheights, "Show lines for fontheights");
		bhelper->SetHelp(liveupdate, "Settings apply live to the changes");
		bhelper->SetHelp(revert, "Revert to last saved settings");
		bhelper->SetHelp(apply, "Apply settings");
		bhelper->SetHelp(tooltips, "Show tool tips");
	}
}

void PrefsWindow::RemoveToolTips()
{
	if ((bhelper = fontboy->bhelper) != NULL) {
		bhelper = fontboy->bhelper;
		bhelper->SetHelp(splashscreen, NULL);
		bhelper->SetHelp(cmbg, NULL);
		bhelper->SetHelp(cmdisplay, NULL);
		bhelper->SetHelp(cminfo, NULL);
		bhelper->SetHelp(cmstroke, NULL);
		bhelper->SetHelp(cmheights, NULL);
		bhelper->SetHelp(cmselect, NULL);
		bhelper->SetHelp(cpbg, NULL);
		bhelper->SetHelp(cpdisplay, NULL);
		bhelper->SetHelp(cpselect, NULL);
		bhelper->SetHelp(cpstroke, NULL);
		bhelper->SetHelp(cdefault, NULL);
		bhelper->SetHelp(slider, NULL);
		bhelper->SetHelp(aafield, NULL);
		bhelper->SetHelp(drawborder, NULL);
		bhelper->SetHelp(drawheights, NULL);
		bhelper->SetHelp(liveupdate, NULL);
		bhelper->SetHelp(text, NULL);
		bhelper->SetHelp(revert, NULL);
		bhelper->SetHelp(apply, NULL);
		bhelper->SetHelp(tooltips, NULL);
	}
}

void PrefsWindow::GetPreferences()
{
	char	atext[20];

	slider->SetValue(int32(prefs->GetFontSize()));
	snumcols = int32(prefs->GetNumCols());
	sprintf(atext, "%d", int(snumcols));
	if ((item = numcols->FindItem(atext)) != NULL) {
		item->SetMarked(true);
	}
	else {
		if (snumcols == 0) {
			item = numcols->FindItem("Auto-adjust");
			item->SetMarked(true);
		}
	}
	drawheights->SetValue(prefs->GetDrawHeights());
	drawborder->SetValue(prefs->GetDrawBorder());
	splashscreen->SetValue(prefs->GetSplashScreen());
	tooltips->SetValue(prefs->GetToolTips());
	liveupdate->SetValue(prefs->GetLiveupdate());
	text->SetText(prefs->GetDisplayText()->String());
	cmbg->SetValue(prefs->GetMBgColor());
	cmdisplay->SetValue(prefs->GetMDisplayColor());
	cminfo->SetValue(prefs->GetMInfoColor());
	cmstroke->SetValue(prefs->GetMStrokeColor());
	cmheights->SetValue(prefs->GetMHeightsColor());
	cmselect->SetValue(prefs->GetMSelectColor());
	cpbg->SetValue(prefs->GetPBgColor());
	cpdisplay->SetValue(prefs->GetPDisplayColor());
	cpselect->SetValue(prefs->GetPSelectColor());
	cpstroke->SetValue(prefs->GetPStrokeColor());
}

void PrefsWindow::SetPreferences()
{
	if (snumcols == 0)
		prefs->SetAutoAdjust(1);
	else
		prefs->SetAutoAdjust(0);

	prefs->SetFontSize(float(slider->Value()));
	prefs->SetNumCols(float(snumcols));
	prefs->SetDrawHeights(drawheights->Value());
	prefs->SetDrawBorder(drawborder->Value());
	prefs->SetSplashScreen(splashscreen->Value());
	prefs->SetToolTips(tooltips->Value());
	prefs->SetLiveupdate(liveupdate->Value());
	prefs->SetDisplayText(text->Text());

	rgb_color col;
	col = cmbg->ValueAsColor();
	prefs->SetMBgColor(&col);
	col = cmdisplay->ValueAsColor();
	prefs->SetMDisplayColor(&col);
	col = cminfo->ValueAsColor();
	prefs->SetMInfoColor(&col);
	col = cmstroke->ValueAsColor();
	prefs->SetMStrokeColor(&col);
	col = cmheights->ValueAsColor();
	prefs->SetMHeightsColor(&col);
	col = cmselect->ValueAsColor();
	prefs->SetMSelectColor(&col);
	col = cpbg->ValueAsColor();
	prefs->SetPBgColor(&col);
	col = cpdisplay->ValueAsColor();
	prefs->SetPDisplayColor(&col);
	col = cpselect->ValueAsColor();
	prefs->SetPSelectColor(&col);
	col = cpstroke->ValueAsColor();
	prefs->SetPStrokeColor(&col);
}

void PrefsWindow::UpdatePrefs(bool force = false)
{
	if ((liveupdate->Value() == B_CONTROL_ON) ||
		(force == true)) {
		SetPreferences();
		be_app->PostMessage(M_APPLYSETTINGS);
		apply->SetEnabled(false);
	}
	else {
		apply->SetEnabled(true);
	}
	revert->SetEnabled(true);
}

void PrefsWindow::MessageReceived(BMessage* msg)
{
	int32	val, item_int;
	BStringItem	*stritem, *superitem;

	switch(msg->what) {
		case M_LIVEUPDATE:
			revert->SetEnabled(false);
			UpdatePrefs();
			break;

		// General
		case M_SPLASHSCREEN:
		case M_TOOLTIPS:
			UpdatePrefs();
			break;

		// ListView
		case M_NUMCOLS:
			val = msg->FindInt32("index");
			if (val != B_NAME_NOT_FOUND || B_BAD_TYPE) {
				snumcols = val;
			}

		case M_VIEWTEXTINV:
		case M_VIEWTEXTMOD:
		case M_FONTSIZE:
		case M_DRAWHEIGHTS:
		case M_DRAWBORDER:
		case M_COLCHANGE_LIST:

			prefs->SetListWindowForUpdate();
			UpdatePrefs();
			break;

		case M_C1DEFAULT:
			ccwindow = GetAppWindow("Fontboy color");
			if (ccwindow != NULL) {
				ccwindow->PostMessage(B_QUIT_REQUESTED);
				ccwindow = NULL;
			}

			cmbg->SetValue(prefs->GetMBgDefault());
			cmdisplay->SetValue(prefs->GetMDisplayDefault());
			cminfo->SetValue(prefs->GetMInfoDefault());
			cmstroke->SetValue(prefs->GetMStrokeDefault());
			cmheights->SetValue(prefs->GetMHeightsDefault());
			cmselect->SetValue(prefs->GetMSelectDefault());
			prefs->SetListWindowForUpdate();
			UpdatePrefs();
			break;
		
		// Details
		case M_C2DEFAULT:
			ccwindow = GetAppWindow("Fontboy color");
			if (ccwindow != NULL) {
				ccwindow->PostMessage(B_QUIT_REQUESTED);
				ccwindow = NULL;
			}

			cpbg->SetValue(prefs->GetPBgDefault());
			cpdisplay->SetValue(prefs->GetPDisplayDefault());
			cpselect->SetValue(prefs->GetPSelectDefault());
			cpstroke->SetValue(prefs->GetPStrokeDefault());

		case M_COLCHANGE_DETAIL:
			prefs->SetDetailWindowForUpdate();
			UpdatePrefs();
			break;
		
		case M_APPLY:
			UpdatePrefs(true);
			break;
		
		case M_REVERT:
			prefs->LoadPrefs();
			GetPreferences();
			be_app->PostMessage(M_APPLYSETTINGS);
			apply->SetEnabled(false);
			revert->SetEnabled(false);
			break;
		
		case M_PREFS_LV_SELECT:
			stritem = dynamic_cast<BStringItem*>(lView->ItemAt(lView->CurrentSelection()));

			if (stritem != NULL) {
				// General
				if (!strcmp(stritem->Text(), "General")) {
					if (cprefsview != gbox) {
						cprefsview->Hide();
						cprefsview = gbox;
						cprefsview->Show();
					}
				}
	
				// List - Display
				if (!strcmp(stritem->Text(), "Display")) {
					if (cprefsview != dbox) {
						cprefsview->Hide();
						cprefsview = dbox;
						cprefsview->Show();
					}
				}
	
				// Details - Colors
				if (!strcmp(stritem->Text(), "Colors")) {
					superitem = dynamic_cast<BStringItem*>(lView->Superitem(stritem));
	
					if (superitem != NULL) {
						if (!strcmp(superitem->Text(), "List")) {
							cprefsview->Hide();
							cprefsview = c1box;
							cprefsview->Show();
						}
						if (!strcmp(superitem->Text(), "Details")) {
							cprefsview->Hide();
							cprefsview = c2box;
							cprefsview->Show();
						}
					}
				}

				// Details & List
				if ((!strcmp(stritem->Text(), "List"))
				 ||	(!strcmp(stritem->Text(), "Details"))) {
					item_int = lView->IndexOf(stritem);
	
					if (lView->IsExpanded(item_int))
						lView->Select(item_int + 1);
				}
			}
			break;
		
		default:
			BWindow::MessageReceived(msg);
	}
}

bool PrefsWindow::QuitRequested(void)
{

	ccwindow = GetAppWindow("Fontboy color");
	if (ccwindow != NULL) {
		ccwindow->PostMessage(B_QUIT_REQUESTED);
		ccwindow = NULL;
	}

	BMessage msg(M_SETTINGSQUIT);
	msg.AddRect("rect", Frame());
	be_app->PostMessage(&msg);

	return true;
}

/* loop through the window list of the application, looking for
   a window with a specified name. */
BWindow	*PrefsWindow::GetAppWindow(char *name)
{

	int32		index;
	BWindow		*window;
	
	for (index = 0;; index++) {
		window = be_app->WindowAt(index);
		if (window == NULL)
			break;
		if (window->LockWithTimeout(200000) == B_OK) {
			if (strcmp(window->Name(), name) == 0) {
				window->Unlock();
				break;
			}
			window->Unlock();
		}
	}
	return window; 
}

