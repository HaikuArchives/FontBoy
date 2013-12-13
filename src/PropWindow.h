#include <stdio.h>
#include <Window.h>
#include <Rect.h>
#include <Font.h>
#include <TextView.h>
#include <StringView.h>
#include <Bitmap.h>
#include <Point.h>
#include <PictureButton.h>
#include <Alert.h>
#include <TranslationUtils.h>
#include <TranslatorFormats.h>
#include <String.h>
#include <string.h>

#include "BubbleHelper.h"
#include "CharSetView.h"
#include "CharView.h"
#include "SplitPane.h"

class PropView;
class TopView;
class LeftView;
class SplitView;

//
// Properties Window
class PropWindow : public BWindow
{
public:

	enum
	{
		MINIMUM_WIDTH = 100,
		MINIMUM_HEIGHT = 100
	};

					PropWindow(BRect, font_family, font_style);
					~PropWindow(void);
	virtual void	MessageReceived(BMessage*);
	virtual bool	QuitRequested(void);
	virtual void	Zoom(BPoint, float, float);
			void	SetTitle(void);
			void	GetFamilyAndStyle(font_family*, font_style*);
			void	SetFamilyAndStyle(const font_family, const font_style);
	BubbleHelper*	GetToolTipHandler(void);
			void	SelectChar(uint16);
			void	Update();
			void	LockSplitpane(bool);
			bool	IsSplitpaneLocked(void);

private:
	PropView		*pview;
	font_family		family;
	font_style		style;
};

class PropView : public BView
{
public:

	enum
	{
		BORDER_PADDING = 10,
		DARKGREY_DECOR_WIDTH = 50,
		SV_PANETWO_MIN = 300,
	};

					PropView(BRect);
					~PropView(void);
	virtual void	AttachedToWindow(void);
	virtual void	MessageReceived(BMessage*);
	virtual void	Update(void);
			int		GetSliderPos(void);
			void	SetSliderPos(int, bool);
			void	LockSliderPos(bool);
			bool	IsSliderPosLocked(void);
			void	Zoom(float, float);
			void	SetCharPos(uint16);

private:
	Fontboy			*fontboy;
	Prefs			*prefs;
	TopView			*topview;
	LeftView		*leftview;
	CharSetView		*csview;
	SplitView		*sp;
};

class SplitView : public SplitPane
{
public:
	enum
	{
		SV_MINSIZE_ONE = 160,
		SV_MINSIZE_TWO = 300
	};

					SplitView(BRect,BView*,BView*);
					~SplitView(void);
	virtual void	Update(void);
	virtual void	FrameResized(float, float);
	virtual void	MouseMoved(BPoint,uint32,const BMessage*);
//			void	Zoom(float, float);
			void	UpdatePos(float width, float height, int = 0);
			void	UpdateLimits(float = 0);
			void	UpdateLimits(float, float, float = 0);

private:
			int		move_size;
};

class TopView : public BView
{
public:

	enum
	{
		PADDING = 5,
		HEIGHT = 32 + PADDING
	};

					TopView(BRect, BView*);
					~TopView(void);
	virtual void	MessageReceived(BMessage*);
	virtual void	AttachedToWindow(void);
	virtual void	AllAttached(void);
	virtual void	FrameResized(float, float);
	virtual void	Update(uint16 = 0xffff);
	virtual void	Draw(BRect urect);
		BPicture	*GetPictureFromBitmap(const char*);

private:
	BubbleHelper	*bhelper;
	CharView		*cview;

	BView			*targetview;
	BStringView		*currascii,	*rangeascii,
					*currascii_value, *rangeascii_value;
	BBitmap			*ttf_icon, *ps_icon;
	BPictureButton	*npage, *ppage;
	uint16			old_pagepos;
};

class LeftView : public BView
{
public:

	enum
	{
		DEF_WIDTH = 180,
		FONTINFO_PADDING = 10,
		FONTINFO_LINE_HEIGHT = 16,
	};

					LeftView(BRect);
					~LeftView(void);
	virtual void	MessageReceived(BMessage*);
	virtual void	AttachedToWindow(void);
	virtual void	FrameResized(float, float);
	virtual void	MouseDown(BPoint);
	virtual void	Draw(BRect);
	virtual void	Update(void);
			void	DrawTruncString(BRect, const char*);

private:
	Fontboy			*fontboy;
	Prefs			*prefs;
	CharView		*cview;
	BBitmap			*schubknopf;
	float			bottom_bounds;
};
