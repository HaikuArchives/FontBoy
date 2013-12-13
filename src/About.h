// Fontboy About Window

#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Message.h>
#include <Screen.h>
#include <Resources.h>
#include <TranslationUtils.h>


class AboutWindow : public BWindow {

	public:				
						AboutWindow(int32);
						~AboutWindow(void);
		virtual void	MessageReceived(BMessage*);
		virtual bool	QuitRequested(void);
				void	Center(void);

	private:

};

class AboutView : public BView {
	
	public:
						AboutView(BRect);
		virtual void	Draw(BRect);
		virtual void	MouseDown(BPoint point);
		virtual void	Pulse(void);
	
	private:
		int32			PulseCnt;

};
#endif
