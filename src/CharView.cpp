#include "CharView.h"
#include <Debug.h>
#include <string.h>

CharView::CharView(BRect frame, font_family fam, font_style sty)
:BView(frame, "CharSetView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS),
	drawmode(false)
{
	strcpy(family, fam);
	strcpy(style, sty);
	font.SetFamilyAndStyle(family, style);
}

CharView::~CharView(void)
{
}

void CharView::SetChar(uint16 c)
{
	mutf = c;
}

void CharView::FrameResized(float width, float height)
{
	Draw(Bounds());
}

void CharView::Draw(BRect urect)
{
	BPoint		point;
	font_height	fheight;
	char		utf8Char[3];
	uint16		uniChar[1];

	urect = Bounds();

//	SetLowColor(def_viewcolor);
	SetDrawingMode(B_OP_COPY);
	SetHighColor(strokeColor);
	SetPenSize(1);
	StrokeRect(urect);

	urect.InsetBy(1, 1);
	SetHighColor(bgColor);
	FillRect(urect);
	SetLowColor(bgColor);
	SetHighColor(displayColor);

	if (drawmode) {
		font.SetSize(urect.Width() * 0.8);
		font.GetHeight(&fheight);

		//  Unicode to UTF8 Character encoding
		uniChar[0] = B_HOST_TO_BENDIAN_INT16(((mutf == 0) || (mutf == 65535)) ? 1 : mutf);
		int32 state = 0;
		int32 srcLen = 2;
		int32 destLen = sizeof(utf8Char);
		convert_to_utf8(B_UNICODE_CONVERSION, (const char*)uniChar, &srcLen,
						utf8Char, &destLen, &state);

		SetFont(&font);
	
		bool hasGlyph[1];
		font.GetHasGlyphs(utf8Char, 1, hasGlyph);

		if (hasGlyph[0]) {
			float choffset = (urect.right - urect.left - StringWidth(utf8Char, destLen)) / 2;
			float cvoffset = (urect.Height() - fheight.ascent - fheight.descent) / 2 + fheight.ascent;
			point.Set(choffset, cvoffset);
			DrawString(utf8Char, destLen, point);
		}
	}

/*	printf("\nCharView!\n");
	printf("utf8Char[0]: %x\n", utf8Char[0]);
	printf("utf8Char[1]: %x\n", utf8Char[1]);
	printf("utf8Char[2]: %x\n", utf8Char[2]);*/
}

void CharView::SetBGColor(rgb_color col)
{
	bgColor = col;
}

void CharView::SetDisplayColor(rgb_color col)
{
	displayColor = col;
}

void CharView::SetStrokeColor(rgb_color col)
{
	strokeColor = col;
}

void CharView::SetDrawMode(bool mode)
{
	drawmode = mode;
}

void CharView::SetFamilyAndStyle(const font_family family, const font_style style)
{
	strcpy(this->family, family);
	strcpy(this->style, style);
	font.SetFamilyAndStyle(family, style);
}
