
#include "CharSetView.h"

#include <UnicodeChar.h>

#include "MsgVals.h"
#include "ConstColors.h"
#include "Debug.h"

CharSetView::CharSetView(BRect frame, font_family fam, font_style sty, BMessage *msg)
			:BControl(frame, "CharSetView", B_EMPTY_STRING, msg, B_FOLLOW_ALL_SIDES, B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE | B_WILL_DRAW | B_FRAME_EVENTS),
	drawmode(false)
{
	strcpy(family, fam);
	strcpy(style, sty);
	font.SetFamilyAndStyle(family, style);
	charpos = 0;
	pad = 0;
	SetViewColor(B_TRANSPARENT_32_BIT);
	clist = new CharList(&font);
}

void CharSetView::FrameResized(float aWidth, float aHeight)
{
}

void CharSetView::AttachedToWindow(void)
{
	SetCharPos(charpos);
}

void CharSetView::Draw(BRect urect)
{
	SetDrawingMode(B_OP_COPY);
	SetLowColor(Parent()->ViewColor());
	SetHighColor(Parent()->ViewColor());
	SetPenSize(0);

	BRect vrect = Bounds();
	if (pad != 0)
	{
		BRect r = vrect;
		r.right = r.left + pad;
		FillRect(r);
		r = vrect;
		r.bottom = r.top + pad;
		FillRect(r);
		r = vrect;
		r.left = r.right - pad;
		FillRect(r);
		r = vrect;
		r.top = r.bottom - pad;
		FillRect(r);
		vrect.InsetBy(pad, pad);
	}

	SetLowColor(bgColor);

	colwidth = (vrect.right - vrect.left) / numcols;
	rowheight = (vrect.bottom - vrect.top) / numrows;

	if (drawmode) {
		float fh = (colwidth > rowheight) ? rowheight : colwidth;
		font.SetSize(fh * 0.6);
		font.GetHeight(&fheight);
		SetFont(&font);

		int32 cpos = GetPageStartPos();
		clist->PrepareRange(cpos, (numrows*numcols));
		for (int32 i = 0; i < numrows; i++) {
			for (int32 j = 0; j < numcols; j++) {
				if (urect.Intersects(GetCharRect(cpos)))
					DrawCharBox(cpos, cpos == charpos ? true : false);
				cpos++;
			}
		}
	}
	else {
		BRect rect = Bounds();
		rect.InsetBy(pad, pad);
		SetHighColor(bgColor);
		FillRect(rect);
		SetHighColor(strokeColor);
		StrokeRect(rect);
	}
}

void CharSetView::DrawCharBox(uint16 cpos, bool selected)
{
	if (selected) {
		if (IsFocus()) {
			SetHighColor(selectedBgColor);
			SetLowColor(selectedBgColor);
		}
		else {
			SetHighColor(inactiveSelectedBgColor);
			SetLowColor(inactiveSelectedBgColor);
		}
	}
	else {
		SetHighColor(bgColor);
		SetLowColor(bgColor);
	}
	BRect crect = GetCharRect(cpos);
	FillRect(crect);
	SetHighColor(strokeColor);
	StrokeRect(crect);

	cnode *node = clist->CharAt(cpos);
	if (node->flag & CL_HASGLYPH) {
		if (selected) 
			SetHighColor(IsFocus() ? selectedTextColor : inactiveSelectedTextColor);
		else 
			SetHighColor(textColor);

	
		// Unicode to UTF8 Character encoding
		char utf8chars[3+1];
		int32 state = 0;
		int32 srcLen = 2;
		int32 destLen = sizeof(utf8chars);
		convert_to_utf8(B_UNICODE_CONVERSION, (const char*) &node->unicode, &srcLen,
						utf8chars, &destLen, &state);

		float choffset = (crect.right - crect.left - StringWidth(utf8chars, destLen)) / 2;
		float cvoffset = (rowheight - fheight.ascent - fheight.descent) / 2 + fheight.ascent;
		BPoint dpoint(crect.left + choffset, crect.top + cvoffset);
		DrawString(utf8chars, destLen, dpoint);
	}

/*	if (cpos == 0xffff) {
		printf("\nCharSetview!\n");
		printf("utf8Char[0]: %x\n", utf8Char[0]);
		printf("utf8Char[1]: %x\n", utf8Char[1]);
		printf("utf8Char[2]: %x\n", utf8Char[2]);
	}*/
}

