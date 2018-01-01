/* ListView.h */

#ifndef	LISTVIEW_H
#define	LISTVIEW_H

#include <stdio.h>
#include <View.h>
#include <Window.h>
#include <Screen.h>
#include <Bitmap.h>
#include <ScrollBar.h>
#include <Font.h>
#include <String.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include "MultiBoxView.h"

#define FFont_Support

enum { dist = 4 , leftdist = 10, dragdist = 5 };

class Fontboy;
class FontList;
class ListView : public MultiBoxView {

	public:				ListView(BRect frame);
						~ListView();
		virtual void	DrawContentBox(int32 element);
				void	Init();
				void	SetRowsCols(int32 rows, int32 columns);
				void	SetElements(int32 elements);
				void	SetFrame(BRect frame);
				void	UpdateScrollBar();
				float	Height() { return fHeight; }

	protected:
		virtual void	MouseDown(BPoint point);
		virtual void	MessageReceived(BMessage *message);
		virtual void	FrameResized(float aWidth, float aHeight);
		virtual void	ScrollTo(BPoint apoint);

	private:
				void	CalcLeader();
				void	PrintRect(BRect *rect);

		Fontboy			*fontboy;
		Prefs			*prefs;
		FontList		*fontlist;
		font_height		infoheight, textheight;

		float			th_all, th_base, ih_base;
		BPoint			lastPoint;

		float			fHeight;
};

#endif	// LISTVIEW_H
