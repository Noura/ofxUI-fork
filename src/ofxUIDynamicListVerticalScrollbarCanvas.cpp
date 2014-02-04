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

#include "ofxUIDynamicListVerticalScrollbarCanvas.h"

// TODO make these settable
#define OFX_UI_SCROLLBAR_W 15
#define OFX_UI_SCROLLBAR_H_MIN 25

ofxUIDynamicListVerticalScrollbarCanvas::~ofxUIDynamicListVerticalScrollbarCanvas() {
    delete scrollbar;
    delete scrollbarTrack;
    for (list<ofxUIWidget*>::iterator it = listItems.begin(); it != listItems.end(); it++) {
        delete (*it);
    }
}

ofxUIDynamicListVerticalScrollbarCanvas::ofxUIDynamicListVerticalScrollbarCanvas(float x, float y, float w, float h, ofxUICanvas * sharedResources)
: ofxUIScrollableCanvas(x, y, w, h, sharedResources)
, init_x(x)
, init_y(y)
, init_w(w)
, init_h(h)
, listPadding(5.0) //TODO make this settable
, scrollbar(NULL)
, scrollbarTrack(NULL) {
    scrollbar_w = OFX_UI_SCROLLBAR_W; 
    scrollbar_h = OFX_UI_SCROLLBAR_H_MIN;
    // TODO is the kind member variable still used by ofxUI?
    kind = OFX_UI_WIDGET_DYNAMICLISTVERTICALSCROLLBARCANVAS;

    scrollbarTrack = new ofRectangle(x + w - scrollbar_w, y, scrollbar_w, h);
    scrollbar = new ofxUIDraggableRect(x + w - scrollbar_w, y, scrollbar_w, scrollbar_h, *scrollbarTrack); //TODO should scrollbarTrack be passed in as a pointed instead of dereferencing like this?

    setContentHeight(h);
}

// TODO rename to addWidget
// TODO shadow all other addWidget like functions so they don't do anything
ofxUIWidget* ofxUIDynamicListVerticalScrollbarCanvas::addWidgetToList(ofxUIWidget * widget, bool reflow) {
    listItems.push_back(widget);
    addWidget(widget);

    // TODO double check that this is still how parents work in ofxUI
    widget->setParent(this);
    widget->setRectParent(this->rect);
    if (reflow) reflowWidgets();
}

// TODO rename to removeWidget
ofxUIWidget* ofxUIDynamicListVerticalScrollbarCanvas::removeWidgetFromList(list<ofxUIWidget*>::iterator it, bool reflow) {
    ofxUIWidget * w = *it;
    listItems.erase(it);
    
    removeWidget(w);
    if (reflow) reflowWidgets();
}

// TODO rename to getWidgets
list<ofxUIWidget*>* ofxUIDynamicListVerticalScrollbarCanvas::getWidgetList() {
    return &listItems;
}

void ofxUIDynamicListVerticalScrollbarCanvas::reflowWidgets() {
    float y = listPadding;
    for (list<ofxUIWidget*>::iterator it = listItems.begin(); it != listItems.end(); it++) {
        ofxUIWidget *w = (*it);
        w->getRect()->y = y;
        float h = MAX(w->getRect()->height, w->getPaddingRect()->height);
        y += h + listPadding;
    }
    setContentHeight(y);
}

void ofxUIDynamicListVerticalScrollbarCanvas::setContentHeight(float _contentHeight) {
    contentHeight = _contentHeight;
    if (_contentHeight <= init_h) {
        scrollbar_h = init_h;
    } else {
        scrollbar_h = CLAMP(init_h * init_h / contentHeight, OFX_UI_SCROLLBAR_H_MIN, contentHeight);
    }
    scrollbar->setHeight(scrollbar_h);
    scrollTop = init_y + scrollbar->height/2;
    scrollBottom = init_y + init_h - scrollbar->height/2;
}

float ofxUIDynamicListVerticalScrollbarCanvas::getContentHeight() {
    return contentHeight;
}

ofRectangle ofxUIDynamicListVerticalScrollbarCanvas::getAvailableSpace() {
    return ofRectangle(rect->x, rect->y, rect->width - scrollbar_w, rect->height);
}

void ofxUIDynamicListVerticalScrollbarCanvas::setScrollbarImage(string imagePath) {
    scrollbar->setImage(imagePath);
}

void ofxUIDynamicListVerticalScrollbarCanvas::draw() {
    ofxUIPushStyle();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofxUISetRectMode(OFX_UI_RECTMODE_CORNER);
    ofSetLineWidth(1.0);

    drawPadded();
    drawPaddedOutline();
    drawBack();
    drawFill();
    drawFillHighlight();
    drawOutline();
    drawOutlineHighlight();

    for(list<ofxUIWidget*>::iterator it = listItems.begin(); it != listItems.end(); it++) {
        if ((*it)->isVisible() && (*it)->getRect()->rInside(*sRect)) {
            ofxUIWidget * w = (*it);
            w->draw();
            if (w->hasLabel() &&
                w->getKind() != OFX_UI_WIDGET_TEXTAREA) {
                ((ofxUIWidgetWithLabel*)w)->getLabelWidget()->draw();
                //TODO wtf? why not just draw all widgets?
            }
            // TODO handle embedded widgets? what are embedded widgets?
        }
    }
    scrollbar->draw();

    ofxUIPopStyle();
}

void ofxUIDynamicListVerticalScrollbarCanvas::mousePressed(int x, int y, int button) {
    if (!sRect->inside(x, y)) {
        return;
    }
    if (!scrollbarTrack->inside(x, y)) {
        ofxUIScrollableCanvas::mousePressed(x, y, button);
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::mouseReleased(int x, int y, int button) {
    if (!sRect->inside(x, y)) {
        return;
    }
    if (!scrollbarTrack->inside(x, y)) {
        ofxUIScrollableCanvas::mouseReleased(x, y, button);
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::mouseDragged(int x, int y, int button) {
    if (scrollbar->isHit() && scrollBottom != scrollTop) {
        float scrollPercent = (scrollbar->getCenterY() - scrollTop) / (scrollBottom - scrollTop);
        scrollPercent = CLAMP(scrollPercent, 0.0, 1.0);
        rect->y = init_y - scrollPercent * (contentHeight - init_h);
    } else {
        ofxUIScrollableCanvas::mouseDragged(x, y, button);
        float scrollPercent = (init_y - rect->y) / (contentHeight - init_h);
        scrollPercent = CLAMP(scrollPercent, 0.0, 1.0);
        float scrollbarY = init_y + scrollPercent * (init_h - scrollbar->height);
        scrollbar->y = scrollbarY;
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::scrollToBottom() {
    rect->y = init_y - (contentHeight - init_h);
    scrollbar->y = init_y + (init_h - scrollbar->height);
}

void ofxUIDynamicListVerticalScrollbarCanvas::sortWidgets(bool (*f)(const ofxUIWidget *, const ofxUIWidget *)) {
    listItems.sort(f);
    reflowWidgets();
}

void ofxUIDynamicListVerticalScrollbarCanvas::update() {
    ofxUIScrollableCanvas::update();
}
