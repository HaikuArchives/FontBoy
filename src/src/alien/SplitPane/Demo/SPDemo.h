#ifndef _SPDemo_H
#define _SPDemo_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "SPDemoWindow.h"

extern const char *APP_SIGNATURE;

class SPDemo : public BApplication {
   public:
      SPDemo();
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:   
      SPDemoWindow *theWin;
};
#endif
