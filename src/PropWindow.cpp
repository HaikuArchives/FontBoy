#include "Fontboy.h"
#include "Prefs.h"
#include "PropWindow.h"
#include "MsgVals.h"
#include "Debug.h"
#include "FFont.h"

/* Property Window
-------------------------------------------------------------------|
|  Top View                                                        |
|  includes title of font, displayrange and navigationbuttons      |
|------------------------------------------------------------------|
| SplitpaneView		   |										   |
|					   |										   |
| |------------------| | |---------------------------------------| |
| |  CharView        | | |  CharSetView                          | |
| |                  | | |                                       | |
| |                  | | |                                       | |
| |                  | | |                                       | |
| |                  | | |                                       | |
| |                  | | |                                       | |
| |------------------| | |                                       | |
|                      | |                                       | |
|  Left View           | |                                       | |
|  includes Charview,  | |                                       | |
|  Fontname, style, ...| |                                       | |
|                      | |                                       | |
|                      | |---------------------------------------| |
|----------------------|-------------------------------------------|
*/

PropWindow::PropWindow(BRect frame, font_family family, font_style style)
		   :BWindow(frame, "Details", B_TITLED_WINDOW, 0),
	pview(NULL)
{

	SetFamilyAndStyle(family, style);
	SetTitle();

	pview = new PropView(Bounds());
	AddChild(pview);
}

PropWindow::~PropWindow(void)
{
}

void PropWindow::MessageReceived(BMessage *msg)
{
	FFont font;

	switch(msg->what) {
		case M_FFONT:
			FindMessageFont(msg, "font", 0, &font);
			font.GetFamilyAndStyle(&family, &style);
			pview->Update();
			SetTitle();
			Activate();
			break;

		case M_PROPWIN_UPDATE:
			pview->Update();
			break;

		case B_SIMPLE_DATA:
			break;

		default:
			BWindow::MessageReceived(msg);
	}
}

bool PropWindow::QuitRequested(void)
{
	// Save settings
	BMessage msg(M_DETAILSQUIT);
	msg.AddRect("rect", Frame());
	msg.AddPointer("winptr", (void *)this);
	msg.AddInt16("sppos", pview->GetSliderPos());
	be_app->PostMessage(&msg);

	return true;
}

void PropWindow::Zoom(BPoint origin, float width, float heigh)
{
	pview->Zoom(width, heigh);
	BWindow::Zoom(origin, width, heigh);
}

void PropWindow::SetTitle()
{
	// Set title
	BString name = family;
	name += " ";
	name += style;
	BWindow::SetTitle(name.String());
}

void PropWindow::Update()
{
	if (pview != NULL)
		pview->Update();
}

void PropWindow::SelectChar(uint16 unicode)
{
	if (pview != NULL)
		pview->SetCharPos(unicode);
}

void PropWindow::LockSplitpane(bool val)
{
	pview->LockSliderPos(val);
}

bool PropWindow::IsSplitpaneLocked()
{
	return pview->IsSliderPosLocked();
}

void PropWindow::GetFamilyAndStyle(font_family *fam, font_style *st)
{
	strcpy(*fam, this->family);
	strcpy(*st, this->style);
}

void PropWindow::SetFamilyAndStyle(const font_family family, const font_style style)
{
	strcpy(this->family, family);
	strcpy(this->style, style);
}


PropView::PropView(BRect frame)
		 :BView(frame, "Details", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS),
	fontboy(NULL),
	prefs(NULL),
	topview(NULL),
	leftview(NULL),
	csview(NULL),
	sp(NULL)
{
	fontboy = dynamic_cast<Fontboy*>(be_app);
	prefs = fontboy->prefs;
}

PropView::~PropView(void)
{
}

