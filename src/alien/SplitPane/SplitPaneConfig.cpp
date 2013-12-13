/*******************************************************
*   SplitPane©
*
*   SplitPane is a usefull UI component. It alows the 
*   use to ajust two view Horizontaly or Vertacly so
*   that they are a desired size. This type of Pane 
*   shows up most comonly in Mail/News Readers.
*
*   @author  YNOP (ynop@acm.org)
*   @version beta
*   @date    Dec. 10 1999
*******************************************************/
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "SplitPane.h"
#include "SplitPaneConfig.h"

/*******************************************************
*   Our wonderful BWindow, ya its kewl like that.
*   we dont do much here but set up the menubar and 
*   let the view take over.  We also nead some message
*   redirection and handling
*******************************************************/
SplitPaneConfig::SplitPaneConfig(SplitPane *parent) : BWindow(BRect(50,50,300,300),"SplitPane Config",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_NOT_RESIZABLE){//B_NOT_ANCHORED_ON_ACTIVATE|
 BRect r = Bounds();
 
 BView *v = new BView(r,"",B_FOLLOW_ALL_SIDES,0);
 v->SetViewColor(255,255,0);
 AddChild(v);
      
      
}



/*******************************************************
*   To make things a little nicer and more organized
*   we are gona let View be the message handler for 
*   the whole app. All messages that are ours send to
*   View for redistribution.  But we must handle our
*   own BWindow messages or else (crash)
*******************************************************/
void SplitPaneConfig::MessageReceived(BMessage* msg){
   switch(msg->what){
   default:
      BWindow::MessageReceived(msg);
   }
}

/*******************************************************
*   Someone asked us nicely to quit. I gess we should
*   so clean up. save our setings (position size of win)
*   and tell the main be_app to shut it all down .. bye
*******************************************************/
bool SplitPaneConfig::QuitRequested(){
   return true;
}
