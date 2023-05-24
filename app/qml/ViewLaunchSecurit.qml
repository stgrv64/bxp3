/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.7
import QtMultimedia 5.8
import "colibri/"

Rectangle {
    property bool morpho_enabled:false
//    property bool morpho_status: morpho_enabled
    property bool security_enabled:false
    property bool securitymax_enabled:false
    property bool heure_opacity:false
    anchors.fill: parent
    color: "#F0F0F0"
   MouseArea {
        anchors.fill: parent
        onClicked: {
            security_enabled=false
            securitymax_enabled=false
            keyboard.text=""
            keyboard.opacity=1
            textinput1.opacity=0.2
            image.opacity=1
            heure_opacity=false
            screen_touch();main.state="view1";}
        }

   function test_clear()
   {
       security_enabled=false
       securitymax_enabled=false
       keyboard.text=""
       keyboard.opacity=1
       textinput1.opacity=0.2
       image.opacity=1
       screen_touch()
   }

   function test_securit()
   {

       var a=Qt.formatTime(new Date(), "hh");
       var b=Qt.formatTime(new Date(), "mm");
       if(textinput1.text==a+b)
           {
           keyboard.text=""
           security_enabled=true
           securitymax_enabled=false
           keyboard.opacity=0
           textinput1.opacity=0
           image.opacity=0
           heure_opacity=true
           }
       else if(textinput1.text==a+"."+b)
           {
           keyboard.text=""
           security_enabled=true
           securitymax_enabled=true
           keyboard.opacity=0
           textinput1.opacity=0
           image.opacity=0
           heure_opacity=true
           }
       else if(textinput1.text=="..0..")
           {
           qml_message("reboot");
           security_enabled=false
           securitymax_enabled=false
           keyboard.text=""
           keyboard.opacity=1
           textinput1.opacity=0.1
           image.opacity=1
           heure_opacity=false
           }
       else if(textinput1.text=="..9..")
           {
           test_clear();
           qml_message("restart");
           security_enabled=false
           securitymax_enabled=false
           keyboard.text=""
           keyboard.opacity=1
           textinput1.opacity=0.1
           image.opacity=1
           heure_opacity=false
           }
       else
           {
           test_clear();
           security_enabled=false
           securitymax_enabled=false
           keyboard.text=""
           keyboard.opacity=1
           textinput1.opacity=0.1
           image.opacity=1
//           heure_opacity=false
           }

   }


   NumericKeyboard{
     id:keyboard
     kwidth:200
     kheight:300
     maxlength: 5
     anchors.horizontalCenter: parent.horizontalCenter
     anchors.verticalCenter: parent.verticalCenter
     onClicked: {viewid9_timer.stop();viewid9_timer.start();}
 }

   Text {
      text:main.heure
      anchors.top:keyboard.bottom
      anchors.topMargin:40
      anchors.horizontalCenter: parent.horizontalCenter
      font.family: "FreeMono"
      font.pixelSize: 20
      color:"#e89101"
      opacity:heure_opacity & keyboard.opacity
   }




 TextInput {
     id:textinput1
     anchors.horizontalCenter: parent.horizontalCenter
     anchors.top: parent.top
     anchors.topMargin:20
     text: keyboard.text
        maximumLength:10
        font.family: "Liberation Sans Narrow"
        font.pixelSize: 30
        opacity:0.1
        color: "black"
 }

 Image {
         id:image
         opacity:1
         height:source.height
          width: source.width
          anchors.top: parent.top
          anchors.topMargin:20
          anchors.left:textinput1.right
          anchors.leftMargin:5
          source :"qrc:/qml/content/Ok.png"
          MouseArea {
                     anchors.fill: parent
                     onClicked: {test_securit();viewid9_timer.stop();viewid9_timer.start();}
                    }

     }




   Image {
            id : setup_launch
            height:source.height
            width: source.width
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.topMargin:10
            anchors.leftMargin:10
            opacity: security_enabled
            enabled: security_enabled
            source :"qrc:/qml/content/setup.png"
            MouseArea {
                       anchors.fill: parent
                       onClicked: {test_clear();viewsetup.clear();main.state="view9";}
                      }

       }


   Image {
             id : datetime
             height:source.height
             width: source.width
             anchors.horizontalCenter: parent.horizontalCenter
             anchors.top:parent.top
             anchors.topMargin:10
             opacity: security_enabled
             enabled: security_enabled
            source :"qrc:/qml/content/date-and-time.png"
             MouseArea {
                        anchors.fill: parent
                        onClicked: {test_clear();viewhorodate.an= Qt.formatDate(new Date(), "yyyy"); viewhorodate.mois= Qt.formatDate(new Date(), "MM"); viewhorodate.jour= Qt.formatDate(new Date(), "dd");
                            viewhorodate.heure= Qt.formatTime(new Date(), "hh"); viewhorodate.minute=Qt.formatTime(new Date(), "mm"); viewhorodate.seconde=Qt.formatTime(new Date(), "ss");
                            main.state="view4";viewid9_timer.stop();viewid9_timer.start();}
                       }

        }

   Image {
           id : ts_calibratei
           height:source.height
           width: source.width
           anchors.top:parent.top
           anchors.topMargin:10
           anchors.right:parent.right
           anchors.rightMargin:10
           opacity: security_enabled
           enabled: security_enabled
           source :"qrc:/qml/content/ts_calibrate.png"
           MouseArea {
                      anchors.fill: parent
                      onClicked:{test_clear();signal_qml_ts_calibrate();}
                     }

      }


   Image {
            id : morpho_launch
            height:source.height
            width: source.width
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: morpho_enabled & (security_enabled | securitymax_enabled)
            enabled: morpho_enabled & (security_enabled | securitymax_enabled)
            source :"qrc:/qml/content/morpho.png"
            MouseArea {
                       anchors.fill: parent
                       onClicked: {test_clear();signal_morpho_stop();main.state="view7";}
                      }

       }

}