void PropView::AttachedToWindow(void)
{
	BRect			rect;
	font_family		family;
	font_style		style;

	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	rect = Bounds();
	rect.bottom = rect.top + TopView::HEIGHT;
	topview = new TopView(rect, this);
	AddChild(topview);

	// LeftView
	rect = Bounds();
	rect.top += TopView::HEIGHT;
	rect.right = rect.left + LeftView::DEF_WIDTH + PropView::BORDER_PADDING;
	leftview = new LeftView(rect);

	// CharSetView
	rect = Bounds();
	rect.top += TopView::HEIGHT;
	rect.left += LeftView::DEF_WIDTH + PropView::BORDER_PADDING;
	
	propwin->GetFamilyAndStyle(&family, &style);
	
	csview = new CharSetView(rect, family, style, new BMessage(M_NEWCHAR));
	csview->SetBGColor(prefs->GetPBgColor());
	csview->SetTextColor(prefs->GetPDisplayColor());
	csview->SetSelectedBGColor(prefs->GetPBgColor(true));
	csview->SetSelectedTextColor(prefs->GetPDisplayColor(true));
	csview->SetInactiveSelectedBGColor(prefs->GetPInactiveSelectedBgColor());
	csview->SetInactiveSelectedTextColor(prefs->GetPInactiveSelectedDisplayColor());
	csview->SetStrokeColor(prefs->GetPStrokeColor());
	csview->SetPadding(5);

	// Splitpane
	rect = Bounds();
	rect.top += TopView::HEIGHT;
	sp = new SplitView(rect, leftview, csview);
	sp->SetBarPosition(prefs->GetSplitPanePos());
	AddChild(sp);
	sp->UpdateLimits();

	csview->SetTarget(this);
	csview->MakeFocus(true);

	// set window sizelimits
	float minWidth, maxWidth, minHeight, maxHeight;
	int fontinfo_height = int(LeftView::FONTINFO_PADDING + (LeftView::FONTINFO_LINE_HEIGHT*4));
	
	Window()->GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
	minHeight = SplitView::SV_MINSIZE_ONE + fontinfo_height + TopView::HEIGHT;
	minWidth = SplitView::SV_MINSIZE_ONE + SplitView::SV_MINSIZE_TWO;
	Window()->SetSizeLimits(minWidth, maxWidth, minHeight, maxHeight);
}

void PropView::MessageReceived(BMessage *msg)
{
	BString *bstr = NULL;
	char *str;
	ssize_t size;

	switch(msg->what) {
		case M_NEWCHAR:
			if (leftview != NULL)
				Looper()->PostMessage(msg, leftview);

			if (topview != NULL)
				Looper()->PostMessage(msg, topview);
			break;

		case M_PREV_PAGE:
			if (csview != NULL)
				Looper()->PostMessage(msg, csview);
			break;

		case M_NEXT_PAGE:
			if (csview != NULL)
				Looper()->PostMessage(msg, csview);
			break;

		case B_SIMPLE_DATA:
			if (msg->FindData("text/plain", B_MIME_TYPE, (const void**) &str, &size) == B_OK) {
				bstr = new BString(str);
				if (bstr != NULL) {
					if (bstr->CountChars() == 1) {
						break;
					}
				}
			}
			break;

		default:
			BView::MessageReceived(msg);
	}
}

void PropView::SetCharPos(uint16 unicode)
{
	BMessage copy(M_NEWCHAR);
	copy.AddInt32("char", (unicode >> 8));
	copy.AddInt32("page", (unicode && 0xff));
	if (leftview != NULL)
		Looper()->PostMessage(&copy, leftview);

	if (topview != NULL)
		Looper()->PostMessage(&copy, topview);

	if (csview != NULL)
		csview->SetCharPos(unicode);
}

int PropView::GetSliderPos()
{
	return sp->GetBarPosition();;
}

void PropView::SetSliderPos(int pos, bool force)
{
	if ((!sp->IsBarLocked())
	||	(force == true))
		sp->SetBarPosition(pos);;
}

bool PropView::IsSliderPosLocked()
{
	return sp->IsBarLocked();
}

// This should be called before BWindow::Zoom()
void PropView::Zoom(float width, float height)
{
	float n =  Window()->Bounds().Width() / sp->GetBarPosition();
	int sp_new_barpos = int(width / n);
	sp->UpdatePos(width, height, sp_new_barpos);
}

void PropView::LockSliderPos(bool val)
{
	sp->SetBarLocked(val);
}

void PropView::Update()
{
	font_family	family;
	font_style	style;

	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());
	propwin->GetFamilyAndStyle(&family, &style);
	
	if (Window()->Lock()) {
		topview->Invalidate();
		leftview->Update();
		csview->SetDrawMode(true);
		csview->SetFamilyAndStyle(family, style);
		csview->SetBGColor(prefs->GetPBgColor());
		csview->SetTextColor(prefs->GetPDisplayColor());
		csview->SetSelectedBGColor(prefs->GetPBgColor(true));
		csview->SetSelectedTextColor(prefs->GetPDisplayColor(true));
		csview->SetInactiveSelectedBGColor(prefs->GetPInactiveSelectedBgColor());
		csview->SetInactiveSelectedTextColor(prefs->GetPInactiveSelectedDisplayColor());
		csview->SetStrokeColor(prefs->GetPStrokeColor());
		csview->Invalidate();
		Window()->Unlock();
	}
}

