#ifndef _COLOR_BUTTON_H_
#define _COLOR_BUTTON_H_

#include <View.h>
#include <Box.h>
#include <Picture.h>
#include <ColorControl.h>

const rgb_color White =		{ 255, 255, 255, 255 };

// ColorButton
class PaletteWindow;
class ColorButton : public BView
{
	public:
						ColorButton(BRect frame, const char* name, const char* label, uint32 resize_mask, uint32 flags, const rgb_color *color, uint32 command);
						~ColorButton(void);

		virtual void	Draw(BRect update_rect);
		virtual void	FrameResized(float width, float height);
		virtual void	AttachedToWindow();
		virtual void	MouseDown(BPoint point);
		virtual void	MessageReceived(BMessage *msg);
		void			SetValue(rgb_color color);
		void			SetValue(int32 color);
		rgb_color		ValueAsColor() const;
		void			SetDivider(float xCoordinate);
		float			Divider(void) const;
		BWindow			*GetAppWindow(char *name);

	private:
		char			*m_label;
		float			m_divider;
		BRect			m_cached_bounds;
		rgb_color		m_background_color;
		rgb_color		m_dark_1_color;
		rgb_color		m_dark_2_color;
		uint32			m_command;
};

#endif //_COLOR_BUTTON_H_
