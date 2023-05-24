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
        id:viewwifii
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
        */
        property string ssid:viewwifi_ssid
        property string psk:viewwifi_psk

        Timer {
              id: timeri
                  interval: 1; repeat:false;running:false
                  onTriggered:
                  {
                    signal_X7GestConfig_setWifi("init","")
                    webbrowser_setup.stop()
                    ssid = viewwifi_ssid
                    psk = viewwifi_psk
                  }

               }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_wifi.opacity=0;
           r0_text.state ='Off';r1_text.state ='Off';
            keyboard_num.text=""; keyboard_alpha.text="";keyboard_alpha.opacity=0;keyboard_num.opacity=0;
        }


        function mode_wifi()
        {
            webbrowser_setup.stop()
            if ( r0_text.state =='On')  {r0_text.state ='Off';ssid=keyboard_alpha.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';psk=keyboard_alpha.text}
            signal_X7GestConfig_setWifi(ssid, psk)
            timeri.start()
            viewwifii.clear();
        }
        function viewid9_timer_restart() {
            viewid9_timer.stop();
            viewid9_timer.start();
        }

       Column {
          spacing: 15
          Row {
              spacing: 10
          }
          Row {
              spacing: 20


               CLButton{
                   id: wifi
                   text:qsTr(" WIFI ")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer_restart();
                    if (ok_wifi.opacity ==0)
                        {
                        ok_wifi.opacity=1
                        timeri.start();
                       }
                    else
                        {
                         clear()
                      }
                    }
                 }

                Image {
                       id:ok_wifi
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_wifi();viewid9_timer_restart();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_wifi.opacity
               Text{
                    id:r0_text
                    text:qsTr("SSID :")
                    state:"Off"
                    color: r0_text.state =='Off' ? "blue" : "#e89101"
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 30

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {viewid9_timer.stop();
                            if ( r0_text.state =='On')
                                {
                                 r0_text.state ='Off';
                                 ssid=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r1_text.state =='On')  {r1_text.state ='Off';psk=keyboard_alpha.text}
                                r0_text.state ='On';
                                keyboard_alpha.maxlength=50
                                keyboard_alpha.text=ssid
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r0_text.state =='Off' ? ssid : keyboard_alpha.text
                    maximumLength:50
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r0_text.state =='On'? true:false
                    }
              }

          Row {
              spacing: 20
              opacity:ok_wifi.opacity
               Text{
                    id:r1_text
                    text:qsTr("PSK :")
                    state:"Off"
                    color: r1_text.state =='Off' ? "blue" : "#e89101"
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 25

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {viewid9_timer.stop();
                            if ( r1_text.state =='On')
                                {
                                 r1_text.state ='Off';
                                 psk=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';ssid=keyboard_alpha.text}
                                r1_text.state ='On';
                                keyboard_alpha.maxlength=63
                                keyboard_alpha.text=psk
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r1_text.state =='Off' ? psk : keyboard_alpha.text
                    maximumLength:63
                    font.family: "Helvetica"
                    font.pixelSize: 20
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false
                    }
              }

            }

 }

