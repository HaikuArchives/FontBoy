/* ListView for displaing Fonts */

#include "Fontboy.h"
#include "Prefs.h"
#include "ListView.h"
#include "ListWindow.h"
#include "FontList.h"
#include "MsgVals.h"

#ifdef FFont_Support
#include "FFont.h"
#endif

#include "Debug.h"

ListView::ListView(BRect frame)
		 :MultiBoxView(frame)
{
	fontboy = dynamic_cast<Fontboy*>(be_app);
	prefs = fontboy->prefs;
}

ListView::~ListView()
{
}

void ListView::FrameResized(float aWidth, float aHeight)
{
	MultiBoxView::UpdateProperties();
	Init();
	MultiBoxView::FrameResized(aWidth, aHeight);
}

void ListView::DrawContentBox(int32 element)
{
	BPoint			point, point2;
	BFont			infofont(be_plain_font), font(be_plain_font);
	BRect 			rect;
	struct fnode	*fontptr;
	char			atext[128];
	const char		*textinput[1];
	char			*textoutput[1];
	float			x, y;

	fontlist = dynamic_cast<Fontboy*>(be_app)->GetFontList();
	
	if (fontlist != NULL) {	
		GetRectAt(element, &rect);
	
		SetDrawingMode(B_OP_COPY);
		SetPenSize(0);
	
		fontptr = fontlist->FontAt(fontlist->GetFirstInView() + element);
	
		// Fill boxes at end of list with background color
		if (fontptr == NULL) {
			SetHighColor(def_viewcolor);
			// adjust rectsize for borders
			rect.left++;
			rect.top++;
			FillRect(rect);
		}
		// Draw Fontbox
		else {
			if (fontptr->selected) {
				SetHighColor(prefs->GetMSelectColor());
				SetLowColor(prefs->GetMSelectColor());
			}
			else {
				SetHighColor(prefs->GetMBgColor());
				SetLowColor(prefs->GetMBgColor());
			}
			x = rect.left;
			y = rect.top;
			FillRect(rect);
		
			float cwidth = GetColWidth();
		
			// Stroke line for base + ascent & decent height
			if (prefs->GetDrawHeights()) {
				SetHighColor(prefs->GetMHeightsColor());
				point.Set(x, y + dist);
				point2.Set(x + cwidth, y + dist);
				StrokeLine(point, point2);
				point.Set(x, y + dist + th_base);
				point2.Set(x + cwidth, y + dist + th_base);
				StrokeLine(point, point2);
				point.Set(x, y + dist + th_all);
				point2.Set(x + cwidth, y + dist + th_all);
				StrokeLine(point, point2);
			}
		
			// Stroke line around fontrect
			SetHighColor(prefs->GetMStrokeColor());
			if (prefs->GetDrawBorder()) {
				StrokeRect(rect);
			}
		
			// Set current font to display
			point.Set(x + leftdist, y + dist + th_base);
			font.SetFamilyAndStyle(fontptr->family, fontptr->style);
			font.SetSize(prefs->GetFontSize());
			if (prefs->GetFontSize() > 18)		// Set correct spacing for current fontsize
				font.SetSpacing(B_CHAR_SPACING);
			else
				font.SetSpacing(B_STRING_SPACING);
			SetFont(&font);
		
			// copy and maybe truncate text to display
			SetHighColor(prefs->GetMDisplayColor());
			strcpy(atext, (const char *)prefs->GetDisplayText()->String());
			*textinput = *textoutput = atext;
			font.GetTruncatedStrings(textinput, 1, B_TRUNCATE_END, cwidth - leftdist, textoutput);
			DrawString(atext, point);
		
			// prepare font and text for fontinformation
			SetHighColor(prefs->GetMInfoColor());
			point.Set(x + leftdist, y + dist + ih_base);
			SetFont(&infofont);
			sprintf(atext, "%s %s", fontptr->family, fontptr->style);
			*textinput = *textoutput = atext;
			infofont.GetTruncatedStrings(textinput, 1, B_TRUNCATE_END, cwidth - leftdist, textoutput);
			DrawString(atext, point);
		}
	}
}

void ListView::ScrollTo(BPoint apoint)
{
	CalcLeader();
	Draw(Bounds());
}

