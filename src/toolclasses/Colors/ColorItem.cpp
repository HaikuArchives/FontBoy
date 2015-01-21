/*
 * Copyright 2002-2013 Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm, darkwyrm@earthlink.net
 *		Rene Gollent, rene@gollent.com
 *		Ryan Leavengood, leavengood@gmail.com
 */


#include "ColorItem.h"

#include <stdio.h>


ColorItem::ColorItem(const char* text, color_type which,
		rgb_color color)
	:
	BStringItem(text, 0, false),
	fColorType(which),
	fColor(color)
{
}


void
ColorItem::DrawItem(BView* owner, BRect frame, bool complete)
{
	rgb_color highColor = owner->HighColor();
	rgb_color lowColor = owner->LowColor();

	if (IsSelected() || complete) {
		if (IsSelected()) {
			owner->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
			owner->SetLowColor(owner->HighColor());
		} else
			owner->SetHighColor(lowColor);

		owner->FillRect(frame);
	}

	rgb_color border = (rgb_color){ 184, 184, 184, 255 };

	BRect colorRect(frame);
	colorRect.InsetBy(2, 2);
	colorRect.right = colorRect.left + colorRect.Height();
	owner->SetHighColor(fColor);
	owner->FillRect(colorRect);
	owner->SetHighColor(border);
	owner->StrokeRect(colorRect);

	owner->MovePenTo(frame.left + colorRect.Width() + 8, frame.top
		+ BaselineOffset());

	if (!IsEnabled()) {
		rgb_color textColor = ui_color(B_LIST_ITEM_TEXT_COLOR);
		if (textColor.red + textColor.green + textColor.blue > 128 * 3)
			owner->SetHighColor(tint_color(textColor, B_DARKEN_2_TINT));
		else
			owner->SetHighColor(tint_color(textColor, B_LIGHTEN_2_TINT));
	} else {
		if (IsSelected())
			owner->SetHighColor(ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR));
		else
			owner->SetHighColor(ui_color(B_LIST_ITEM_TEXT_COLOR));
	}

	owner->DrawString(Text());

	owner->SetHighColor(highColor);
	owner->SetLowColor(lowColor);
}


color_type
ColorItem::ColorType(void)
{
	return fColorType;
}


void
ColorItem::SetColor(rgb_color color)
{
	fColor = color;
}