BRect CharSetView::GetCharRect(uint16 cpos)
{
	cpos %= numcols * numrows;
	BRect rect = BRect(floor(cpos % numcols) * colwidth,
					   floor(cpos / numcols) * rowheight,
					  (floor(cpos % numcols) + 1) * colwidth,
					  (floor(cpos / numcols) + 1) * rowheight);
	rect.OffsetBy(pad, pad);
	return rect;
}

void CharSetView::MoveCharPos(int16 val)
{
	SetCharPos(charpos + val);
}

void CharSetView::SetCharPos(uint16 unicode)
{
	uint16 old_charpos = charpos;
	charpos = unicode;

	if (old_charpos / (numcols * numrows) != (charpos) / (numcols * numrows)) {
		Invalidate();
	}
	else {
		Invalidate(GetCharRect(old_charpos));
		Invalidate(GetCharRect(charpos));
	}
	SetValue(charpos);
	BMessage copy(M_NEWCHAR);
	copy.AddInt16("unicode", (charpos));
	Invoke(&copy);
}

int32 CharSetView::GetCharPos()
{
	return charpos;
}

void CharSetView::MessageReceived(BMessage *msg)
{
	float	deltay;

	switch(msg->what)
	{
		case B_MOUSE_WHEEL_CHANGED:
			msg->FindFloat("be:wheel_delta_y", &deltay);
			MoveCharPos(256 * int(deltay));
			break;

		case M_PREV_PAGE:
			// move to previous page in UTF8 Format
			MoveCharPos(-256);
			break;

		case M_NEXT_PAGE:
			// move to next page in UTF8 Format
			MoveCharPos(256);
			break;

		case M_PROPERTIESCHARPOS:
			break;

		default:
			BView::MessageReceived(msg);
	}
}

void CharSetView::MouseDown(BPoint point)
{
#ifdef FFont_Support
	FFont			font;
#endif
	BMessage		*dragmsg;
	BPoint			dragpoint;
	BRect			rect, dragrect;
	char			Char[3];
	uint32			dragbuttons = 0;
	int32 			clicks = 0;
	int32			buttons = 0;

	MakeFocus(true);
	Window()->CurrentMessage()->FindInt32("clicks", &clicks);
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);

	rect = Bounds();
	rect.InsetBy(pad, pad);
	
	if (!rect.Contains(point))
		return;

	int32 cxpos = int32((point.x - pad) / colwidth);
	int32 cypos = int32((point.y - pad) / rowheight);
	int32 pagepos = int32(charpos / (numcols * numrows) * (numcols * numrows));
	uint16 cpos = ((cypos * numcols) + cxpos) + pagepos;
	if (cpos != charpos) {
		SetCharPos(cpos);
	}

	dragrect.Set(point.x, point.y, point.x, point.y);
	dragrect.InsetBy(-3, -3);
	GetMouse(&dragpoint, &dragbuttons, true);
	while (dragbuttons)
	{
		snooze(20 * 1000);
		GetMouse(&dragpoint, &dragbuttons, true);
		if (!dragrect.Contains(dragpoint))
		{

#ifndef FFont_Support
			dragmsg = new BMessage('Font');
#else
			dragmsg = new BMessage('!FNT');
			font.SetFamilyAndStyle(family, style);
			font.SetSize(12.0);
			if(AddMessageFont(dragmsg, "font", &font) != B_NO_ERROR)
			{
				printf("Error while adding FFont class to Drag & Drop Message!\n");
			}
#endif
			Char[0] = char(((cpos & 0xf000) >> 12) | 0xe0);
			Char[1] = char(((cpos & 0xfc0) >> 6) | 0x80);
			Char[2] = char((cpos & 0x3f) | 0x80);
			Char[3] = 0;
			dragmsg->AddData("text/plain", B_MIME_TYPE, &Char, sizeof(Char));
			DragMessage(dragmsg, GetCharRect(charpos));
			break;
		}
	}
}

