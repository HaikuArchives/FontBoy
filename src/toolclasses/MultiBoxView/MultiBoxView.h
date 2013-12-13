/* MultiBoxView.h */

#ifndef	MULTIBOXVIEW_H
#define	MULTIBOXVIEW_H

#include <stdio.h>
#include <View.h>
#include <Bitmap.h>
#include <Message.h>
#include <String.h>

class MultiBoxView : public BView
{
public:				MultiBoxView(BRect frame, float ncols = 3, float rheight = 30);
	virtual void	Draw(BRect urect);
	virtual void	DrawContentBox(int32 element);
	virtual void	MessageReceived(BMessage *message);
	virtual void	FrameResized(float aWidth, float aHeight);
	virtual void	AllAttached(void);
			void	UpdateProperties();
			int32	GetElementAt(BPoint point);
			int32	GetElementAt(int32 row, int32 column);
			void	GetRectAt(BPoint point, BRect *rect);
			void	GetRectAt(int32 listpos, BRect *rect);

			float	GetNumCols();
			void	SetNumCols(float fval);
			float	GetNumRows();
			float	GetColWidth();
			float	GetMinColWidth();
			void	SetMinColWidth(float fval);
			float	GetRowHeight();
			void	SetRowHeight(float fval);
			void	SetAutoColumns(int32 ival);

private:
	int32			autocolumns;
	int32			numelements;
	float			numrows,					// calculated numrows
					numcols,					// Numer of columns
					rowheight,					// rowheight set by user
					colwidth,					// calculated colwidth
					mincolwidth;				// minimum colwidth
};

#endif	// MULTIBOXVIEW_H