/*
 * ofxUIMultiLineTextInput.cpp
 *
 * text input that wraps around to multiple lines
 *
 * Created on: Sep 13, 2013
 *     Author: noura
 */

#include "ofxUIMultiLineTextInput.h"

ofxUIMultiLineTextInput::~ofxUIMultiLineTextInput() {
    delete textArea;
}

ofxUIMultiLineTextInput::ofxUIMultiLineTextInput(string _name, string _textString, float w, float h, float x, float y, int _size)
: ofxUIWidget()
, clicked(false)
, autoclear(true)
, triggerOnClick(true)
, triggerType(OFX_UI_TEXTINPUT_ON_FOCUS)
, cursorOffset(2.0)
, theta(0)
, cursorChar(0)
, cursorLine(0)
, shiftKeyPressed(false)
, xCorrection(0) // TODO fix it so I don't need this hack
, yCorrection(0)
, cursorWidth(2.0)
{
    name = string(_name);
    defaultstring = string(_textString);
    kind = OFX_UI_WIDGET_MULTILINETEXTINPUT;
    
    // TODO this is all a bad hack
    float margin_guess = 2; // TODO fix it so I don't need this hack
    paddedRect = new ofxUIRectangle(x + margin_guess, y + margin_guess, w - 2.0 * margin_guess, h - 2.0 * margin_guess);
    rect = new ofxUIRectangle(x + padding + margin_guess, y + padding + margin_guess, w - 2.0 * padding - 2.0 * margin_guess, h - 2.0 * padding - 2.0 * margin_guess);
    paddedRect->setParent(rect);

    textArea = new ofxUITextArea(name, defaultstring, rect->width, rect->height, rect->x, rect->y, _size);
    addEmbeddedWidget(textArea);
}

// TODO let the user set the cursor width with a float or to be the same width as a character in the textArea's font

void ofxUIMultiLineTextInput::moveCursorBackward() {
    if(cursorChar > 0) {
        cursorChar--;
    } else if (cursorLine > 0) {
        cursorLine--;
        cursorChar = textArea->textLines[cursorLine].size() - 1;
    }
    cursorLine = CLAMP(cursorLine, 0, textArea->textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textArea->textLines[cursorLine].size());
}

void ofxUIMultiLineTextInput::moveCursorForward() {
    if(textArea->textstring == "" ||
       cursorChar < textArea->textLines[cursorLine].size() - 1) {
        cursorChar++;
    } else if (cursorLine < textArea->textLines.size() &&
               cursorLine < textArea->textLines.size() - 1) {
        cursorLine++;
        cursorChar = 0;
    }
    cursorLine = CLAMP(cursorLine, 0, textArea->textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textArea->textLines[cursorLine].size());
}


int ofxUIMultiLineTextInput::getStringIndex() {
    // returns the index in textArea->textstring corresponding with the
    // cursor position specified by cursorChar and cursorLine
    // cursorChar x is between textArea->textstring[x-1] and textArea->textstring[x]
    // cursorChar = 0 is before textArea->textstring[0]
    int i = 0;
    int currentLineIndex = 0;
    while (currentLineIndex < cursorLine) {
        i += textArea->textLines[currentLineIndex++].size();
    }
    i += cursorChar;
    i = CLAMP(i, 0, textArea->textstring.size());
    return i;
}

int ofxUIMultiLineTextInput::setCursorPosition(int stringIndex) {
    // sets cursorChar and cursorLine to correspond with the stringIndex
    if (textArea->textLines.size() == 0) {
        cursorLine = 0;
        cursorChar = 0;
        return;
    }
    
    int count = 0; // count up to stringIndex
    int i = 0;
    
    for (int i = 0; i < textArea->textLines.size(); i++) {
        if (count + textArea->textLines[i].size() >= stringIndex) {
            cursorLine = i;
            cursorChar = stringIndex - count;
            break;
        } else {
            count += textArea->textLines[i].size();
        }
    }
    cursorLine = CLAMP(cursorLine, 0, textArea->textLines.size() - 1);
    cursorChar = CLAMP(cursorChar, 0, textArea->textLines[cursorLine].size());
}

void ofxUIMultiLineTextInput::clearText() {
    textArea->textstring.clear();
    //textArea->formatDisplayString();
    textArea->formatTextString();
    cursorChar = 0;
    cursorLine = 0;
}

