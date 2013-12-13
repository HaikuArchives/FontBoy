#ifndef _SPDemo_WINDOW_H
#define _SPDemo_WINDOW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "SplitPane.h"

class SPDemoWindow : public BWindow {
   public:
      SPDemoWindow(BRect);
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:
      int StringToInt(const char *);
      SplitPane *SP;
      
      BTextControl *minOne;
      BTextControl *minTwo;
      BTextControl *Thick;
      
      BCheckBox *LockA;
      BCheckBox *LockP;
      
};
#endif