void CharSetView::KeyDown(const char *bytes, int32 numBytes)
{
	bool isValid = true;

	if (numBytes < 1 || numBytes > 4)
		isValid = false;
	else if (numBytes == 1)
		switch (bytes[0]) {
			case B_LEFT_ARROW:
				// move to previous char in UTF8 Format
				MoveCharPos(-1);
				break;

			case B_UP_ARROW:
				MoveCharPos(-numcols);
				break;

			case B_RIGHT_ARROW:
				// move to next char in UTF8 Format
				MoveCharPos(1);
				break;

			case B_DOWN_ARROW:
				MoveCharPos(numcols);
				break;

			case B_PAGE_UP:
				// move to previous page in UTF8 Format
				MoveCharPos(-256);
				break;

			case B_PAGE_DOWN:
				// move to next page in UTF8 Format
				MoveCharPos(256);
				break;
			
			case B_HOME:
				// move to index 0
				if (charpos != 0)
					SetCharPos(0);
				break;

			default:
				// 1-byte UTF-8 encoding, code point range: 0 thru 0x7F
				if (bytes[0] < ' ')	// skip non-printing characters, too
					isValid = false;
				else if (bytes[0] != charpos)
					SetCharPos(bytes[0]);	// same as ASCII
		}
	else {	// multibyte UTF-8 encoding
		uint8 numBits = numBytes + 1;	// number of prefix bits in first byte
		uint8 mask = (1 << numBits) - 1;	// 111, 1111, or 11111

		// prefix bits of first byte must be 110, 1110, or 11110
		isValid = ((unsigned) bytes[0] >> 8 - numBits & mask) == mask - 1;

		// prefix bits of subsequent bytes must be 10
		for (int i = 1; isValid && i < numBytes; i++)
			if ((bytes[i] & 0xC0) != 0x80)
				isValid = false;

		if (isValid) {
			uint32 utf32 = BUnicodeChar::FromUTF8(&bytes);

			if (utf32 != charpos)
				SetCharPos(utf32);
		}
	}

	if (!isValid)
		BView::KeyDown(bytes, numBytes);
}

int32 CharSetView::GetPageStartPos()
{
	return charpos / (numcols * numrows) * (numcols * numrows);
}

int32 CharSetView::GetPageEndPos()
{
	return (charpos / (numcols * numrows) + 1) * (numcols * numrows) - 1;
}

void CharSetView::SetValue(int32 value)
{
	if (value != Value())
	{
		charpos = value;
	}
}

int32 CharSetView::Value()
{
	return charpos;
}

void CharSetView::SetBGColor(rgb_color col)
{
	bgColor = col;
}

void CharSetView::SetTextColor(rgb_color col)
{
	textColor = col;
}

void CharSetView::SetStrokeColor(rgb_color col)
{
	strokeColor = col;
}

void CharSetView::SetSelectedBGColor(rgb_color col)
{
	selectedBgColor = col;
}

void CharSetView::SetSelectedTextColor(rgb_color col)
{
	selectedTextColor = col;
}

void CharSetView::SetInactiveSelectedBGColor(rgb_color col)
{
	inactiveSelectedBgColor = col;
}

void CharSetView::SetInactiveSelectedTextColor(rgb_color col)
{
	inactiveSelectedTextColor = col;
}

void CharSetView::SetPadding(int val)
{
	pad = val;
}

void CharSetView::SetDrawMode(bool mode)
{
	drawmode = mode;
}

void CharSetView::SetFamilyAndStyle(const font_family family, const font_style style)
{
	strcpy(this->family, family);
	strcpy(this->style, style);
	font.SetFamilyAndStyle(family, style);
}
