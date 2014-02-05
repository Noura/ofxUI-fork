/*
 * ofxUIDynamicListVerticalScrollbarCanvas.h
 *
 * Designed for vertically scrolling lists of dynamic content. Add and remove widgets and have their positions automatically laid out in a vertical list on the canvas. There is a scrollbar that automatically resizes itself based on the height of the content.
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
    ofxUIDraggableRect* getScrollbar();
    void setScrollbarWidth(float w);
    void setScrollbarMinHeight(float h);
    void showScrollbar();
    void hideScrollbar();
    
    void setListPadding(float padding);
    
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
    
    static const float scrollbarDefaultWidth;
    static const float scrollbarDefaultMinHeight;
    
protected:
    float listPadding;
    float scrollbar_h, scrollbar_h_min;
    float contentHeight;
    float scrollTop, scrollBottom;
    ofxUIDraggableRect * scrollbar;
    bool show_scrollbar;
    std::list<ofxUIWidget*> listItems;
};

#endif
