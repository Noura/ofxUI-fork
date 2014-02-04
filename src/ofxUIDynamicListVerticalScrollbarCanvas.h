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
 * To add widgets you should only use addWidgetToList
 * To remove widgets you should only use removeWidgetFromList
 * You must tell an instance the height of its content using setContentHeight
 * TODO can I remove that last restriction?
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#ifndef OFXUI_SCROLLBAR_CANVAS
#define OFXUI_SCROLLBAR_CANVAS

#include "ofxUIScrollableCanvas.h"
#include "ofxUIDraggableRect.h"

class ofxUIDynamicListVerticalScrollbarCanvas : public ofxUIScrollableCanvas
{
public:
    ~ofxUIDynamicListVerticalScrollbarCanvas();
    ofxUIDynamicListVerticalScrollbarCanvas(float x, float y, float w, float h);
    ofxUIWidget* addWidgetToList(ofxUIWidget * widget, bool reflow = true);
    ofxUIWidget* removeWidgetFromList(list<ofxUIWidget*>::iterator it, bool reflow = true);
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
