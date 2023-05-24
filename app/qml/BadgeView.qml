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
 //import badge 1.0
//import "content"


/*
This is exactly the same as states.qml, except that we have appended
a set of transitions to apply animations when the item changes
between each state.
*/


Item{
   id: badge
   anchors.fill: parent

/*        anchors.horizontalCenter: parent.horizontalCenter
    width: badge_logo.width
    height: badge_logo.height
*/

   function badge_valide(){animation_fin.running=true; badge_empreinte_attente_text.text="";badge_empreinte_attente.opacity=0;badge_empreinte_attente.source=""}

   SequentialAnimation {
        id:badge_logo_animation
            PropertyAnimation  { target: badge_logo; properties: "zoomRatio"; to: 0;easing.type: Easing.Linear;duration: 1;}
            PropertyAnimation  { target: badge_logo; properties: "zoomRatio"; to: 1.5;easing.type: Easing.Linear;duration: 100;}
            }

   PropertyAnimation  { id:animation_fin; target: badge_logo; properties: "zoomRatio"; to: 0;easing.type: Easing.Linear; duration: 100;
    onStopped:{badge_logo_animation.running=false;badge_text.text="";badge_logo.source="";/*badge_audio.stop()*/}
    }

   function stop_animation()
   {
       badge_logo_animation.stop();
       badge_text.text="";
       badge_logo.source="";
   }

   function attente_badge_empreinte(text) {
       badge_empreinte_attente.source="qrc:/qml/content/message_badge_empreinte.png"
       badge_empreinte_attente_text.text=text
       badge_empreinte_attente.opacity=1
       empreinte_timer.restart();
       }


   function new_badge(text,auto) {
        badge_empreinte_attente_text.text=""
       badge_empreinte_attente.opacity=0
       signal_audio_play("/var/x7/contents/wav/badge_ok.wav")
       badge_text.text="";
       badge_logo.source="qrc:/qml/content/badge.png";
       badge_logo_animation.running=true;
       badge_text.text=text;
       if (auto) badge_timer.restart();

   }

   function erreur_badge() {
       badge_empreinte_attente_text.text=""
      badge_empreinte_attente.opacity=0
       signal_audio_play("/var/x7/contents/wav/badge_inconnu.wav")
       badge_text.text="";
       badge_logo.source="qrc:/qml/content/badge_inconnu.png";
       badge_logo_animation.running=true;
       badge_timer.restart();

   }


   Timer {
       id: badge_timer
           interval: 1000; repeat:false;running:true
           onTriggered:
               PropertyAnimation  { target: badge_logo; properties: "zoomRatio"; to: 0;easing.type: Easing.Linear; duration: 250;
               onStopped:{badge_logo_animation.running=false;badge_text.text="";badge_logo.source="";/*badge_audio.stop()*/}
                }

        }

   Timer {
       id: empreinte_timer
           interval: 10000; repeat:false;running:true
           onTriggered:{main.reset_touche();main.badge_clear();badge_empreinte_attente.source="";badge_empreinte_attente.opacity=0}

          }


   Image
        {
        id: badge_logo
        property real zoomRatio


        width: sourceSize.width * zoomRatio
        height: sourceSize.height * zoomRatio


        Text {
            id: badge_text
            opacity:badge_text_opacity
            color: "#020203"
            anchors.right:  badge_logo.right
            anchors.rightMargin: 2* badge_logo.zoomRatio
            anchors.top: badge_logo.top
            anchors.topMargin: 2* badge_logo.zoomRatio +50
            font.family: "Ubuntu Condensed"
            font.pixelSize: Math.round(15*badge_logo.zoomRatio)+1
            text:""
           }
        }

   Image
        {
         id:badge_empreinte_attente
        opacity:0
 //       source:"qrc:/qml/content/message_badge_empreinte.png"

        width: sourceSize.width
        height: sourceSize.height
        anchors.horizontalCenter:parent.horizontalCenter


        Text {
            id:badge_empreinte_attente_text
            anchors.verticalCenter:parent.verticalCenter
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.verticalCenterOffset:-20
            color: "black"
            font.family: "Ubuntu Condensed"
            font.pixelSize:35
            text:""
           }
        MouseArea {
           anchors.fill: parent
           onClicked:
              {
               badge_empreinte_attente.source=""
               parent.opacity=00
               main.badge_clear()
               main.reset_touche()
               }
           onPressed:
              {
               badge_empreinte_attente.source=""
               parent.opacity=00
               main.badge_clear()
               main.reset_touche()
               }
           }
        }


}

