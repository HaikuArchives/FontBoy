/*
 * Copyright 2001-2008, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm, bpmagic@columbus.rr.com
 *		Rene Gollent, rene@gollent.com
 *		Ryan Leavengood, leavengood@gmail.com
 *		John Scipione, jscipione@gmail.com
 */
#ifndef COLOR_ITEM_H
#define COLOR_ITEM_H


#include <InterfaceDefs.h>
#include <ListItem.h>
#include <View.h>

enum color_type {LIST_WINDOW_COLOR, DETAILS_WINDOW_COLOR};


class ColorItem : public BStringItem
{
public:
							ColorItem(const char* text, color_type which,
								rgb_color color);

	virtual void			DrawItem(BView* owner, BRect frame, bool complete);
			color_type		ColorType(void);
			void			SetColor(rgb_color color);
			rgb_color		Color() {return fColor;};
private:
			color_type		fColorType;
			rgb_color		fColor;
};


#endif
