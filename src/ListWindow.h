// ListWindow.h

#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <Screen.h>
#include <Window.h>
#include <Box.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Alert.h>
#include <ScrollView.h>
#include <Rect.h>
#include <AppDefs.h>

class ListView;
class ListWindow : public BWindow {

	public:				
						ListWindow(BRect);
						~ListWindow(void);
		virtual bool	QuitRequested();
		virtual void	Show();
		virtual void	MessageReceived(BMessage*);

	private:
		ListView		*fontview;
};

#endif /* LISTWINDOW_H */
