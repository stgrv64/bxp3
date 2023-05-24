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
        id:viewntpi
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
        */
        property string serveur:viewntp_serveur
        property string cadence:viewntp_cadence
        property string timezone:viewntp_timezone
        property string langue:viewntp_langue


        Timer {
              id: timeri
                  interval: 1000; repeat:false;running:false
                  onTriggered:{ signal_window_set_proxy_web("");
                      signal_ihm_process_set_ntp("init","","","");
                      webbrowser_setup.run("http://127.0.0.1/setup/ntp");
                      serveur = viewntp_serveur
                      cadence = viewntp_cadence
                      timezone = viewntp_timezone
                      langue = viewntp_langue
                                }
               }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_ntp.opacity=0;
           r0_text.state ='Off';r1_text.state ='Off';;r2_text.state ='Off';;r3_text.state ='Off';
           keyboard_num.text=""; keyboard_alpha.text="";keyboard_alpha.opacity=0;keyboard_num.opacity=0;
        }


        function mode_ntp()
        {
            webbrowser_setup.stop();
            if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';cadence=keyboard_alpha.text}
            if ( r2_text.state =='On')  {r2_text.state ='Off';timezone=keyboard_alpha.text}
            if ( r3_text.state =='On')  {r3_text.state ='Off';langue=keyboard_alpha.text}
            signal_ihm_process_set_ntp(serveur,cadence,timezone,langue)
            timeri.start()
            viewntpi.clear();
        }


       Column {
          spacing: 15
          Row {
              spacing: 10
          }
          Row {
              spacing: 20


               CLButton{
                   id: ntp
                   text: qsTr("SERVEUR TEMPS")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer.stop();
                    if (ok_ntp.opacity ==0)
                        {
                        ok_ntp.opacity=1
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                       }
                    else
                        {
                         clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/ntp");
                       }
                    }
                 }

                Image {
                       id:ok_ntp
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_ntp();viewid9_timer.stop();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_ntp.opacity
               Text{
                    id:r0_text
                    text:qsTr("Nom ou Adresse IP :")
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
                                 serveur=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r1_text.state =='On')  {r1_text.state ='Off';cadence=keyboard_alpha.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';timezone=keyboard_alpha.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';langue=keyboard_alpha.text}
                                r0_text.state ='On';
                                keyboard_alpha.maxlength=20
                                keyboard_alpha.text=serveur
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                            }
                           }
                        }
                    }

                TextInput {
                    text: r0_text.state =='Off' ? serveur : keyboard_alpha.text
                    maximumLength:20
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r0_text.state =='On'? true:false
                    }
              }


          Row {
              spacing: 20
              opacity:ok_ntp.opacity
               Text{
                    id:r1_text
                    text:qsTr("Cadence demande heure :")
                    state:"Off"
                    color: r1_text.state =='Off' ? "blue" : "#e89101"
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 30

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {viewid9_timer.stop();
                            if ( r1_text.state =='On')
                                {
                                 r1_text.state ='Off';
                                 cadence=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';timezone=keyboard_alpha.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';langue=keyboard_alpha.text}
                                r1_text.state ='On';
                                keyboard_alpha.maxlength=5
                                keyboard_alpha.text=cadence
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r1_text.state =='Off' ? cadence : keyboard_alpha.text
                    maximumLength:5
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false
                    }
              }



          Row {
              spacing: 20
              opacity:ok_ntp.opacity
               Text{
                    id:r2_text
                    text:qsTr("Timezone :")
                    state:"Off"
                    color: r2_text.state =='Off' ? "blue" : "#e89101"
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 30

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {viewid9_timer.stop();
                            if ( r2_text.state =='On')
                                {
                                 r2_text.state ='Off';
                                 timezone=keyboard_alpha.text;
                                 }
                            else
                                {
                               if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                               if ( r1_text.state =='On')  {r1_text.state ='Off';cadence=keyboard_alpha.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';langue=keyboard_alpha.text}
                                r2_text.state ='On';
                                keyboard_alpha.maxlength=60
                                keyboard_alpha.text=timezone
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r2_text.state =='Off' ? timezone : keyboard_alpha.text
                    maximumLength:60
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r2_text.state =='On'? true:false
                    }
              }
          Row {
              spacing: 20
              opacity:ok_ntp.opacity
               Text{
                    id:r3_text
                    text:qsTr("Langue :")
                    state:"Off"
                    color: r1_text.state =='Off' ? "blue" : "#e89101"
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 30

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {viewid9_timer.stop();
                            if ( r3_text.state =='On')
                                {
                                 r3_text.state ='Off';
                                 langue=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                if ( r1_text.state =='On')  {r1_text.state ='Off';cadence=keyboard_alpha.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';timezone=keyboard_alpha.text}
                                r3_text.state ='On';
                                keyboard_alpha.maxlength=2
                                keyboard_alpha.text=langue
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0;
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r3_text.state =='Off' ? langue : keyboard_alpha.text
                    maximumLength:2
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r3_text.state =='On'? true:false
                    }
              }



            }



/*        Alphakeyboard_alpha {
             id:keyboard_alpha
             opacity: ok_ntp.opacity
             anchors.bottom: parent.bottom
             anchors.right: parent.right
             anchors.bottomMargin: 10
             anchors.rightMargin: 10
             width:480
             height:195

            }
*/
}

