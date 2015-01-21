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
#include <ColorControl.h>
#include <Slider.h>
#include <ScrollView.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Point.h>
#include <Message.h>
#include <Messenger.h>

#include "StatusSlider.h"
#include "ColorItem.h"
#include "ColorPreview.h"


class PrefsWindow : public BWindow {

	public:				
						PrefsWindow(BRect);
						~PrefsWindow(void);
		virtual void	MessageReceived(BMessage*);
		virtual bool	QuitRequested(void);
		
		void			GetPreferences();
		void			SetPreferences();
		void			UpdatePrefs();
		BWindow			*GetAppWindow(char *name);

	private:
		void			_SetCurrentColor(rgb_color color);
		void			_UpdateColors();
		BTabView* 		fTabView; 
		ColorPreview* 	fColorPreview;
		BColorControl* 	fPicker;
		BListView*		fAttrList;
		Fontboy			*fontboy;
		Prefs			*prefs;

		BButton			*fRevertButton, *fDefaultButton;
		BMenu			*menu;
		BPopUpMenu		*fontSize, *numcols;
		BMenuField		*aafield, *tsfield;
		BMenuItem		*item;
		BTextControl	*text;
		BCheckBox		*drawheights, *drawborder;
		ColorItem		*cmbg, *cmdisplay, *cminfo, *cmstroke, *cmheights, *cmselect, *cpbg, *cpdisplay, *cpselect, *cpstroke;
		StatusSlider	*slider;
		int32			snumcols;
};

#endif /* PREFSWINDOW_H */