void ListView::MouseDown(BPoint point)
{
	struct fnode	*fontptr;
	BPopUpMenu		*popup;
	BMenuItem		*item;
	BMessage		*dragmsg;
	BString			dragtext;
	BPoint			dragpoint;
	BRect			rect, dragrect;
	int32 			clicks = 0;
	int32			buttons = 0;
	uint32			dragbuttons = 0;
#ifdef FFont_Support
	FFont			font;
#endif

	rect = Bounds();
	rect.top = GetNumRows() * GetRowHeight() + 1;
	if (rect.Contains(point))
		return;

	Window()->CurrentMessage()->FindInt32("clicks", &clicks);
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);

	// check for doubleclick
	if ((clicks >= 2) && (buttons & B_PRIMARY_MOUSE_BUTTON)) {
		GetRectAt(point, &rect);
		if (rect.Contains(lastPoint)) {
			be_app->PostMessage(M_DETAILS);
			return;
		}
	}
	lastPoint = point;

	fontlist = dynamic_cast<Fontboy*>(be_app)->GetFontList();
	if (fontlist == NULL)
		return;

	// update to new selected font
	fnode *sel = fontlist->FontAt(fontlist->GetSelected());
	
	// Get FontPtr & Rect at Point x,y
	fontptr = fontlist->FontAt(GetElementAt(point) + fontlist->GetFirstInView());
	if (fontptr != NULL) {
		if (fontptr != sel) {
			fontlist->SetSelected(fontptr->listpos);
			if (sel != NULL)
				DrawContentBox(sel->listpos - fontlist->GetFirstInView());
			DrawContentBox(fontptr->listpos - fontlist->GetFirstInView());
		}

		// check for right mousebutton
		if(buttons & B_SECONDARY_MOUSE_BUTTON) {
			popup = new BPopUpMenu("PopUp", false);
			popup->AddItem(item = new BMenuItem("Details", new BMessage(M_DETAILS)));
			popup->AddSeparatorItem();
			popup->AddItem(item = new BMenuItem("Refresh", new BMessage(M_REFRESH)));
			ConvertToScreen(&point);
			item = popup->Go(point);
			if (item) {
				be_app->PostMessage(item->Message()->what);
			}
			return;
		}

		dragrect.Set(point.x - 3, point.y - 3, point.x + 3, point.y + 3);
		GetMouse(&dragpoint, &dragbuttons, true);
		while (dragbuttons) {
			snooze(20 * 1000);
			GetMouse(&dragpoint, &dragbuttons, true);
			if (!dragrect.Contains(dragpoint)) {
#ifndef FFont_Support
				dragmsg = new BMessage('Font');
#else
				dragmsg = new BMessage('!FNT');
				font.SetFamilyAndStyle(fontptr->family, fontptr->style);
				font.SetSize(prefs->GetFontSize());
				if(AddMessageFont(dragmsg, "font", &font) != B_NO_ERROR) { 
					printf("Error while adding FFont class to Drag & Drop Message!\n");
				}
#endif
				dragtext = fontptr->family;
				dragtext += " ";
				dragtext += fontptr->style;
				dragmsg->AddData("text/plain", B_MIME_TYPE, dragtext.String(), dragtext.Length());
	
				dragtext = "<FONT FACE=\"";
				dragtext += fontptr->family;
				dragtext += "\"></FONT>\0";
				dragmsg->AddData("text/html", B_MIME_TYPE, dragtext.String(), dragtext.Length());
	
				// calc rectangle for fontname
				BFont font(be_plain_font);
				font.SetFamilyAndStyle(fontptr->fsysvalue);
				font.SetSize(20.0);
				char atext[256];
				sprintf(atext, "%s %s", fontptr->family, fontptr->style);
				const char* strings[1] = { atext };
				BRect rrects[1];
				escapement_delta zero_escapements[1] = {{ 0,0 }};
				font.GetBoundingBoxesForStrings(strings,1,B_SCREEN_METRIC,zero_escapements,rrects);
				BRect rect(rrects[0]);
				rect.InsetBy(-dragdist, -dragdist);
				rect.OffsetTo(0, 0);
	
				//rect.Set(0, 0, 150, 50);
				BBitmap *dbmp = new BBitmap(rect, B_RGB32, true);
				BView *dview = new BView(rect, "temp", B_FOLLOW_NONE, B_WILL_DRAW);
				if (dbmp && dview) {
					dbmp->Lock();
					dbmp->AddChild(dview);
					rgb_color c = prefs->GetMSelectColor();
					c.alpha = 196;
					dview->SetLowColor(c);
					dview->SetHighColor(c);
					dview->FillRect(rect);
					c = prefs->GetMStrokeColor();
					c.alpha = 196;
					dview->SetHighColor(c);
					dview->StrokeRect(rect);
					c = prefs->GetMDisplayColor();
					c.alpha = 196;
					dview->SetHighColor(c);
					dview->SetFont(&font);
					font_height theight;
					font.GetHeight(&theight);
	//				_sPrintf("asc: %f, desc: %f, lead: %f\n", theight.ascent, theight.descent, theight.leading);
					dview->DrawString(atext, BPoint(dragdist, dragdist + theight.ascent - theight.descent));
					dbmp->RemoveChild(dview);
					dbmp->Unlock();
					delete dview;
	 				DragMessage(dragmsg, dbmp, B_OP_ALPHA, BPoint(10, 10));
				}
			}
		}
	}
}

