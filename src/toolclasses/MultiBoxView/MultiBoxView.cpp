/* MultiBoxView for displaing Fonts */

#include "MultiBoxView.h"

MultiBoxView::MultiBoxView(BRect frame, float ncols, float rheight)
		 :BView(frame, "MultiBoxView",  B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS)
{
	numcols = ncols;
	rowheight = rheight;
	UpdateProperties();
}

void MultiBoxView::FrameResized(float aWidth, float aHeight)
{
	Draw(Bounds());
}

void MultiBoxView::AllAttached(void)
{
	rgb_color fLowColor;

	if(Parent() != NULL)
		fLowColor = Parent()->ViewColor();
	else
		fLowColor.red = fLowColor.green = fLowColor.blue = 255;
	
	SetLowColor(fLowColor);
	SetViewColor(B_TRANSPARENT_COLOR);

	UpdateProperties();
}

void MultiBoxView::Draw(BRect urect)
{
	UpdateProperties();

	BRect rect;
	int32 element;
	for (int32 i=0; i<numcols; i++) {
		for (int32 j=0; j<numrows; j++) {
//_sPrintf("i: %d, j: %d\n", i, j);
			element = GetElementAt(j, i);
			GetRectAt(element, &rect);
			if (urect.Intersects(rect))
			{
				DrawContentBox(element);
			}
		}
	}
	// clear bottom area of view
	rect = Bounds();
	rect.top = numrows * rowheight + 1;
	SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	FillRect(rect);
}

void MultiBoxView::DrawContentBox(int32 element)
{
	BRect rect;

	GetRectAt(element, &rect);
	SetHighColor(255, 255, 255, 255);
	FillRect(BRect(rect));
	SetHighColor(0, 0, 0, 255);
	StrokeRect(BRect(rect));
}

void MultiBoxView::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		default:
			BView::MessageReceived(msg);
			break;
	}
}

//
// UpdateProperties
// Calculates new values for:
// - Number of Rows
// - Columnwidth
void MultiBoxView::UpdateProperties()
{
	BRect rect = Bounds();

	numrows = floor(rect.bottom / rowheight);
	colwidth = rect.right / numcols;
}

int32 MultiBoxView::GetElementAt(BPoint point)
{
	float colpos = floor(point.x / colwidth);
	float rowpos = floor(point.y / rowheight);
	return int32(colpos * numrows + rowpos);
}

int32 MultiBoxView::GetElementAt(int32 row, int32 column)
{
	return (int32) (column * numrows + row);
}

void MultiBoxView::GetRectAt(BPoint point, BRect *rect)
{
	int32 x = int32(floor(point.x / colwidth));
	int32 y = int32(floor(point.y / rowheight));
	rect->Set(x * colwidth, y * rowheight, (x + 1) * colwidth, (y + 1) * rowheight);
}

void MultiBoxView::GetRectAt(int32 element, BRect *rect)
{
	int32 x = element / int32(numrows);
	int32 y = element % int32(numrows);
	rect->Set(x * colwidth, y * rowheight, (x + 1) * colwidth, (y + 1) * rowheight);
}

float MultiBoxView::GetNumCols()
{
	return numcols;
}

void MultiBoxView::SetNumCols(float fval)
{
	numcols  = fval;
}

float MultiBoxView::GetNumRows()
{
	return numrows;
}

float MultiBoxView::GetColWidth()
{
	return colwidth;
}

float MultiBoxView::GetMinColWidth()
{
	return mincolwidth;
}

void MultiBoxView::SetMinColWidth(float fval)
{
	mincolwidth  = fval;
}

float MultiBoxView::GetRowHeight()
{
	return rowheight;
}

void MultiBoxView::SetRowHeight(float fval)
{
	rowheight  = fval;
}

void MultiBoxView::SetAutoColumns(int32 ival)
{
	autocolumns  = ival;
}
