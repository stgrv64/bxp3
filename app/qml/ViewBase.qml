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
    property real alerte_morpho_db_opacity:0
    property real alerte_morpho_db_enabled:0
    property real alerte_http_opacity:0
    property bool alerte_http_enabled:false
    property real alerte_badge_db_opacity:0
    property bool alerte_badge_db_enabled:false

    property string adresseip
    property bool adresseip_opacity:false
    property bool adresseip_enabled:false
    property real usbkey_opacity:0
    property bool usbkey_enabled:false

    property string acces_intranet_border_color:"transparent";
    property string acces_intranet_color:"#7e7a7a"


    MouseArea {
         anchors.fill: parent
         onClicked: {reset_touche()}
         }

    Image {
      id:background

       anchors.bottom:parent.bottom
       anchors.left:parent.left
       anchors.leftMargin:0
       opacity: 1
       source: background_file

       //source: main.orientation === 0 ? "file:/var/x7/contents/images/background_horizontal.img" : "file:/var/x7/contents/images/background_vertical.img"

    }

/*   Image {
      id:bullemirror

       anchors.bottom:parent.bottom
       anchors.right:parent.right
       anchors.rightMargin:5
       opacity: 1
       source: "qrc:/qml/content/bulles.png"
       mirror:true
       //rotation:180
       MouseArea {
           anchors.fill: parent
           onClicked: {reset_touche()}
           onPressAndHold: {reset_touche();logo.opacity=0.4;;launch_timeout.start()}
           }
    }

*/

    Rectangle {
        id: launch
        color:"transparent"
        opacity:1
        width:80
        height:60
         anchors.bottom:parent.bottom
         anchors.right:parent.right
         MouseArea {
             anchors.fill: parent
             onClicked: {reset_touche();logo.opacity=0.4;console.log("Clicked");launch_timeout.start()}
             //onClicked: {reset_touche();console.log("Clicked");}
             //onPressAndHold: {reset_touche();logo.opacity=0.4;console.log("PressAndHold");launch_timeout.start()}
             }
      }

     Timer {
            id:launch_timeout
            interval: 3000
            onTriggered: logo.opacity=1
        }

     Image {
        id: logo
        property string logo_path:"file:/var/x7/contents/images/"
        anchors.bottom:parent.bottom
        anchors.bottomMargin:3
         anchors.horizontalCenter:parent.horizontalCenter
         opacity: 1
         source: logo_path+"logo.img"
         onStatusChanged: {
                   if (logo.status == Image.Error) {
                          logo.source = "qrc:/qml/content/logo.png"
                       logo_gfi.opacity=0
                   }
               }

         MouseArea {
                 anchors.fill: parent
                 onClicked:
                 {
                  if (logo.opacity < 1.0) {logo.opacity=1;main.state ="view6"}
                  else {adresseip_opacity=true;adresseip_enabled=true}
                 }
                 onPressAndHold:
                    {
                     if (logo.opacity < 1.0) {logo.opacity=1;main.state ="view6"}
                     else {adresseip_opacity=true;adresseip_enabled=true}
                    }

                }

       }


     Text {
         id: adresseipi
         anchors.bottom:parent.bottom
         anchors.bottomMargin:5
         anchors.right:logo.left
         anchors.rightMargin:10
          opacity: adresseip_opacity
          enabled: adresseip_enabled
          text:"@: "+adresseip
          MouseArea {
              anchors.fill: parent
              onClicked: {adresseip_opacity:0;adresseip_enabled:0}
              onPressed: {adresseip_opacity:0;adresseip_enabled:0}
              }
         }


     Image {
         id: alerte_http
         anchors.bottom:parent.bottom
         anchors.bottomMargin:5
         anchors.left:logo.right
         anchors.leftMargin:10
          opacity: alerte_http_opacity
          enabled: alerte_http_enabled
          source:"qrc:/qml/content/alerte_http.png"
          MouseArea {
              anchors.fill: parent
              onClicked: {reset_touche()}
              onPressed: {reset_touche()}
              }
         }

     Image {
         id: alerte_badge_db
         anchors.bottom:parent.bottom
         anchors.bottomMargin:5
         anchors.left:logo.right
         anchors.leftMargin:10
          opacity: alerte_badge_db_opacity
          enabled: alerte_badge_db_enabled
          source:"qrc:/qml/content/alerte_badge_db.png"
          MouseArea {
              anchors.fill: parent
              onClicked: {reset_touche()}
              onPressed: {reset_touche()}
              }
        }

     Image {
         id: alerte_morpho_db
         anchors.bottom:parent.bottom
         anchors.bottomMargin:5
         anchors.left:logo.right
         anchors.leftMargin:10
          opacity: alerte_morpho_db_opacity
          enabled: alerte_morpho_db_enabled
          source:"qrc:/qml/content/alerte_morpho_db.png"
          MouseArea {
              anchors.fill: parent
              onClicked: {reset_touche()}
              onPressed: {reset_touche()}
              }
        }


     Image {
         id: usbkey
         anchors.bottom:parent.bottom
         anchors.bottomMargin:5
         anchors.left:logo.right
         anchors.leftMargin:10
         opacity: usbkey_opacity
         enabled: usbkey_enabled
         source:"qrc:/qml/content/usbkey.jpg"
       }




     Image {
        id: logo_gfi
        width:sourceSize.width
        height:sourceSize.height
        anchors.bottom:parent.bottom
        anchors.left:parent.left
        anchors.bottomMargin:5
        anchors.leftMargin:10
        opacity: 1
        source : "qrc:/qml/content/logo_mini.png"
        MouseArea {
                anchors.fill: parent
                onClicked:{reset_touche()}
                }
        }

    Rectangle {
         id:horodate1
         width: main.orientation == 0 ? 800 : 480
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.top:parent.top
         height:main.orientation == 0 ? 40 : 70
         color: "transparent"
         MouseArea {
            anchors.fill: parent
            //onClicked:  main.reset_touche();
            onClicked: {
                  reset_touche()
                  if (logo.opacity < 1) {logo.opacity=1;main.state ="view8"};
                  viewid9_timer.start();
                  //Ecran config avance
              }
             }


         Text {
              id:date_text
              text:main.orientation == 0 ? main.date : main.date_sans_année
              anchors.top:parent.top
              anchors.topMargin:5
              anchors.right: parent.right
              anchors.rightMargin:main.orientation == 0 ? 10: 5
              color: "white"

              font.family: "Liberation Sans Narrow"
              font.capitalization: Font.Capitalize
              font.italic: true

              font.pixelSize: main.orientation == 0 ? 25 : 20
          }

         Text {
              text:main.orientation == 0 ? "" : main.année
              anchors.top:date_text.bottom
              anchors.topMargin:5
              anchors.right: parent.right
              anchors.rightMargin:main.orientation == 0 ? 10: 5
              color: "white"
              font.family: "Liberation Sans Narrow"
              font.italic: true
              font.pixelSize: main.orientation == 0 ? 25 : 20
              }

         Text {
            text:main.heure
            anchors.top:parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin:main.orientation == 0 ? 20:5
            font.family: "FreeMono"
            font.bold: true
            font.pixelSize: 40
            //color:"#e86500" gfi
            color : "white"
         }
    }

 }



