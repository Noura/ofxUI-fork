/*
 * ofxUIMultiLineTextInput.h
 *
 * text input that wraps around to multiple lines
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#pragma once

// TODO make it work even when you don't give it the x and y initially

// TODO i think this is mac osx only
#define OFXUI_KEY_BACK_QUOTE 161

#include "ofxUILabel.h"
#include "ofxUIWidgetWithLabel.h"
#include "ofxUITextArea.h"

class ofxUIMultiLineTextInput : public ofxUIWidget
{
public:

    ~ofxUIMultiLineTextInput();
    ofxUIMultiLineTextInput(string _name, string _textString, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM_SIZE); // TODO should this be OFX_UI_FONT_MEDIUM or OFX_UI_FONT_MEDIUM_SIZE ???

    void moveCursorBackward();
    void moveCursorForward();
    int getStringIndex();
    int setCursorPosition(int stringIndex);

    void clearText();

    // TODO expose textArea or other objects to give nice options like setDrawPadding or setDrawPaddingOutline?

    void drawCursor();
    virtual void drawFill();

    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void keyPressed(int key);
    void keyReleased(int key);

    void unClick();
    void focus();
    void unfocus();
    void stateChange();

    string getTextString();
    void setTextString(string s);
    void setVisible(bool _visible);
    bool isClicked();
    ofxUITextArea* getTextArea();
    void setTriggerType(int _triggerType);
    int getTriggerType();
    //TODO make sure trigger types work. also what is setTriggerOnClick for? how is that different than the other triggers?
    void setAutoClear(bool _autoclear);
    void setTriggerOnClick(bool _triggerOnClick);

    float xCorrection, yCorrection; // TODO should not need these

protected:
    ofxUITextArea *textArea;
    string defaultstring;

    bool clicked, autoclear, triggerOnClick;
    int triggerType;

    float theta, cursorWidth, cursorOffset;
    int cursorChar, cursorLine;

    bool shiftKeyPressed;
};