SplitView::SplitView(BRect frame, BView *one, BView *two)
:SplitPane(frame, one, two, B_FOLLOW_ALL_SIDES)
{
	SetMinSizeOne(SV_MINSIZE_ONE);
	SetMinSizeTwo(SV_MINSIZE_TWO);
	SetBarThickness(0);
	SetViewInsetBy(0);
	SetBarAlignmentLocked(true);
}

SplitView::~SplitView()
{
}

void SplitView::MouseMoved(BPoint where,uint32 info,const BMessage *m)
{
	UpdateLimits(where.x - here.x);
	SplitPane::MouseMoved(where, info, m);
}

void SplitView::FrameResized(float aWidth, float aHeight)
{
	SplitView::Update();
	UpdateLimits(aWidth, aHeight);
}

void SplitView::UpdatePos(float width, float height, int i)
{
	if (!IsBarLocked()) {
		pos = i;
	}
	else {
		// prevents <--> control to be moved out of bounds
		int fontinfo_height = int(LeftView::FONTINFO_PADDING + (LeftView::FONTINFO_LINE_HEIGHT*4));
		int maxpos = int(width) - fontinfo_height - SplitView::SV_MINSIZE_TWO;

		if ((pos) > maxpos) {
			pos = maxpos;
			SetMinSizeTwo((int)width - pos);
		}
	}

	UpdateLimits(width, height);
}

void SplitView::UpdateLimits(float movement)
{
	UpdateLimits(Bounds().Width(), Bounds().Height(), movement);
}

void SplitView::UpdateLimits(float width, float height, float movement)
{
	// prevents <--> control to be moved out of bounds
	int fontinfo_height = int(LeftView::FONTINFO_PADDING + (LeftView::FONTINFO_LINE_HEIGHT*4));

	if (!IsBarLocked()) {
		if((pos + fontinfo_height + int(movement)) > (int)height) {
			pos = (int)height - fontinfo_height;
			SetMinSizeTwo((int)width - pos);
		}
		else {
			SetMinSizeTwo(SV_MINSIZE_TWO);
		}
	}

	// set window sizelimits
	float minWidth, maxWidth, minHeight, maxHeight;

	if (Window()->Lock()) {
		if (IsBarLocked()) {
			Window()->GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
			minHeight = pos + fontinfo_height + TopView::HEIGHT;
			minWidth = pos + SplitView::SV_MINSIZE_TWO;
			Window()->SetSizeLimits(minWidth, maxWidth, minHeight, maxHeight);
		}
		else {
			Window()->GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
			minHeight = SplitView::SV_MINSIZE_ONE + fontinfo_height + TopView::HEIGHT;
			minWidth = SplitView::SV_MINSIZE_ONE + SplitView::SV_MINSIZE_TWO;
			Window()->SetSizeLimits(minWidth, maxWidth, minHeight, maxHeight);
		}
		Window()->Unlock();
	}
}

void SplitView::Update()
{
	SplitPane::Update();
	UpdateLimits();
}

LeftView::LeftView(BRect frame)
:BView(frame, "Left", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS),
	fontboy(NULL),
	prefs(NULL),
	cview(NULL),
	schubknopf(NULL)
{
	fontboy = dynamic_cast<Fontboy*>(be_app);
	prefs = fontboy->prefs;
}

LeftView::~LeftView(void)
{
	if (schubknopf != NULL)
		delete schubknopf;
}

void LeftView::AttachedToWindow(void)
{
	font_family	family;
	font_style	style;
	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());

	propwin->GetFamilyAndStyle(&family, &style);

	SetViewColor(Parent()->ViewColor());

	BRect rect = Bounds();
	rect.InsetBy(5,5);
	rect.bottom = rect.top + rect.Width();
	cview = new CharView(rect, family, style);
	cview->SetBGColor(prefs->GetPBgColor());
	cview->SetDisplayColor(prefs->GetPDisplayColor());
	cview->SetStrokeColor(prefs->GetPStrokeColor());
	cview->SetChar(0);
	AddChild(cview);
}

