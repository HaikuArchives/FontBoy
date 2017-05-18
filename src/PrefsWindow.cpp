/* Preference Window */

#include "Fontboy.h"
#include "Prefs.h"
#include "PrefsWindow.h"
#include "MsgVals.h"
#include <Debug.h>

#include <Button.h>
#include <Catalog.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <Locale.h>
#include <Messenger.h>
#include <SpaceLayoutItem.h>
#include <TabView.h>

PrefsWindow::PrefsWindow(BRect rect)
:BWindow(rect, "Settings", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS | B_ASYNCHRONOUS_CONTROLS)
{
	BFont			font(be_plain_font);
	const int		ncols = 8;
	char			atext[64];
	const rgb_color	sfillcolor = { 160, 200, 240, 0 };

	fontboy = dynamic_cast<Fontboy*>(be_app);
	prefs = fontboy->prefs;

	/* View for color preferences in fontlist
	 ****************************************/

	// Set up list of color attributes
	fAttrList = new BListView("AttributeList", B_SINGLE_SELECTION_LIST);

	BScrollView* fScrollView = new BScrollView("ScrollView", fAttrList, 0, false, true);
	fScrollView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	rgb_color col;
	cmbg = new ColorItem("List background", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmbg);
	cmdisplay = new ColorItem("List text", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmdisplay);
	cminfo = new ColorItem("List info text", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cminfo);
	cmheights = new ColorItem("List height lines", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmheights);
	cmstroke = new ColorItem("List borders", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmstroke);
	cmselectedbg = new ColorItem("Selected list background", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmselectedbg);
	cmselecteddisplay = new ColorItem("Selected list text", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmselecteddisplay);
	cmselectedinfo = new ColorItem("Selected list info text", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmselectedinfo);	
	cmselectedheights = new ColorItem("Selected list height lines", LIST_WINDOW_COLOR, col);
	fAttrList->AddItem(cmselectedheights);

	
	cpbg = new ColorItem("Details background", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpbg);
	cpdisplay = new ColorItem("Details text", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpdisplay);
	cpstroke = new ColorItem("Details borders", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpstroke);
	cpselectedbg = new ColorItem("Selected details background", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpselectedbg);
	cpselecteddisplay = new ColorItem("Selected details text", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpselecteddisplay);
	cpinactiveselectedbg = new ColorItem("Inactive selected details background", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpinactiveselectedbg);
	cpinactiveselecteddisplay = new ColorItem("Inactive Selected details text", DETAILS_WINDOW_COLOR, col);
	fAttrList->AddItem(cpinactiveselecteddisplay);
	
	BRect wellrect(0, 0, 50, 50);
	fColorPreview = new ColorPreview(wellrect, new BMessage(M_COLOR_DROPPED), 0);

	fColorPreview->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, 
			B_ALIGN_BOTTOM));

	fPicker = new BColorControl(B_ORIGIN, B_CELLS_32x8, 8.0,
		"picker", new BMessage(M_UPDATE_COLOR));

	BView* colorsView = new BView("Colors", 0);

	colorsView->SetLayout(new BGroupLayout(B_VERTICAL));

	colorsView->AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(fScrollView)
		.Add(BSpaceLayoutItem::CreateVerticalStrut(5))
		.Add(BGroupLayoutBuilder(B_HORIZONTAL)
			.Add(fColorPreview)
			.Add(BSpaceLayoutItem::CreateHorizontalStrut(5))
			.Add(fPicker)
		)
		.SetInsets(10, 10, 10, 10)
	);
	
	fColorPreview->Parent()->SetExplicitMaxSize(
		BSize(B_SIZE_UNSET, fPicker->Bounds().Height()));
	fAttrList->SetSelectionMessage(new BMessage(M_ATTRIBUTE_CHOSEN));

	/* View for fontlist settings
	 ********************************/
	 
	BView* displayView = new BView("Display", 0);
	BRect zeroRect(0,0,0,0);
	
	slider = new StatusSlider(zeroRect, "FontSize", "Font size:", new BMessage(M_FONTSIZE), 6, 360, B_TRIANGLE_THUMB);
	slider->UseFillColor(true, &sfillcolor);
	slider->SetUpdateText("%upt");
	slider->SetHashMarks(B_HASH_MARKS_BOTH);
	slider->SetValue(int32(prefs->GetFontSize()));
	slider->SetHashMarkCount(13);
	slider->SetKeyIncrementValue(1);
	slider->SetLimitLabels("6pt", "360pt");
	slider->SetModificationMessage(new BMessage(M_FONTSIZE));
	slider->SetToolTip("Change size of fonts");
	
	
	numcols = new BPopUpMenu("Columns");
	for (int i = 0; i <= ncols; i++) {

		if (i == 0)
			sprintf(atext, "Auto-adjust");
		else
			sprintf(atext, "%d", i);

		numcols->AddItem(item = new BMenuItem(atext, new BMessage(M_NUMCOLS)));
	}
	aafield = new BMenuField(zeroRect, "", "Columns:", numcols);
	aafield->SetFont(&font);
	aafield->SetDivider(font.StringWidth("Example text:") + 12.0);
	aafield->SetToolTip("Change number of columns");
	
	text = new BTextControl(zeroRect, "", NULL, "Example text", new BMessage(M_VIEWTEXTINV));
	text->SetModificationMessage(new BMessage(M_VIEWTEXTMOD));
	text->SetDivider(font.StringWidth("Example text:") + 12.0);
	text->SetLabel("Example text:");
	text->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	text->SetToolTip("Change example text");
	// ListView uses 128 bytes array. Avoid overflow...
	text->TextView()->SetMaxBytes(128);
	
	
	drawborder = new BCheckBox(zeroRect, "", "Draw borders",  new BMessage(M_DRAWBORDER));
	drawborder->SetToolTip("Draw borders for each font");
	
	drawheights = new BCheckBox(zeroRect, "", "Draw heights",  new BMessage(M_DRAWHEIGHTS));
	drawheights->SetToolTip("Show lines for font heights");
	
	displayView->SetLayout(new BGroupLayout(B_VERTICAL));
	
	displayView->AddChild(BLayoutBuilder::Group<>(B_VERTICAL)
		.Add(slider)
		.Add(aafield)
		.Add(text)
		.Add(drawborder)
		.Add(drawheights)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING,
			B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	);
	
	fTabView = new BTabView("tabview", B_WIDTH_FROM_LABEL);
	fTabView->AddTab(displayView);
	fTabView->AddTab(colorsView);

	GetPreferences();
	
	fRevertButton = new BButton(zeroRect, "", "Revert", new BMessage(M_REVERT));
	fRevertButton->SetToolTip("Revert to last saved settings");
	fRevertButton->SetEnabled(false);

	fDefaultsButton = new BButton(zeroRect, "", "Defaults", new BMessage(M_DEFAULT));
	fDefaultsButton->SetToolTip("Restore defaults settings");
	fDefaultsButton->SetEnabled(!prefs->IsDefault());

	
	SetLayout(new BGroupLayout(B_VERTICAL));

	AddChild(BLayoutBuilder::Group<>(B_VERTICAL)
		.Add(fTabView)
		.AddGroup(B_HORIZONTAL)
			.Add(fDefaultsButton)
			.Add(fRevertButton)
			.AddGlue()
		.End()
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING,
			B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	);
	
	fAttrList->Select(0);
}

