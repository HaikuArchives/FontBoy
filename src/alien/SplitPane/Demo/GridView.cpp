/*******************************************************
*   SPDemo©
*
*   @author  YNOP (ynop@acm.org)
*   @version none
*   @date    Jan 1 2000
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <TranslationKit.h>
#include <TranslationUtils.h>

#include <stdio.h>

#include "GridView.h"

/*******************************************************
*   
*******************************************************/
GridView::GridView(BRect frame,rgb_color c):BView(frame, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS){//B_PULSE_NEEDED|B_FRAME_EVENTS
   SetViewColor(216,216,216,0);
   GridColor = c;   
}


/*******************************************************
*   
*******************************************************/
void GridView::AllAttached(){
   SetViewColor(Parent()->ViewColor());
}

/*******************************************************
*   
*******************************************************/
void GridView::FrameResized(float,float){
   Invalidate();
}

/*******************************************************
*   
*******************************************************/
void GridView::Draw(BRect){
   BRect b = Bounds();
   int NumLines = 10;
   
   int Jump = int(b.right / NumLines);
   if(Jump < 1){ Jump = 1; }
   
   BeginLineArray(2*(NumLines+1));
   for(int i = 0; i < b.right; i+=Jump){  // drawing | lines
      AddLine(BPoint(i,0),BPoint(i,b.bottom),GridColor);
   }
   Jump = int(b.bottom / NumLines);
   if(Jump < 1){ Jump = 1; }
   for(int i = 0; i < b.bottom; i+=Jump){  // drawing -- lines
      AddLine(BPoint(0,i),BPoint(b.right,i),GridColor);
   }
   EndLineArray();
   
}


/*******************************************************
*   
*******************************************************/
void GridView::MessageReceived(BMessage *msg){

   switch(msg->what){
    
   default:
      BView::MessageReceived(msg);
   }
}






