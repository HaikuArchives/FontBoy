#include <Window.h>
#include <ColorControl.h>
#include <Box.h>
#include <Looper.h>
#include <Screen.h>

enum {
	M_PALETTE			= 'mPlC',
	M_PALETTE_QUIT		= 'mPlQ',
};

class PaletteWindow : public BWindow
{
public:				
					PaletteWindow(BPoint point, BHandler *target_i);
	virtual bool	QuitRequested(void);
			void	SetColor(rgb_color color);

private:
	BColorControl	*color_ctrl;
	BHandler		*target;
};

/* This view is used to work around a bug in the current ColorControl,
   that doesn't allow live feedback when changing the color. */
class CControl : public BColorControl
{
	public:
						CControl(BPoint start, BMessage *message);
		virtual	void	SetValue(int32 color_value);
};
