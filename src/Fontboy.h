#ifndef FONTBOYAPP_H
#define FONTBOYAPP_H

#include <Application.h>
#include <View.h>
#include <String.h>
#include <List.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <stdio.h>

#define APP_SIG "application/x-vnd.madison.fontboy"
#define VERSION "0.9.7"

class TPreferences;
class PrefsWindow;
class PropWindow;
class ListWindow;
class FontList;
class Prefs;

class Fontboy: public BApplication {
	public:					
						Fontboy();
						~Fontboy(void);
		virtual void	MessageReceived(BMessage *message);
		virtual bool	QuitRequested(void);
				bool	OpenNewPropWindow(BRect rect);
				void	ShowSplashScreen(bool);
			FontList	*GetFontList(void);
				void	ApplySettings(int);
				void	RefreshFontList(void);

				Prefs	*prefs;

	private:
		ListWindow		*mainwindow;
		PrefsWindow		*prefwindow;
		BList			*proplist;
		FontList		*fontlist;
};

#endif /* FONTBOYAPP_H */