void ListView::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		default:
			BView::MessageReceived(msg);
			break;
	}
}

void ListView::Init()
{
	BRect frame = Bounds();
	prefs->SetMViewRect(&frame);
	prefs->Update();

	float rheight = prefs->GetRowHeight();
	SetRowHeight(rheight);

	BFont font(be_plain_font);
	font.GetHeight(&infoheight);
	ih_base = dist + infoheight.ascent;

	font.SetSize(prefs->GetFontSize());
	font.GetHeight(&textheight);
	th_base = textheight.ascent;
	th_all = textheight.ascent + textheight.descent;
	ih_base += th_all;

	SetAutoColumns(prefs->GetAutoAdjust());

	float swidth = font.StringWidth(prefs->GetDisplayText()->String());
	swidth += swidth / 8.0;

	if (prefs->GetAutoAdjust()) {
		// calculate number of columns
		int32 ncols = int32(frame.right / swidth);
		if (ncols < 1)
			ncols = 1;
		SetNumCols(ncols);
	}
	else {
		SetNumCols(prefs->GetNumCols());
	}

	float minw, maxw, minh, maxh;
	Window()->GetSizeLimits(&minw, &maxw, &minh, &maxh);
	Window()->SetSizeLimits(100, maxw, rheight + 33 + B_H_SCROLL_BAR_HEIGHT, maxh);
	BRect aRect(0, 0, swidth + 10, rheight + B_H_SCROLL_BAR_HEIGHT);
	if (aRect.Contains(Window()->Bounds())) {
		aRect = aRect | Window()->Bounds();
		aRect = Window()->ConvertToScreen(aRect) & BScreen(B_MAIN_SCREEN_ID).Frame();
		aRect.bottom -= 25;
		aRect = Window()->ConvertFromScreen(aRect);
		Window()->ResizeTo(aRect.right, aRect.bottom);
	}

	UpdateScrollBar();
	CalcLeader();
}

// needs ScrollBarUpdate before Call!
void ListView::CalcLeader()
{
	struct fnode	*fontptr;
	BScrollBar		*hsb, *vsb;
	int32 			sval;

	fontlist = dynamic_cast<Fontboy*>(be_app)->GetFontList();
	if (fontlist != NULL) {

		fontptr = fontlist->FirstFont();

		if (int(GetNumCols()) == 1) {
			vsb=ScrollBar(B_VERTICAL);
			sval = int32(vsb->Value() - 1);
			for (int32 i = 0; i < sval; i++)
				fontptr = fontlist->GetNext(fontptr);
		}

		else {
			hsb=ScrollBar(B_HORIZONTAL);
			sval = int32((hsb->Value() - 1) * GetNumRows());
			for (int32 i = 0; i < sval; i++)
				fontptr = fontlist->GetNext(fontptr);
		}

		fontlist->SetFirstInView(fontptr->listpos);
	}
}

void ListView::UpdateScrollBar()
{
	fontlist = dynamic_cast<Fontboy*>(be_app)->GetFontList();
	if (fontlist != NULL) {

		float nrows = GetNumRows();
		float ncols = GetNumCols();
		BScrollBar *hsb=ScrollBar(B_HORIZONTAL);
		BScrollBar *vsb=ScrollBar(B_VERTICAL);

		if (int(GetNumCols()) != 1) {
			float max = ceil(fontlist->CountFonts() / nrows) - ncols;
			float prop = ceil(fontlist->CountFonts() / nrows) / ncols;
			hsb->SetRange(1, max+1);
			hsb->SetProportion(1 / prop);
			hsb->SetSteps(1, ncols);
			vsb->SetRange(0, 0);
		}

		else {
			float max = fontlist->CountFonts() - nrows;
			float prop = ceil(fontlist->CountFonts() / nrows);
			vsb->SetRange(1, max+1);
			vsb->SetProportion(1 / prop);
			vsb->SetSteps(1, nrows);
			hsb->SetRange(0, 0);
		}
	}
}

void ListView::PrintRect(BRect *rect)
{
	_sPrintf("Rect-> X:%f, Y:%f, W:%f, H:%f\n", rect->left, rect->top, rect->Width(), rect->Height());
}