void LeftView::FrameResized(float width, float height)
{
	BRect rect = Bounds();
	rect.InsetBy(5, 5);
	if (width != 0) {
		rect.bottom = rect.top + rect.Width();
		cview->ResizeTo(rect.Width(), rect.Height());
	}
	Invalidate();
}

void LeftView::Update()
{
	font_family	family;
	font_style	style;
	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());

	propwin->GetFamilyAndStyle(&family, &style);

	cview->SetDrawMode(true);
	cview->SetFamilyAndStyle(family, style);
	cview->SetBGColor(prefs->GetPBgColor());
	cview->SetDisplayColor(prefs->GetPDisplayColor());
	cview->SetStrokeColor(prefs->GetPStrokeColor());
	cview->Invalidate();
	Invalidate();
}

void LeftView::MessageReceived(BMessage *msg)
{
	uint16	unicode;

	switch(msg->what) {
		case M_NEWCHAR:
			if (B_OK == msg->FindInt16("unicode", (int16*)&unicode)) {
				cview->SetChar(unicode);
				cview->Invalidate();
			}
			break;

		default:
			BView::MessageReceived(msg);
	}
}

void LeftView::MouseDown(BPoint where)
{
	// redirect mouseevents for <--> control
	if ((schubknopf != NULL)
	&&	(schubknopf->IsValid())) {

		// get state of mousebuttons
		int32 buttons = 0;
		Window()->CurrentMessage()->FindInt32("buttons", &buttons);

		// check for doubleclick
		if (buttons & B_SECONDARY_MOUSE_BUTTON) {

			PropWindow *propwin = dynamic_cast<PropWindow*>(Window());

			if (propwin != NULL)
				propwin->LockSplitpane(!propwin->IsSplitpaneLocked());
		}
		else {
			BRect rect = Bounds();
			rect.InsetBy(5, 5);
			rect.top = rect.bottom - schubknopf->Bounds().Height();
			rect.left = rect.right - schubknopf->Bounds().Width();
			BMessage *currentMsg = Window()->CurrentMessage();
	
			if ((currentMsg->what == B_MOUSE_DOWN)
			&&	(rect.Contains(where))) {
				Looper()->PostMessage(currentMsg, Parent());
			}
		}
	}
}

void LeftView::Draw(BRect urect)
{
	BRect		rect, l_rect, r_rect;
	BFont		font;
	font_family	family;
	font_style	style;
	float		cv_height = 100.0;

	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());

//	Fill lighter grey area
	rect = l_rect = Bounds();
	rect.left += PropView::DARKGREY_DECOR_WIDTH;
	SetHighColor(ViewColor());
	FillRect(rect);

//	Fill dark grey area
	rect = l_rect;
	rect.right = rect.left + PropView::DARKGREY_DECOR_WIDTH;
	SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_1_TINT));
	FillRect(rect);

//	add Fontinfo below CharView
	if (cview)
		cv_height = cview->Bounds().Height();

	l_rect.left += FONTINFO_PADDING;
	l_rect.top += cv_height + FONTINFO_PADDING + 15;	// da müssen wir nochmal ran!
	l_rect.bottom = l_rect.top + FONTINFO_LINE_HEIGHT;
	r_rect = l_rect;

	l_rect.right = PropView::DARKGREY_DECOR_WIDTH;
	r_rect.left = PropView::DARKGREY_DECOR_WIDTH + 5;

	SetHighColor(0, 0, 0);
	SetLowColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_1_TINT));
	DrawTruncString(l_rect, "Family:");

	l_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
	DrawTruncString(l_rect, "Style:");

	l_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
	DrawTruncString(l_rect, "Type:");

	l_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
	DrawTruncString(l_rect, "Fixed:");

	propwin->GetFamilyAndStyle(&family, &style);
	font.SetFamilyAndStyle(family, style);

	SetLowColor(ViewColor());
	DrawTruncString(r_rect, family);

	r_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
	DrawTruncString(r_rect, style);

	r_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
	switch (font.FileFormat()) {
		case B_TRUETYPE_WINDOWS:
			DrawTruncString(r_rect, "Windows True Type");
			break;

		case B_POSTSCRIPT_TYPE1_WINDOWS:
			DrawTruncString(r_rect, "Windows Postscript");
			break;

		default:
			DrawTruncString(r_rect, "Unknown");
	}

	r_rect.OffsetBy(0, FONTINFO_LINE_HEIGHT);
 	if (font.IsFixed())
		DrawTruncString(r_rect, "yes");
 	else
		DrawTruncString(r_rect, "no");

