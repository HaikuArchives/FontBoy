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
#include <Alert.h>

//#include <stdio.h>

#include "SPDemo.h"
#include "SPDemoWindow.h"

/*******************************************************
*
*******************************************************/
SPDemo::SPDemo() : BApplication(APP_SIGNATURE){
   BRect defaultSize(50,50,650,350);
   theWin = new SPDemoWindow(defaultSize);

}

/*******************************************************
*
*******************************************************/
void SPDemo::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BApplication::MessageReceived(msg);
      break; 
   }  
}

/*******************************************************
*
*******************************************************/
bool SPDemo::QuitRequested(){
   theWin->Lock();
   if(theWin->QuitRequested()){
      theWin->Unlock(); 
      return true; // Ok .. fine .. leave then
   }else{
      theWin->Unlock();
      return false;
   }
}




