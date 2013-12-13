#ifndef _GRID_VIEW_H
#define _GRID_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>


class GridView : public BView {
   public:
      GridView(BRect,rgb_color);
      virtual void AllAttached();
      virtual void Draw(BRect);
      virtual void FrameResized(float,float);
      virtual void MessageReceived(BMessage*);
   private:
      rgb_color GridColor;
};
#endif