#include <View.h>
//#include <Window.h>
#include <Control.h>
//#include <String.h>
//#include <Font.h>
//#include <Message.h>
//#include <string.h>
#include <UTF8.h>

class CharView : public BView
{
public:
					CharView(BRect, font_family, font_style);
					~CharView(void);
	virtual void	FrameResized(float, float);
	virtual void	Draw(BRect);
			void	SetChar(uint16);
			void	SetBGColor(rgb_color);
			void	SetDisplayColor(rgb_color);
			void	SetStrokeColor(rgb_color);
			void	SetDrawMode(bool);
			void	SetFamilyAndStyle(const font_family, const font_style);

private:
	BFont			font;
	font_family		family;
	font_style		style;

	uint16			mutf;
	rgb_color		bgColor, displayColor, strokeColor;
	bool			drawmode;
};