void ofxUIMultiLineTextInput::drawCursor() {
    ofxUILabel * label = textArea->getLabelWidget();

    string beforeCursor = "";
    if (textArea->textLines.size() > 0) {
        beforeCursor = textArea->textLines[cursorLine].substr(0, cursorChar);
    }

    // we need to put "." on both ends of the string so that the width of
    // white space at the beginning or end of the string is included
    float xOffset = label->getStringWidth("." + beforeCursor + ".") - label->getStringWidth("..");
    
    float x = textArea->getRect()->getX() + xOffset;
    float y = textArea->getLineTopY(cursorLine);
    
    // cursor color oscillates
    ofxUIFill();
    ofxUISetColor(textArea->getLabelWidget()->getColorFillHighlight(), 255.0*fabs(cos(theta)));
    theta +=0.05;
    ofxUIDrawRect(x + xCorrection, y + yCorrection, cursorWidth, textArea->lineHeight);
}

void ofxUIMultiLineTextInput::drawFill() {
    if(draw_fill)
    {
        ofxUIFill(); 
        ofxUISetColor(color_fill); 
        rect->draw(); 
    }
    textArea->drawFill();
    if(clicked)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        rect->draw();
        drawCursor();
    }
    if(textArea->textstring == "" && !clicked)
    {
        ofxUIFill(); 
        ofxUISetColor(color_fill);
        textArea->getLabelWidget()->drawString(rect->x + 2.0, yCorrection + textArea->getLineBottomY(0), defaultstring);
    }
}

