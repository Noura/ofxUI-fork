/*
 * ofxUIDynamicListVerticalScrollbarCanvas.cpp
 *
 * Designed for vertically scrolling lists of dynamic content. Add and remove widgets and have their positions automatically laid out in a vertical list on the canvas. There is a scrollbar that automatically resizes itself based on the height of the content.
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#include "ofxUIDynamicListVerticalScrollbarCanvas.h"

#define OFX_UI_SCROLLBAR_W_DEFAULT 15
#define OFX_UI_SCROLLBAR_H_MIN_DEFAULT 25

ofxUIDynamicListVerticalScrollbarCanvas::~ofxUIDynamicListVerticalScrollbarCanvas() {
    delete scrollbar;
    listItems.clear();
}

ofxUIDynamicListVerticalScrollbarCanvas::ofxUIDynamicListVerticalScrollbarCanvas(float x, float y, float w, float h, ofxUICanvas * sharedResources)
: ofxUIScrollableCanvas(x, y, w, h, sharedResources)
, listPadding(OFX_UI_GLOBAL_WIDGET_SPACING)
, scrollbar(NULL)
, show_scrollbar(true) {
    scrollbar_h = OFX_UI_SCROLLBAR_H_MIN_DEFAULT;
    scrollbar_h_min = OFX_UI_SCROLLBAR_H_MIN_DEFAULT;

    kind = OFX_UI_WIDGET_DYNAMICLISTVERTICALSCROLLBARCANVAS;

    float scrollbar_w = OFX_UI_SCROLLBAR_W_DEFAULT;
    ofRectangle scrollbarTrack(x + w - scrollbar_w, y, scrollbar_w, h);
    scrollbar = new ofxUIDraggableRect(x + w - scrollbar_w, y, scrollbar_w, scrollbar_h, scrollbarTrack);

    setContentHeight(h);
    
    setSnapping(false);
}

ofxUIWidget* ofxUIDynamicListVerticalScrollbarCanvas::addWidget(ofxUIWidget * widget, bool reflow) {
    listItems.push_back(widget);
    widget->getRect()->setX(sRect->getX() + OFX_UI_GLOBAL_PADDING);
    ofxUICanvas::addWidget(widget);
    if (reflow) reflowWidgets();
}

ofxUIWidget* ofxUIDynamicListVerticalScrollbarCanvas::removeWidget(list<ofxUIWidget*>::iterator it, bool reflow) {
    ofxUIWidget * w = *it;
    listItems.erase(it);
    
    ofxUICanvas::removeWidget(w);
    if (reflow) reflowWidgets();
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
    float sRectH = sRect->getHeight();
    float sRectY = sRect->getY();
    if (_contentHeight <= sRectH) {
        show_scrollbar = false;
        scrollbar_h = sRectH;
    } else {
        show_scrollbar = true;
        scrollbar_h = CLAMP(sRectH * sRectH / contentHeight, scrollbar_h_min, contentHeight);
    }
    scrollbar->setHeight(scrollbar_h);
    scrollTop = sRectY + scrollbar->height/2;
    scrollBottom = sRectY + sRectH - scrollbar->height/2;
}


float ofxUIDynamicListVerticalScrollbarCanvas::getContentHeight() {
    return contentHeight;
}

void ofxUIDynamicListVerticalScrollbarCanvas::setScrollbarWidth(float w) {
    ofxUIRectangle* track = scrollbar->getBounds();
    scrollbar->setWidth(w);
    track->setWidth(w);
    // keep scrollbar and track right aligned
    float x = sRect->getX() + sRect->getWidth() - w;
    scrollbar->setX(x);
    track->setX(x);
}

void ofxUIDynamicListVerticalScrollbarCanvas::setScrollbarMinHeight(float h) {
    scrollbar_h_min = h;
}

void ofxUIDynamicListVerticalScrollbarCanvas::showScrollbar() {
    show_scrollbar = true;
}

void ofxUIDynamicListVerticalScrollbarCanvas::hideScrollbar() {
    show_scrollbar = false;
}

ofRectangle ofxUIDynamicListVerticalScrollbarCanvas::getAvailableSpace() {
    return ofRectangle(rect->x, rect->y, rect->width - scrollbar->getWidth(), rect->height);
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
            for (int i = 0; i < w->getEmbeddedWidgetsSize(); i++) {
                w->getEmbeddedWidget(i)->draw();
            }
        }
    }
    if (show_scrollbar) {
        scrollbar->draw();
    }

    ofxUIPopStyle();
}

void ofxUIDynamicListVerticalScrollbarCanvas::mousePressed(int x, int y, int button) {
    if (!sRect->inside(x, y)) {
        return;
    }
    if (!scrollbar->getBounds()->inside(x, y)) {
        ofxUIScrollableCanvas::mousePressed(x, y, button);
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::mouseReleased(int x, int y, int button) {
    if (!sRect->inside(x, y)) {
        return;
    }
    if (!scrollbar->getBounds()->inside(x, y)) {
        ofxUIScrollableCanvas::mouseReleased(x, y, button);
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::mouseDragged(int x, int y, int button) {
    float sRectH = sRect->getHeight();
    float sRectY = sRect->getY();
    if (scrollbar->isHit() && scrollBottom != scrollTop) {
        float scrollPercent = (scrollbar->getCenterY() - scrollTop) / (scrollBottom - scrollTop);
        scrollPercent = CLAMP(scrollPercent, 0.0, 1.0);
        rect->y = sRectY - scrollPercent * (contentHeight - sRectH);
    } else {
        ofxUIScrollableCanvas::mouseDragged(x, y, button);
        float scrollPercent = (sRectY - rect->y) / (contentHeight - sRectH);
        scrollPercent = CLAMP(scrollPercent, 0.0, 1.0);
        float scrollbarY = sRectY + scrollPercent * (sRectH - scrollbar->height);
        scrollbar->y = scrollbarY;
    }
}

void ofxUIDynamicListVerticalScrollbarCanvas::scrollToBottom() {
    float sRectH = sRect->getHeight();
    float sRectY = sRect->getY();
    rect->y = sRectY - (contentHeight - sRectH);
    scrollbar->y = sRectY + (sRectH - scrollbar->height);
}

void ofxUIDynamicListVerticalScrollbarCanvas::sortWidgets(bool (*f)(const ofxUIWidget *, const ofxUIWidget *)) {
    listItems.sort(f);
    reflowWidgets();
}

void ofxUIDynamicListVerticalScrollbarCanvas::setListPadding(float padding) {
    listPadding = padding;
    reflowWidgets();
}

void ofxUIDynamicListVerticalScrollbarCanvas::update() {
    ofxUIScrollableCanvas::update();
}

ofxUIDraggableRect* ofxUIDynamicListVerticalScrollbarCanvas::getScrollbar() {
    return scrollbar;
}

list<ofxUIWidget*>* ofxUIDynamicListVerticalScrollbarCanvas::getWidgetList() {
    return &listItems;
}
