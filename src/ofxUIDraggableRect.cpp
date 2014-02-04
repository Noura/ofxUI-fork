/*
 * ofxUIDraggableRect.h
 *
 * an ofxUIRectangle that can be dragged around with the mouse
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#include "ofxUIDraggableRect.h"

ofxUIDraggableRect::~ofxUIDraggableRect() {
    delete bounds;
    delete image;
    ofRemoveListener(ofEvents().mousePressed, this, &ofxUIDraggableRect::onMousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxUIDraggableRect::onMouseReleased);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxUIDraggableRect::onMouseDragged);
}

ofxUIDraggableRect::ofxUIDraggableRect(float x, float y, float w, float h, ofRectangle boundsRect)
: ofxUIRectangle(x, y, w, h)
, hit(false)
, mouseDx(0)
, mouseDy(0)
, image(NULL) {
    bounds = new ofxUIRectangle(boundsRect.x, boundsRect.y, boundsRect.width, boundsRect.height);
    ofAddListener(ofEvents().mousePressed, this, &ofxUIDraggableRect::onMousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxUIDraggableRect::onMouseReleased);
    ofAddListener(ofEvents().mouseDragged, this, &ofxUIDraggableRect::onMouseDragged);
}

bool ofxUIDraggableRect::isHit() {
    return hit;
}

float ofxUIDraggableRect::getCenterX() {
    return getMinX() + width/2;
}

float ofxUIDraggableRect::getCenterY() {
    return getMinY() + height/2;
}

void ofxUIDraggableRect::setHeight(float h) {
    ofxUIRectangle::setHeight(h);
    if (image) {
        image->resize(width, height);
    }
}

void ofxUIDraggableRect::setImage(string imagePath) {
    delete image;
    image = new ofImage();
    image->loadImage(imagePath);
    image->resize(width, height);
}

void ofxUIDraggableRect::removeImage() {
    delete image;
    image = NULL;
}

void ofxUIDraggableRect::draw() {
    ofxUIRectangle::draw();
    if (image) {
        ofPushStyle();
        ofSetColor(255);
        image->draw(x, y);
        ofPopStyle();
    }
}

void ofxUIDraggableRect::onMousePressed(ofMouseEventArgs &e) {
    if (inside(e.x, e.y)) {
        hit = true;
        mouseDx = e.x - x;
        mouseDy = e.y - y;
    }
}

void ofxUIDraggableRect::onMouseReleased(ofMouseEventArgs &e) {
    hit = false;
    mouseDx = 0;
    mouseDy = 0;
}

void ofxUIDraggableRect::onMouseDragged(ofMouseEventArgs &e) {
    //TODO we assume that this->x and this->y are top left corner and that bounds->x and bounds->y are top left corner
    if (hit) {
        x = CLAMP(e.x - mouseDx, bounds->x, bounds->x + bounds->width - width);
        y = CLAMP(e.y - mouseDy, bounds->y, bounds->y + bounds->height - height);
    }
}