//	add resize control
	if (schubknopf == NULL)
		schubknopf = BTranslationUtils::GetBitmap("schubknopf", NULL);

//	draw resize control
	if ((schubknopf != NULL)
	&&	(schubknopf->IsValid())) {
		bottom_bounds = r_rect.bottom;

		rect = Bounds();
		rect.InsetBy(5, 5);
	
		BPoint point(rect.right - schubknopf->Bounds().Width(), rect.bottom - schubknopf->Bounds().Height());
		SetDrawingMode(B_OP_COPY);
		DrawBitmap(schubknopf, point);
	}
}

void LeftView::DrawTruncString(BRect rect, const char *text)
{
	BFont			font;
	char			atext[128];
	const char		*textinput[1];
	char			*textoutput[1];

	*textinput = text;
	*textoutput = atext;
	SetFont(be_plain_font);
	GetFont(&font);
	font.GetTruncatedStrings(textinput, 1, B_TRUNCATE_END, rect.Width(), textoutput);
	DrawString(atext, rect.LeftTop());
}

TopView::TopView(BRect frame, BView *target)
:BView(frame, "Top", B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_FRAME_EVENTS),
	targetview(target),
	ttf_icon(NULL),
	ps_icon(NULL),
	old_pagepos(0)
{
}

TopView::~TopView(void)
{
	if (ttf_icon != NULL)
		delete ttf_icon;

	if (ps_icon != NULL)
		delete ps_icon;
}

BPicture *TopView::GetPictureFromBitmap(const char *bitmap)
{
	BPicture *pic = NULL;

	BBitmap	*bmap = BTranslationUtils::GetBitmap(bitmap, NULL);
	if (bmap != NULL) {
		BRect vrect = bmap->Bounds();
		BView *tview = new BView(vrect, "temp", B_FOLLOW_NONE, B_WILL_DRAW );
		AddChild(tview);
		tview->BeginPicture(new BPicture); 
		tview->SetDrawingMode(B_OP_ALPHA);
		tview->DrawBitmap(bmap);
		pic = tview->EndPicture();
		
		//get rid of temp views
		RemoveChild(tview);
		delete tview;
		delete bmap;
	}
	else {
		printf("error reading bitmap %s\n", bitmap);
	}
	return pic;
}