PrefsWindow::~PrefsWindow(void)
{
	prefs->SavePrefs();
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
	text->SetModificationMessage(NULL);
	text->SetText(prefs->GetDisplayText().String());
	text->SetModificationMessage(new BMessage(M_VIEWTEXTMOD));
	cmbg->SetColor(prefs->GetMBgColor());
	cmdisplay->SetColor(prefs->GetMDisplayColor());
	cminfo->SetColor(prefs->GetMInfoColor());
	cmstroke->SetColor(prefs->GetMStrokeColor());
	cmheights->SetColor(prefs->GetMHeightsColor());
	cmselectedbg->SetColor(prefs->GetMBgColor(true));
	cmselecteddisplay->SetColor(prefs->GetMDisplayColor(true));
	cmselectedinfo->SetColor(prefs->GetMInfoColor(true));
	cmselectedheights->SetColor(prefs->GetMHeightsColor(true));
	cpbg->SetColor(prefs->GetPBgColor());
	cpdisplay->SetColor(prefs->GetPDisplayColor());	
	cpstroke->SetColor(prefs->GetPStrokeColor());
	cpselectedbg->SetColor(prefs->GetPBgColor(true));
	cpselecteddisplay->SetColor(prefs->GetPDisplayColor(true));
	cpinactiveselectedbg->SetColor(prefs->GetPInactiveSelectedBgColor());
	cpinactiveselecteddisplay->SetColor(prefs->GetPInactiveSelectedDisplayColor());

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
	prefs->SetDisplayText(text->Text());

	rgb_color col;
	col = cmbg->Color();
	prefs->SetMBgColor(&col);
	col = cmdisplay->Color();
	prefs->SetMDisplayColor(&col);
	col = cminfo->Color();
	prefs->SetMInfoColor(&col);
	col = cmstroke->Color();
	prefs->SetMStrokeColor(&col);
	col = cmheights->Color();
	prefs->SetMHeightsColor(&col);
	col = cmselectedbg->Color();
	prefs->SetMSelectedBgColor(&col);
	col = cmselecteddisplay->Color();
	prefs->SetMSelectedDisplayColor(&col);
	col = cmselectedinfo->Color();
	prefs->SetMSelectedInfoColor(&col);
	col = cmselectedheights->Color();
	prefs->SetMSelectedHeightsColor(&col);
	col = cpbg->Color();
	prefs->SetPBgColor(&col);
	col = cpdisplay->Color();
	prefs->SetPDisplayColor(&col);
	col = cpstroke->Color();
	prefs->SetPStrokeColor(&col);
	col = cpselectedbg->Color();
	prefs->SetPSelectedBgColor(&col);
	col = cpselecteddisplay->Color();
	prefs->SetPSelectedDisplayColor(&col);
	col = cpinactiveselectedbg->Color();
	prefs->SetPInactiveSelectedBgColor(&col);
	col = cpinactiveselecteddisplay->Color();
	prefs->SetPInactiveSelectedDisplayColor(&col);
}

