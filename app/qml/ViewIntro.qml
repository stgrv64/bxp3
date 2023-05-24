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
   Component.onCompleted: {logo_animation.start();playIntroid.source= "file:/var/x7/contents/m4a/intro.m4a";playIntroid.play()}
//   Component.onCompleted: {logo_animation.start()}
   property real zoomratiomax: main.orientation == 0 ? 3 : 1.5
   MouseArea {
        anchors.fill: parent
//        onPressed: {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
        onDoubleClicked: {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
        onPressAndHold:  {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
        }


   function animation_start() {logo_animation.start();}
   function animation_fin_start() {logo_animation_fin.start();}


/*   Timer {
            id: timer_fin
            interval: 1; repeat:false;
            onTriggered:{logo_animation_fin.running=true}
         }
*/
  SequentialAnimation {
        id:logo_animation
            PropertyAnimation  { target: logo; properties: "zoomRatio"; to: 0;easing.type: Easing.Linear;duration: 100;}
            PropertyAnimation  { target: logo; properties: "zoomRatio"; to: zoomratiomax;easing.type: Easing.Linear;duration: 2000;}
            PropertyAnimation  { target: logo; properties: "zoomRatio"; to: zoomratiomax;easing.type: Easing.Linear;duration: 500;}
          onStopped:logo_animation_fin.running=true
    }

   PropertyAnimation  { id:logo_animation_fin; target: logo; properties: "zoomRatio"; to: 0;easing.type: Easing.Linear; duration: 2000;onStopped:main.state="view1";}

   Audio{
       id: playIntroid
       onStopped:source=""

   }

   Image {
       id: logo
       property real zoomRatio
       //width:sourceSize.width*zoomRatio
       //height:sourceSize.height*zoomRatio
       width:sourceSize.width
       height:sourceSize.height
       anchors.right: parent.right
       anchors.top: parent.top
       anchors.topMargin: 100
       anchors.rightMargin:100
       opacity: 1
       source:"qrc:/qml/content/Logo_fond_blanc.jpg"
       MouseArea {
            anchors.fill: parent
    //        onPressed: {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
            onDoubleClicked: {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
            onPressAndHold:  {screen_touch();logo_animation.stop();logo_animation_fin.stop();main.signal_qml_ts_calibrate_init();}
            }
   }

 /*   Image {
           id: time2bii
           height:source.height
           width: source.width
           anchors.right: parent.right
           anchors.bottom: parent.bottom
           anchors.bottomMargin:  main.orientation == 0 ? 30 : 0
           anchors.rightMargin: main.orientation == 0 ? 20 : 5

           opacity: 1
           source: main.orientation == 0 ? "qrc:/qml/content/time2bii0.jpg" : "qrc:/qml/content/time2bii1.jpg"
            }



    Image {
      id:bulle
       anchors.bottom:parent.bottom
       anchors.left:parent.left
       anchors.leftMargin:5
       opacity: 1
       source: "qrc:/qml/content/bulles.png"

     }*/

}