void TopView::AttachedToWindow(void)
{
	BFont	font;
	BRect	trect;

	SetViewColor(Parent()->ViewColor());

	BRect rect = Bounds();
	rect.InsetBy(5,5);

	BPicture *npon = GetPictureFromBitmap("npage_on");
	if (npon != NULL) {
		BPicture *npoff = GetPictureFromBitmap("npage_off");
		if (npoff != NULL) {

			//create a picture button using the two pictures
			trect = rect;
			trect.bottom = trect.top + 32;
			trect.left = trect.right - 32;
			npage = new BPictureButton(trect, 
										"picture", 
										npoff, 
										npon, 
										new BMessage(M_NEXT_PAGE), 
										B_ONE_STATE_BUTTON,
										B_FOLLOW_RIGHT | B_FOLLOW_TOP);
			AddChild(npage);
			npage->SetTarget(targetview);
			npage->SetToolTip("Show next page");
			delete npoff;
		}
		delete npon;
	}
			
	BPicture *ppon = GetPictureFromBitmap("ppage_on");
	if (ppon != NULL) {
		BPicture *ppoff = GetPictureFromBitmap("ppage_off");
			if (ppoff != NULL) {
			trect.right -= 32;
			trect.left = trect.right - 32;
			ppage = new BPictureButton(trect, 
										"picture", 
										ppoff, 
										ppon, 
										new BMessage(M_PREV_PAGE), 
										B_ONE_STATE_BUTTON,
										B_FOLLOW_RIGHT | B_FOLLOW_TOP);
			AddChild(ppage);
			ppage->SetTarget(targetview);
			ppage->SetToolTip("Show previous page");
			delete ppoff;
		}
		delete ppon;
	}
			
	// add ASCII Info
	trect.bottom = trect.top + 16;
	trect.right = trect.left - 1;
	trect.left = trect.right - 80;
	BRect trect2 = trect;
	currascii_value = new BStringView(trect, B_EMPTY_STRING, NULL, B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	currascii_value->SetAlignment(B_ALIGN_LEFT);	
	currascii_value->GetFont(&font);
	font.SetSize(10.0);
	currascii_value->SetFont(&font);
 	AddChild(currascii_value);

	trect.right = trect.left + 4;
	trect.left = trect.right - 60;
	currascii = new BStringView(trect, B_EMPTY_STRING, "Selected: ", B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	currascii->SetAlignment(B_ALIGN_RIGHT);
	currascii->SetFont(&font);
 	AddChild(currascii);

	trect = trect2;
	trect.top += 17;
	trect.bottom = trect.top + 16;
	rangeascii_value = new BStringView(trect, B_EMPTY_STRING, NULL, B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	rangeascii_value->SetAlignment(B_ALIGN_LEFT);
	rangeascii_value->SetFont(&font);
 	AddChild(rangeascii_value);

	trect.right = trect.left + 4;
	trect.left = trect.right - 60;
	rangeascii = new BStringView(trect, B_EMPTY_STRING, "Pagerange: ", B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	rangeascii->SetAlignment(B_ALIGN_RIGHT);
	rangeascii->SetFont(&font);
 	AddChild(rangeascii);
}

void TopView::AllAttached()
{

}

void TopView::MessageReceived(BMessage *msg)
{
	uint16	unicode;

	switch(msg->what) {
		case M_NEWCHAR:
			msg->FindInt16("unicode", (int16*)&unicode);
			Update(unicode);
			break;

		default:
			BView::MessageReceived(msg);
	}
}

void TopView::FrameResized(float width, float height)
{
	Invalidate();
}

void TopView::Update(uint16 chr)
{
	char	atext[32];

	if (chr != old_pagepos) {

		sprintf(atext, "%d", int(chr));
		currascii_value->SetText(atext);

		int32 page = chr / 256;
		sprintf(atext, "%d - %d", int(page * 256), int(page * 256 + 255));
		rangeascii_value->SetText(atext);

		old_pagepos = chr;
	}
	Invalidate();
}

void TopView::Draw(BRect urect)
{
	BFont			font;
	BBitmap			*ficon = NULL;
	font_family		family;
	font_style		style;
	char			atext[128];
	const char		*textinput[1];
	char			*textoutput[1];

	PropWindow	*propwin = dynamic_cast<PropWindow*>(Window());

	propwin->GetFamilyAndStyle(&family, &style);
	font.SetFamilyAndStyle(family, style);

	urect = Bounds();
	urect.right = urect.left + PropView::DARKGREY_DECOR_WIDTH;
	SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_1_TINT));
	FillRect(urect);

	// select TrueType or Postscript icon
	switch (font.FileFormat()) {
		case B_TRUETYPE_WINDOWS:
			if (ttf_icon == NULL)
				if ((ttf_icon = BTranslationUtils::GetBitmap("ttficon", NULL)) == NULL)
					printf("error reading bitmap %s\n", "ttficon");
			ficon = ttf_icon;
			break;

		case B_POSTSCRIPT_TYPE1_WINDOWS:
			if (ps_icon == NULL)
//				we activate it when we have a nice icon
//				ps_icon = BTranslationUtils::GetBitmap("psicon", NULL);
				if ((ps_icon = BTranslationUtils::GetBitmap("ttficon", NULL)) == NULL)
					printf("error reading bitmap %s\n", "psicon");
			ficon = ps_icon;
			break;
	}

	// print icon
	if (ficon != NULL) {
		if (ficon->IsValid()) {
			BPoint point((PropView::DARKGREY_DECOR_WIDTH - ficon->Bounds().Width()) / 2, 5);
			SetDrawingMode(B_OP_ALPHA);
			DrawBitmap(ficon, point);
		}
	}

	urect = Bounds();
	urect.left += PropView::DARKGREY_DECOR_WIDTH + 5;
	urect.right -= 208;
	urect.top += 28;

	BString name = family;
	name += " ";
	name += style;

	GetFont(&font);
	font.SetSize(20.0);
	font.SetSpacing(B_CHAR_SPACING);
	SetFont(&font);
	SetHighColor(0, 0, 0);

	*textinput = name.String();
	*textoutput = atext;
	font.GetTruncatedStrings(textinput, 1, B_TRUNCATE_END, urect.Width(), textoutput);
	DrawString(atext, urect.LeftTop());
}