void PrefsWindow::UpdatePrefs()
{
	SetPreferences();
	be_app->PostMessage(M_APPLYSETTINGS);

	fRevertButton->SetEnabled(prefs->CanRevert());
	fDefaultsButton->SetEnabled(!prefs->IsDefault());
}

void PrefsWindow::MessageReceived(BMessage* msg)
{
	int32	val, item_int;
	BStringItem	*stritem, *superitem;
	
	if (msg->WasDropped() && fTabView->Selection() == 1) {
		rgb_color* color = NULL;
		ssize_t size = 0;

		if (msg->FindData("RGBColor", (type_code)'RGBC', (const void**)&color,
				&size) == B_OK) {
			_SetCurrentColor(*color);
			
			int32 currentIndex = fAttrList->CurrentSelection();
			ColorItem* item = (ColorItem*)fAttrList->ItemAt(currentIndex);
			prefs->SetListWindowForUpdate();
			prefs->SetDetailWindowForUpdate();
			UpdatePrefs();
		}
	}
	
	
	switch(msg->what) {

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

		case M_COLCHANGE_DETAIL:
			prefs->SetDetailWindowForUpdate();
			UpdatePrefs();
			break;
			
		case M_DEFAULT: 
			prefs->Default();
			GetPreferences();
			prefs->SetListWindowForUpdate();
			prefs->SetDetailWindowForUpdate();
			UpdatePrefs();
			_UpdateColors();
			break;
		
		case M_REVERT:
			prefs->Revert();
			GetPreferences();
			prefs->SetListWindowForUpdate();
			prefs->SetDetailWindowForUpdate();
			UpdatePrefs();
			_UpdateColors();
			break;
		
			
		case M_UPDATE_COLOR:
		{
			// Received from the color fPicker when its color changes
			rgb_color color = fPicker->ValueAsColor();
			_SetCurrentColor(color);
			
			int32 currentIndex = fAttrList->CurrentSelection();
			ColorItem* item = (ColorItem*)fAttrList->ItemAt(currentIndex);
			if (item != NULL) {
				if (item->ColorType() == LIST_WINDOW_COLOR)
					prefs->SetListWindowForUpdate();
				else
					prefs->SetDetailWindowForUpdate();
			}
			
			UpdatePrefs();

			break;
		}

		case M_ATTRIBUTE_CHOSEN:
		{
			// Received when the user chooses a GUI fAttribute from the list

			ColorItem* item = (ColorItem*)
				fAttrList->ItemAt(fAttrList->CurrentSelection());
			if (item == NULL)
				break;

			rgb_color color = item->Color();
			_SetCurrentColor(color);
			break;
		}
		
		default:
			BWindow::MessageReceived(msg);
	}
}

void
PrefsWindow::_SetCurrentColor(rgb_color color)
{
	int32 currentIndex = fAttrList->CurrentSelection();
	ColorItem* item = (ColorItem*)fAttrList->ItemAt(currentIndex);
	if (item != NULL) {	
		item->SetColor(color);
		fAttrList->InvalidateItem(currentIndex);
	}

	fPicker->SetValue(color);
	fColorPreview->SetColor(color);
	fColorPreview->Invalidate();
}


void
PrefsWindow::_UpdateColors()
{
	fAttrList->Invalidate();
	int32 currentIndex = fAttrList->CurrentSelection();
	ColorItem* item = (ColorItem*)fAttrList->ItemAt(currentIndex);
	if (item != NULL) 
		_SetCurrentColor(item->Color());
}


bool PrefsWindow::QuitRequested(void)
{

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

