/*******************************************************
*   SPDemo©
*
*   @author  YNOP (ynop@acm.org)
*   @version none
*   @date    Jan 1 2000
*******************************************************/
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

//#include <stdio.h>

#include "SPDemoWindow.h"
#include "SplitPane.h"
#include "GridView.h"

/*******************************************************
*   
*******************************************************/
SPDemoWindow::SPDemoWindow(BRect frame) : BWindow(frame,"SplitPane Demo",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS){//B_NOT_ANCHORED_ON_ACTIVATE|
   BRect b = Bounds();
   rgb_color red = { 255,0,0 };
   rgb_color blue = { 0,0,255 };
   int split = 35;
      
   BView *View = new BView(b,"MainView",B_FOLLOW_ALL_SIDES,0);
   View->SetViewColor(216,216,216);
   
   b.bottom -= split;
   
   GridView *gv1 = new GridView(b,red);
   GridView *gv2 = new GridView(b,blue);

   SP = new SplitPane(b,gv1,gv2,B_FOLLOW_ALL);
   View->AddChild(SP);

   SP->SetMinSizeOne(100); // View one will be no smaller than 100
   SP->SetBarThickness(10); // Make the toggle bar 10 pixels thick
   
   
   //------- Add controls and other stuff to the view ---------//
   b = Bounds();
   b.top = b.bottom - split;
   
   BBox *Bb = new BBox(b,"",B_FOLLOW_BOTTOM|B_FOLLOW_LEFT_RIGHT); 
   View->AddChild(Bb);
   
   minOne = new BTextControl(BRect(10,10,160,20),"","Min Size of View One","100",new BMessage('minO'),B_FOLLOW_BOTTOM);
   minOne->SetDivider(110);
   Bb->AddChild(minOne);
   
   minTwo = new BTextControl(BRect(170,10,320,20),"","Min Size of View Two","0",new BMessage('minT'),B_FOLLOW_BOTTOM);
   minTwo->SetDivider(110);
   Bb->AddChild(minTwo);
   
   Thick = new BTextControl(BRect(330,10,430,20),"","Bar Thickness","10",new BMessage('thik'),B_FOLLOW_BOTTOM);
   Thick->SetDivider(75);
   Bb->AddChild(Thick);
   
   LockA = new BCheckBox(BRect(440,10,510,20),"","Lock Align",new BMessage('lokA'),B_FOLLOW_BOTTOM);
   Bb->AddChild(LockA); 
    
   LockP = new BCheckBox(BRect(520,10,585,20),"","Lock Pos",new BMessage('lokP'),B_FOLLOW_BOTTOM);
   Bb->AddChild(LockP);
   
   AddChild(View);
   Show();
}


/*******************************************************
*  
*******************************************************/
void SPDemoWindow::MessageReceived(BMessage* msg){
   switch(msg->what){
   case 'minO':
      SP->SetMinSizeOne(StringToInt(minOne->Text()));
      break;
   case 'minT':
      SP->SetMinSizeTwo(StringToInt(minTwo->Text()));
      break;
   case 'thik':
      SP->SetBarThickness(StringToInt(Thick->Text()));
      break;
   case 'lokA':
      SP->SetBarAlignmentLocked(LockA->Value());
      break;
   case 'lokP':
      SP->SetBarLocked(LockP->Value());
      break;
   default:
      BWindow::MessageReceived(msg);
   }
}


/*******************************************************
*   Quick and dirty func to change a string of chars 
*   into a int. Method by Chris Camp (cmc008@coes.latech.edu)
*******************************************************/
int SPDemoWindow::StringToInt(const char *temp){
   int i = 0;
   int t = 0;
   if (strcmp(temp,"") == -1){ return 0; }
   char c = 'X';
   while(temp[i] != '\0'){
      c = temp[i];
      t = t * 10 + (c - 48); // 48 is the ASCII code for 0  
      i++;
   }
   return t;
}


/*******************************************************
*   Someone asked us nicely to quit. I gess we should
*   so clean up. save our setings (position size of win)
*   and tell the main be_app to shut it all down .. bye
*******************************************************/
bool SPDemoWindow::QuitRequested(){
   be_app->Lock();
   be_app->Quit();
   be_app->Unlock();
   return true;
}



