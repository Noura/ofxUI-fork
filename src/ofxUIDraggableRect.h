/*
 * ofxUIDraggableRect.h
 *
 * an ofxUIRectangle that can be dragged around with the mouse
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#ifndef OFXUI_DRAGGABLE_RECT
#define OFXUI_DRAGGABLE_RECT

#include "ofxUIRectangle.h"
#include "ofMain.h"

class ofxUIDraggableRect : public ofxUIRectangle {

public:

    ~ofxUIDraggableRect();
    ofxUIDraggableRect(float x, float y, float w, float h, ofRectangle boundsRect);
    bool isHit();
    float getCenterX();
    float getCenterY();
    void setHeight(float h);
    void setImage(string imagePath);
    void draw();
    virtual void onMousePressed(ofMouseEventArgs &e);
    virtual void onMouseReleased(ofMouseEventArgs &e);
    virtual void onMouseDragged(ofMouseEventArgs &e);

protected:

    bool hit;
    float mouseDx, mouseDy;
    ofxUIRectangle * bounds;
    ofImage * image;

};

#endif
