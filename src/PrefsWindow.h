// PrefsWindow.h

#ifndef PREFSWINDOW_H
#define PREFSWINDOW_H

#include <stdio.h>
#include <Window.h>
#include <View.h>
#include <TabView.h>
#include <ListView.h>
#include <OutlineListView.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Slider.h>
#include <ScrollView.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Point.h>
#include <Message.h>
#include <Messenger.h>

#include "ColorButton.h"
#include "StatusSlider.h"


enum { CB_WIDTH = 32 , CB_HEIGHT = 16 , CB_HOFFSET = 7, CB_VOFFSET = 7};

class PrefsWindow : public BWindow {

	public:				
						PrefsWindow(BRect);
						~PrefsWindow(void);
		virtual void	MessageReceived(BMessage*);
		virtual bool	QuitRequested(void);
		
		void			GetPreferences();
		void			SetPreferences();
		void			UpdatePrefs(bool);
		BWindow			*GetAppWindow(char *name);

	private:
		Fontboy			*fontboy;
		Prefs			*prefs;
		BWindow			*ccwindow;

		BView			*cprefsview;
		BOutlineListView *lView;
		BBox			*gbox, *c1box, *c2box, *dbox, *mbox, *pbox, *outbox;
		BButton			*apply, *revert, *cdefault;
		BMenu			*menu;
		BPopUpMenu		*fontSize, *numcols;
		BMenuField		*aafield, *tsfield;
		BMenuItem		*item;
		BTextControl	*text;
		BCheckBox		*drawheights, *drawborder, *splashscreen, *liveupdate;
		ColorButton		*cmbg, *cmdisplay, *cminfo, *cmstroke, *cmheights, *cmselect, *cpbg, *cpdisplay, *cpselect, *cpstroke;
		StatusSlider	*slider;
		int32			current_color, snumcols;
};

#endif /* PREFSWINDOW_H */