// TODO maybe I should inherit from ofxUITextInput so I don't have to re-implement mouseMoved etc
void ofxUIMultiLineTextInput::mouseMoved(int x, int y) {
    if(rect->inside(x, y))
    {
        state = OFX_UI_STATE_OVER;         			
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();         
}

void ofxUIMultiLineTextInput::mouseDragged(int x, int y, int button) {
    if(hit)
    {
        state = OFX_UI_STATE_DOWN;         
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;  
    }
    stateChange();     
}

void ofxUIMultiLineTextInput::mousePressed(int x, int y, int button) {
    if(rect->inside(x, y))
        focus();
    else
        unfocus();
}

void ofxUIMultiLineTextInput::mouseReleased(int x, int y, int button) {
    if(hit)
    {
#ifdef TARGET_OPENGLES
        state = OFX_UI_STATE_NORMAL;        
#else            
        state = OFX_UI_STATE_OVER; 
#endif 
//			triggerType = OFX_UI_TEXTINPUT_ON_UNFOCUS; 
//			triggerEvent(this); 
    }    
    else
    {
        state = OFX_UI_STATE_NORMAL;         
    }
    hit = false;
    stateChange();         
}

void ofxUIMultiLineTextInput::keyPressed(int key) {
    // TODO might be cleaner with an if (!clicked) return; instead
    if(clicked)
    {
        switch (key)
        {
            case OF_KEY_BACKSPACE:
            {
                if (textArea->textstring.size() > 0) {
                    int i = getStringIndex();
                    if (i > 0) {
                        textArea->textstring.erase(i - 1, 1);
                        //textArea->formatDisplayString();
                        textArea->formatTextString();
                        setCursorPosition(i - 1);
                    }
                }
            }
                break;

            case OF_KEY_DEL:
            {
                if (textArea->textstring.size() > 0) {
                    int i = getStringIndex();
                    if (i < textArea->textstring.size()) {
                        textArea->textstring.erase(i, 1);
                        //textArea->formatDisplayString();
                        textArea->formatTextString();
                        setCursorPosition(i);
                    }
                }
            }
                break;
                
            case OF_KEY_RETURN:
                triggerType = OFX_UI_TEXTINPUT_ON_ENTER;
                triggerEvent(this);
                if(autoclear) {
                    string input = textArea->textstring.substr();
                    clearText();
                    clicked = true;
                }
                break;
                
            case OF_KEY_RIGHT:
                moveCursorForward();
                break;					

            case OF_KEY_LEFT:
                moveCursorBackward();
                break;
                
            case OF_KEY_DOWN:
                if (cursorLine < textArea->textLines.size() - 1) {
                    cursorLine++;
                    cursorChar = CLAMP(cursorChar, 0, textArea->textLines[cursorLine].size());
                }
                break;
                
            case OF_KEY_UP:
                if (cursorLine > 0) {
                    cursorLine--;
                    cursorChar = CLAMP(cursorChar, 0, textArea->textLines[cursorLine].size());
                }
                break;

#if (OF_VERSION_MINOR > 7)
            case OF_KEY_TAB:
            case OF_KEY_COMMAND:
            case OF_KEY_CONTROL:
            case OF_KEY_LEFT_CONTROL:
            case OF_KEY_RIGHT_CONTROL:
            case OF_KEY_LEFT_ALT:
            case OF_KEY_RIGHT_ALT:
            case OF_KEY_LEFT_SUPER:
            case OF_KEY_RIGHT_SUPER:					
#endif
            case OF_KEY_F1:
            case OF_KEY_F2:
            case OF_KEY_F3:
            case OF_KEY_F4:
            case OF_KEY_F5:
            case OF_KEY_F6:
            case OF_KEY_F7:
            case OF_KEY_F8:
            case OF_KEY_F9:
            case OF_KEY_F10:
            case OF_KEY_F11:
            case OF_KEY_F12:
            case OF_KEY_PAGE_UP:
            case OF_KEY_PAGE_DOWN:
            case OF_KEY_HOME:
            case OF_KEY_END:
            case OF_KEY_INSERT:
            case OF_KEY_ALT:
            break;
              
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
            case OF_KEY_SHIFT:
                shiftKeyPressed = true;
                break;
                
            default:
            {
                char c = key;
                if (key == OFXUI_KEY_BACK_QUOTE) { //TODO better way to do this
                    c = '`';
                }
                if (shiftKeyPressed) {
                    if (key == OFXUI_KEY_BACK_QUOTE) c = '~';
                    if (key == '1') c = '!';
                    if (key == '2') c = '@';
                    if (key == '3') c = '#';
                    if (key == '4') c = '$';
                    if (key == '5') c = '%';
                    if (key == '6') c = '^';
                    if (key == '7') c = '&';
                    if (key == '8') c = '*';
                    if (key == '9') c = '(';
                    if (key == '0') c = ')';
                    if (key == '-') c = '_';
                    if (key == '=') c = '+';
                    if (key == '[') c = '{';
                    if (key == ']') c = '}';
                    if (key == '\\') c = '|';
                    if (key == ';') c = ':';
                    if (key == '\'') c = '"';
                    if (key == ',') c = '<';
                    if (key == '.') c = '>';
                    if (key == '/') c = '?';
                }
                int i = getStringIndex();
                textArea->textstring.insert(i, 1, c);
                //textArea->formatDisplayString();
                textArea->formatTextString();
                setCursorPosition(i + 1);
            }
                break;
        }
    }
}

void ofxUIMultiLineTextInput::keyReleased(int key) {
    // TODO if (!clicked)...
    if (clicked) {
        switch (key) {
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
            case OF_KEY_SHIFT:
                shiftKeyPressed = false;
                break;
            default:
                break;
        }
    }
}

void ofxUIMultiLineTextInput::unClick() {
    if(clicked)
    {
        clicked = false;          
        triggerType = OFX_UI_TEXTINPUT_ON_UNFOCUS; 
        triggerEvent(this);             
    }
}

void ofxUIMultiLineTextInput::focus() {
    clicked = true;
    theta = 0;
    hit = true;
    
    setCursorPosition(textArea->textstring.size());
    
    state = OFX_UI_STATE_DOWN;
    triggerType = OFX_UI_TEXTINPUT_ON_FOCUS;
    
    if(triggerOnClick) triggerEvent(this);
    
    stateChange();
}

void ofxUIMultiLineTextInput::unfocus() {
    state = OFX_UI_STATE_NORMAL;
    if(clicked) unClick();
    stateChange();
}

void ofxUIMultiLineTextInput::stateChange() {
    // TODO maybe instead of having to call stateChange() every time we change state = , this method should assign to state and do these things in reaction
    switch (state) {
        case OFX_UI_STATE_NORMAL:
        {            
            draw_fill_highlight = false;             
            draw_outline_highlight = false;  
            textArea->getLabelWidget()->unfocus();
        }
            break;
        case OFX_UI_STATE_OVER:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = true;  
            textArea->getLabelWidget()->focus();
        }
            break;
        case OFX_UI_STATE_DOWN:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = true;             
            textArea->getLabelWidget()->focus();
        }
            break;
        case OFX_UI_STATE_SUSTAINED:
        {
            draw_fill_highlight = false;            
            draw_outline_highlight = false;                         
            textArea->getLabelWidget()->unfocus();
        }
            break;                            
        default:
            break;
    }        
}

string ofxUIMultiLineTextInput::getTextString() {
    return textArea->textstring;
}

void ofxUIMultiLineTextInput::setTextString(string s) {
    textArea->setTextString(s);
    cursorChar = 0;
}

void ofxUIMultiLineTextInput::setVisible(bool _visible) {
    visible = _visible;
    textArea->setVisible(visible);
}

bool ofxUIMultiLineTextInput::isClicked() {
    return clicked; 
}

ofxUITextArea* ofxUIMultiLineTextInput::getTextArea() {
    return textArea;
}

void ofxUIMultiLineTextInput::setTriggerType(int _triggerType) {
    triggerType = _triggerType;
}

int ofxUIMultiLineTextInput::getTriggerType() {
    return triggerType;
}

//TODO make sure trigger types work. also what is setTriggerOnClick for? how is that different than the other triggers?
void ofxUIMultiLineTextInput::setAutoClear(bool _autoclear) {
    autoclear = _autoclear;
}

void ofxUIMultiLineTextInput::setTriggerOnClick(bool _triggerOnClick) {
    triggerOnClick = _triggerOnClick;
}
