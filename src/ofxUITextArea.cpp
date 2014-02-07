/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#include "ofxUITextArea.h"
#include "ofxUI.h"

ofxUITextArea::ofxUITextArea(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUIWidgetWithLabel()
{
    init(_name, _textstring, w, h, x, y, _size);
}

void ofxUITextArea::init(string _name, string _textstring, float w, float h, float x, float y, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TEXTAREA;
    textstring = _textstring;
    setDrawFill(true);
    setDrawBack(false);
    drawShadow = false;
    drawLabel = false;
    
    label = new ofxUILabel(padding*2.0,0,(name+" LABEL"), _size);
    addEmbeddedWidget(label);
    label->setVisible(drawLabel);
    
    if(h == 0)
    {
        autoSize = true;
    }
    else
    {
        autoSize = false;
    }
}

void ofxUITextArea::drawBack()
{
    if(draw_back)
    {
        ofFill();
        ofSetColor(color_back);
        rect->draw();
    }
}

void ofxUITextArea::drawFill()
{
    if(draw_fill)
    {
        if(drawShadow)
        {
            ofSetColor(color_back);
            for(unsigned int i = 0; i < textLines.size(); i++)
            {
                label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
            }
        }
        
        ofSetColor(color_fill);
        for(unsigned int i = 0; i < textLines.size(); i++)
        {
            label->drawString(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
        }
    }
}

string ofxUITextArea::getTextString()
{
    return textstring;
}


void ofxUITextArea::setTextString(string s)
{
    textLines.clear();
    textstring = s;
    formatTextString();
}

void ofxUITextArea::formatTextString()
{
    textLines.clear();
    
    lineHeight = label->getStringHeight("1");
    lineSpaceSize = padding*2;
    
    float widthLimit = rect->getWidth() - label->getStringWidth("M")- 2 * padding;
    if (widthLimit < label->getStringWidth("M")) {
        return;
    }
    float heightLimit = rect->getHeight();
    
    int numLinesLimit;
    if (autoSize) {
        numLinesLimit = 0;
    } else {
        float denominator = lineHeight + lineSpaceSize;
        if (denominator == 0) { // before fonts are initialized
            denominator = OFX_UI_FONT_LARGE_SIZE;
        }
        numLinesLimit = (int)heightLimit / denominator;
    }
    
    string line = "";
    for (int i = 0; i < textstring.size(); i++) {
        if (!autoSize && textLines.size() > numLinesLimit) {
            break;
        }
        if (label->getStringWidth(line) >= widthLimit) {
            if (line.size() == 0) {
                break;
            }
            // try to break line at white space
            int whitespace_index = line.size() - 1;
            while (whitespace_index > 0 &&
                   !isspace(line.at(whitespace_index))) {
                whitespace_index--;
            }
            if (whitespace_index <= 0) {
                // white space not found, or found at first char
                textLines.push_back(line.substr(0, line.size()-1));
                line = line.substr(line.size()-1);
            } else {
                // white space found
                textLines.push_back(line.substr(0, whitespace_index));
                line = line.substr(whitespace_index);
            }
        }
        char c = textstring.at(i);
        line += c;
    }
    if (line != "" && !(!autoSize && textLines.size() > numLinesLimit)) {
        textLines.push_back(line);
    }
    
    if(autoSize) {
        rect->setHeight((lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize);
    }
}

void ofxUITextArea::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
    formatTextString();
    calculatePaddingRect();
}

void ofxUITextArea::setDrawShadow(bool _drawShadow)
{
    drawShadow = _drawShadow;
}


float ofxUITextArea::getLineBottomY(int line_index) {
    return rect->getY() + (line_index + 1) * (lineHeight + lineSpaceSize) - lineSpaceSize;
}

float ofxUITextArea::getLineTopY(int line_index) {
    return getLineBottomY(line_index) - lineHeight;
}

