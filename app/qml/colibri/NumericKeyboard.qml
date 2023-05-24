/**
 *  Copyright © 2010 Digia Plc
 *  Copyright © 2010 Nokia Corporation
 *
 *  All rights reserved.
 *
 *  Nokia and Nokia Connecting People are registered trademarks of
 *  Nokia Corporation.
 *  Java and all Java-based marks are trademarks or registered
 *  trademarks of
 *  Sun Microsystems, Inc. Other product and company names
 *  mentioned herein may be
 *  trademarks or trade names of their respective owners.
 *
 *
 *  Subject to the conditions below, you may, without charge:
 *
 *  ·  Use, copy, modify and/or merge copies of this software and
 *     associated documentation files (the "Software")
 *
 *  ·  Publish, distribute, sub-licence and/or sell new software
 *     derived from or incorporating the Software.
 *
 *
 *  This file, unmodified, shall be included with all copies or
 *  substantial portions
 *  of the Software that are distributed in source code form.
 *
 *  The Software cannot constitute the primary value of any new
 *  software derived
 *  from or incorporating the Software.
 *
 *  Any person dealing with the Software shall not misrepresent
 *  the source of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 *  KIND, EXPRESS OR IMPLIED,
 *  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A
 *  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
import QtQuick 2.7
import QtMultimedia 5.8
//import "javascripts/keyboard.js" as KeyboardEngine
import "gradients/"
import "includes/"

/**
 * More simple keyboard. Loading time optimized.
 */
