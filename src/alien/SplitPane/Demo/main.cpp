/*******************************************************
*   SPDemo©
*
*   @author  YNOP (ynop@acm.org)
*   @version none
*   @date    Jan 1 2000
*******************************************************/
#include <AppKit.h>

#include "SPDemo.h"

// Application's signature
const char *APP_SIGNATURE = "application/x-vnd.Abstract-SPDemo";

/*******************************************************
*   Main app. So launch off that SPDemo and lets
*   get down to buizness
*******************************************************/
int main(int argc, char* argv[]){
   SPDemo *app = new SPDemo();

   app->Run();
   delete app;
   return B_NO_ERROR;
}