#include "CharList.h"

#include <Window.h>
#include <Control.h>
#include <String.h>
#include <Font.h>
#include <Message.h>
#include <string.h>
#include <UTF8.h>

//#define FFont_Support

#ifdef FFont_Support
#include "FFont.h"
#endif

enum
{
	numcols = 16,
	numrows = 16,
};

//
// Charset class
//
class CharSetView : public BControl
{
public:
					CharSetView(BRect, font_family, font_style, BMessage*);
	virtual void	Draw(BRect urect);
	virtual void	FrameResized(float, float);
	virtual void	AttachedToWindow(void);
	virtual void	MouseDown(BPoint);
	virtual void	KeyDown(const char*, int32);
	virtual void	MessageReceived(BMessage*);
	virtual void	SetValue(int32);
	virtual int32	Value();

			void	DrawCharBox(uint16, bool);
			void	SetCharPos(uint16);
			void	MoveCharPos(int16);
			int32	GetCharPos();
			int32	GetPageStartPos();
			int32	GetPageEndPos();
			void	SetBGColor(rgb_color col);
			void	SetTextColor(rgb_color col);
			void	SetSelectColor(rgb_color col);
			void	SetStrokeColor(rgb_color col);
			void	SetPadding(int);
			void	SetDrawMode(bool);
			void	SetFamilyAndStyle(const font_family, const font_style);
			BRect	GetCharRect(uint16 cpos);
			void	ValidatePagePos(void);
	struct fnode	*fontptr;

private:
	BFont		font;
	CharList	*clist;
	font_family	family;
	font_style	style;
	float		colwidth, rowheight;
	rgb_color	bgColor, textColor, selectColor, strokeColor;
	font_height	fheight;
	uint16		charpos;
	int			pad;
	bool		drawmode;
};

