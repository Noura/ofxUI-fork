/*
 * ofxUIDynamicListVerticalScrollbarCanvas.h
 *
 * Designed for vertically scrolling lists of dynamic content.
 * Add and remove widgets and have their positions automatically laid out in a
 * vertical list on the canvas.
 * A scrollbar automatically resizes itself based on the height of the content.
 *
 * For these things to happen automatically, a few restrictions are made on the
 * ofxUIScrollableCanvas:
 * To add widgets you can only use addWidgetToList
 * To remove widgets you can only use removeWidgetFromList
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#ifndef OFXUI_SCROLLBAR_CANVAS
#define OFXUI_SCROLLBAR_CANVAS

#include "ofxUIScrollableCanvas.h"
#include "ofxUIDraggableRect.h"

class ofxUIDynamicListVerticalScrollbarCanvas : private ofxUIScrollableCanvas
{
public:
    ~ofxUIDynamicListVerticalScrollbarCanvas();
    ofxUIDynamicListVerticalScrollbarCanvas(float x, float y, float w, float h, ofxUICanvas * sharedResources = NULL);

    ofxUIWidget* addWidget(ofxUIWidget * widget, bool reflow = true);
    ofxUIWidget* removeWidget(list<ofxUIWidget*>::iterator it, bool reflow = true);
    // To get the iterator needed to remove a widget, you can get a reference to the list of widgets with this method
    list<ofxUIWidget*>* getWidgetList();
    
    void reflowWidgets();
    void setContentHeight(float _contentHeight);
    float getContentHeight();
    // Returns the bounds of the area that is not obscured by the scollbar or 
    // its vertical track.
    ofRectangle getAvailableSpace();
    void setScrollbarImage(string imagePath);
    void draw();
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    // Dragging on the scrollbar will scroll using the scrollbar
    // Dragging on the canvas will scroll according to the way the underlying
    // ofxUIScrollableCanvas scrolls
    void mouseDragged(int x, int y, int button);
    void scrollToBottom();
    void sortWidgets(bool (*f)(const ofxUIWidget *, const ofxUIWidget *));
    
    void update();
    
protected:
    //TODO try to use sRect dimensions instead of init_*
    float init_x, init_y, init_w, init_h;
    float listPadding;
    float scrollbar_w, scrollbar_h;
    float contentHeight;
    float scrollTop, scrollBottom;
    ofxUIDraggableRect * scrollbar;
    ofRectangle * scrollbarTrack;
    std::list<ofxUIWidget*> listItems;
};

#endif
