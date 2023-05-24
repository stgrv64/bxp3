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

Rectangle {
  anchors.fill: parent
  color: "#F0F0F0"
   MouseArea {
        anchors.fill: parent
        onClicked: {videoPlayerItem.stopVideo();screen_touch();main.state="view1";}
        }





   Image {
       id: setupi
       height:source.height
       width: source.width
       anchors.top:parent.top
       anchors.left:parent.left
       anchors.topMargin:10
       source :"qrc:/qml/content/time2bii_setup.png"
       MouseArea {
                  anchors.fill: parent
                  onClicked: {videoPlayerItem.stopVideo();screen_touch();console.log("start browser");browser("http://127.0.0.1/setup","","","600");}
                }

  }

   Image {
            id : audioi
            height:source.height
            width: source.width
            anchors.top:parent.top
            anchors.right:parent.right
            anchors.topMargin:10
            anchors.rightMargin:10
            source :"qrc:/qml/content/audio.png"
            MouseArea {
                       anchors.fill: parent
                       onDoubleClicked: {videoPlayerItem.stopVideo();screen_touch();audio_set_volume("0");audioi.source="qrc:/qml/content/audio_out.png";signal_audio_play("/var/x7/contents/wav/badge_ok.wav")}
                       onClicked: {videoPlayerItem.stopVideo();screen_touch();audio_set_volume("1");audioi.source="qrc:/qml/content/audio.png";signal_audio_play("/var/x7/contents/wav/badge_ok.wav")}
//                       onPressAndHold:  {videoPlayerItem.stopVideo();screen_touch();audio_set_volume("0");audioi.source="qrc:/qml/content/audio_out.png";badge_audio.source="file:/var/x7/contents/m4a/badge_ok.m4a";badge_audio.play()}
//                       onClicked: {videoPlayerItem.stopVideo();screen_touch();audio_set_volume("1");audioi.source="qrc:/qml/content/audio.png";badge_audio.source="file:/var/x7/contents/m4a/badge_ok.m4a";badge_audio.play()}
                       }
        }

 /*  Image {
              id : screen_orientationi
              height:source.height
              width: source.width
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.verticalCenter: parent.verticalCenter
              source :"qrc:/qml/content/screen_rotate.png"
              MouseArea {
                         anchors.fill: parent
                         onClicked:{screen_touch();signal_window_orientation_ecran();}
                        }

         }*/


   Image {
            id : time2bii_testsi
            height:source.height
            width: source.width
            anchors.bottom:parent.bottom
            anchors.left:parent.left
            anchors.bottomMargin:10
            anchors.leftMargin:10
            source :"qrc:/qml/content/time2bii_tests.png"
            MouseArea {
                       anchors.fill: parent
                       onClicked: {videoPlayerItem.stopVideo();screen_touch();main.state="view5";signal_time2bii_tests(999);}
                      }

       }





   /*Image {
            id : videoi
            height:source.height
            width: source.width
            anchors.bottom:parent.bottom
            anchors.right:parent.right
            anchors.rightMargin:10
            anchors.bottomMargin:10
            source :"qrc:/qml/content/video.png"
            MouseArea {
                       anchors.fill: parent
                       onClicked: {screen_touch();videoPlayerItem.playVideo();}
                      }

       }*/

   Audio
    {
        id:badge_audio
        onStopped:source="";
    }


Rectangle {
         id: videoPlayerItem
         width:800*0.5
         height:480*0.5
         anchors.top: parent.top
         anchors.right: parent.right
         anchors.topMargin: 10
         anchors.rightMargin: 10
         //main.orientation == "0" ? 10 : 20
         color: "transparent"
         enabled: false
         opacity: 0

        function playVideo(){
             videoPlayer.volume=1
             videoPlayer.source= "file:/var/x7/contents/mp4/gfi.mp4"
             videoPlayer.play()
             videoPlayerItem.opacity=1;
             videoPlayerItem.enabled = true;
              }

         function stopVideo(){
             videoPlayer.stop()
             videoPlayer.source="";
             videoPlayerItem.opacity=0
             videoPlayerItem.enabled = false
              }

         function pauseVideo(){
             videoPlayer.pause();

               }

      MouseArea{
             anchors.fill: parent
             onClicked:
                 videoPlayerItem.stopVideo()
                }


      Video{
          id:videoPlayer
          anchors.fill: videoPlayerItem // never forget to mention the size and position
          focus: true

          onStatusChanged: {
                 if(videoPlayer.status == Video.EndOfMedia)  videoPlayerItem.stopVideo()
             }
       }
    }

}
