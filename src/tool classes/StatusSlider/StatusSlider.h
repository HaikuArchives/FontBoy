#include <Slider.h>
#include <Message.h>
#include <String.h>
#include <Rect.h>
#include <stdio.h>

class StatusSlider : public BSlider
{
public:
					StatusSlider(BRect frame,
								const char *name,
								const char *label,
								BMessage *msg,
								int32 minValue,
								int32 maxValue,
								thumb_style thumbType = B_BLOCK_THUMB,
								uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
								uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW |B_NAVIGABLE);
	virtual 		~StatusSlider(void);
	virtual char	*UpdateText(void) const;
			void	SetUpdateText(char *utext);

private:
			char	*tmpUpdTxt;
			BString	UpdText;
};

