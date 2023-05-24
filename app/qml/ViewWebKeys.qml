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

Item {
   anchors.fill: parent
   property real alerte_opacity:0
   property bool alerte_enabled:false
   property real usbkey_opacity:0
   property bool usbkey_enabled:false


   MouseArea {
        anchors.fill: parent
        onClicked: {screen_touch()}
        }


   Image {
         id:logo
         width:622*0.2
         height:446*0.2
         anchors.top:parent.top
         anchors.topMargin:10
         anchors.leftMargin:5
         anchors.left:parent.left
         opacity: 1
         source: "file:/home/root/logo_client.png"
         onStatusChanged: {
                   if (logo.status == Image.Error) {
                          logo.source = "qrc:/qml/content/gfi.png"
                   }
               }

         MouseArea {
              anchors.fill: parent
              onClicked:
                {
 /*               badge_text=""
                touche_activite='-'
   */
                webbrowserid.stop()
                main.reset_touche()
                badgewebkeys.badge_valide()
                badge_clear()
              }
             }
        }


 Image {
       id:bulle
       width:main.orientation == 0 ? 229*0.75 : 229*0.79
       height:main.orientation == 0 ? 450*0.75 : 450*0.79
       anchors.bottom:parent.bottom
       anchors.left:parent.left
       anchors.leftMargin:5
       opacity: 1
       source: "qrc:/qml/content/bulles.png"

       MouseArea {
            anchors.fill: parent
            onClicked:
              {
               webbrowserid.stop()
                main.reset_touche()
               badgewebkeys.badge_valide()
               badge_clear()
               main.state="view1"
            }
           }
       }



  Rectangle {
       id:horodate
       width: main.orientation == 0 ? 450 : 370
  //     anchors.right:main.orientation == 0 ? touchesfonction.left :parent.right
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.top:parent.top
       height:446*0.2
       MouseArea {
            anchors.fill: parent
            onClicked:
              {
                webbrowserid.stop()
                main.reset_touche()
                badgewebkeys.badge_valide()
           }
           }

     Text {
          text:main.date
          anchors.top:parent.top
          anchors.topMargin:10
          anchors.horizontalCenter:parent.horizontalCenter
          color: "#888484"
          font.family: "Liberation Sans Narrow"
          font.italic: true
          font.pixelSize: main.orientation == 0 ? 25 : 23
          }

     Text {
        text:main.heure
        anchors.bottom:parent.bottom
        anchors.bottomMargin:00
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: "FreeMono"
        font.pixelSize: 35
        color:"#e89101"
        }
  }
}