Rectangle {
    id: keyboard

    property CLStyle style : CLStyle { }

    property int kwidth:200
    property int kheight:250
    property string text
    property int maxlength
    //    property real buttonWidth: ((keyboard.width-buttonSpacing*11)/11) - (keyboard.width*0.05)/11 ;
    property real buttonWidth: ((keyboard.width-buttonSpacing*6)/3);
    property real buttonHeight: (keyboard.height-buttonSpacing*6)/4;
    property int buttonSpacing: width*0.01


    property Gradient buttonGradientWhenSelected : style.gradientWhenSelected
    property color buttonColorWhenSelected : style.colorWhenSelected
    property bool buttonGradientSelectedOn : style.gradientSelectedOn


    property real shiftTextFontSize : buttonFontSize*0.7
    property string shiftTextFontFamily : style.fontFamily
    property string shiftTextFontWeight : style.fontWeight
    property color shiftTextColor : style.textColor


    property real imageWidth : buttonWidth
    property real imageHeight : buttonHeight

    property Image backspaceImage : Image { source: "images/backspace_50x70.png"; scale: buttonWidth*0.01; smooth: true}
     property Image capsImage : nullImage


    //properties straight from button
    property color buttonColorWhenDefault : style.colorWhenDefault
    property color buttonColorWhenPressed : style.colorWhenPressed
    property color buttonColorWhenHovered : style.colorWhenHovered
    property color buttonTextColor : style.textColor
    property real buttonRoundness : style.roundness
    property color buttonBorderColor : style.borderColor
    property int buttonBorderWidth : style.borderWidth
    property real buttonFontSize : height*0.1
    property string buttonFontFamily: style.fontFamily
    property string buttonFontWeight : style.fontWeight
    property color buttonBorderColorWhenHovered : style.borderColorWhenHovered
    property color buttonBorderColorWhenPressed : style.borderColorWhenPressed


    /* Properties for background images
     * --------------------------------
     * This solution is temporary. Remember performance.
     */
    property Image nullImage : Image { //this property is "private" don't write it to documentation
        id: "null"
        source: ""
        width: buttonWidth
        height: buttonHeight
        fillMode: Image.PreserveAspectCrop
    }

    property Image buttonBackgroundImage : nullImage

    property Image buttonBackgroundImageWhenHovered : nullImage
    property Image buttonBackgroundImageWhenPressed : nullImage

    property Image buttonCurrentImage : buttonBackgroundImage //this property is "private" don't write it to documentation

    property bool buttonGradientDefaultOn : style.gradientDefaultOn
    property bool buttonGradientHoveredOn : style.gradientHoveredOn
    property bool buttonGradientPressedOn : style.gradientPressedOn

    property bool buttonHoveredStateOn : style.hoveredStateOn
    property bool buttonPressedStateOn : style.pressedStateOn

    property Gradient buttonGradientWhenDefault : style.gradientWhenDefault
    property Gradient buttonGradientWhenHovered : style.gradientWhenHovered
    property Gradient buttonGradientWhenPressed : style.gradientWhenPressed

    property string buttonTextAlign: "center"
    property real buttonLeftMargin: 0
    property real buttonRightMargin: 0


    signal leftArrowClicked()
    signal rightArrowClicked()
    signal upArrowClicked()
    signal downArrowClicked()
    signal okCLButtonClicked()
    signal cancelCLButtonClicked()
    signal clicked()
    signal backspaceClicked()


   /**
     * Function which will be performed when button is clicked. In this moment
     * only used by buttons with shift value.
     *
     * @param active text(character) of button which is clicked
     * @return nothing
     */
    function keyboardCLButtonClicked(character) {
        if (keyboard.text.length < maxlength) keyboard.text += character;
       keyboard.clicked();
    }



         Component.onCompleted: {
             if (backspaceImage.source != "") backspace.text = "";
             //console.log("NumericKeyboard: ");
             //console.log(buttonFontSize);
             styleCLButton.fontSize = buttonFontSize;
            }

    clip: true;
    smooth: true
    width: kwidth
    height: kheight
    color: "#b5adad"
    border.color: "black"

    //this element is only used for forwarding properties to buttons
    CLButton { id: styleCLButton; visible: false; style: keyboard.style; colorWhenDefault: buttonColorWhenDefault; colorWhenPressed: buttonColorWhenPressed; colorWhenHovered: buttonColorWhenHovered; textColor: buttonTextColor; roundness: buttonRoundness; borderColor: buttonBorderColor; borderWidth: buttonBorderWidth; fontSize: 25; fontFamily: buttonFontFamily; fontWeight: buttonFontWeight; borderColorWhenHovered: buttonBorderColorWhenHovered; borderColorWhenPressed: buttonBorderColorWhenPressed; backgroundImage: buttonBackgroundImage; backgroundImageWhenHovered: buttonBackgroundImageWhenHovered; backgroundImageWhenPressed: buttonBackgroundImageWhenPressed; gradientDefaultOn: buttonGradientDefaultOn; gradientHoveredOn: buttonGradientHoveredOn; gradientPressedOn: buttonGradientPressedOn; hoveredStateOn: buttonHoveredStateOn; pressedStateOn: buttonPressedStateOn; gradientWhenDefault: buttonGradientWhenDefault; gradientWhenHovered: buttonGradientWhenHovered; gradientWhenPressed: buttonGradientWhenPressed; textAlign: buttonTextAlign; leftMargin: buttonLeftMargin; rightMargin: buttonRightMargin; smooth: keyboard.smooth; gradientWhenSelected: buttonGradientWhenSelected; colorWhenSelected: buttonColorWhenSelected; gradientSelectedOn: buttonGradientSelectedOn}


        Column {
            id: abcCLButtons

            spacing: buttonSpacing
            y: buttonHeight*0.05
            x: buttonWidth*0.1


                Row {
                    id: row1

                    spacing: buttonSpacing

                    KeyboardButton { text: value; value: "7"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                    KeyboardButton { text: value; value: "8"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                    KeyboardButton { text: value; value: "9"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                }

                 Row {
                    id: row2

                    spacing: buttonSpacing

                    KeyboardButton { text: value; value: "4"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                    KeyboardButton { text: value; value: "5"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                    KeyboardButton { text: value; value: "6"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor }
                }


                Row {
                    id: row3
                    spacing: buttonSpacing
                    KeyboardButton { text: value; value: "1"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor}
                    KeyboardButton { text: value; value: "2"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor}
                    KeyboardButton { text: value; value: "3"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor}
                 }

                Row {
                    id: row4
                    spacing: buttonSpacing
                    KeyboardButton { text: value; value:"0"; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor}
                    KeyboardButton { text: value; value: "."; shiftValue: ""; style: keyboard.style; width: buttonWidth; height: buttonHeight; onClicked: keyboardCLButtonClicked(text); buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor}
                    KeyboardButton { id: backspace; shiftValue: ""; text:"<--"; style: keyboard.style; width: buttonWidth; height: buttonHeight; specialImage: backspaceImage; onClicked: {
                                       keyboard.text = keyboard.text.substring(0,keyboard.text.length-1);
                                       backspaceClicked()} buttonCLStyle: styleCLButton; shiftTextFontSize: keyboard.shiftTextFontSize; shiftTextFontFamily: keyboard.shiftTextFontFamily; shiftTextFontWeight: keyboard.shiftTextFontWeight; shiftTextColor: keyboard.shiftTextColor;
                                   }
                  }
    }

}
