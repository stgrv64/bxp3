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
        id:viewhttpi
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
*/
        property string serveur:viewhttp_serveur
        property string https:viewhttp_https
        property string http:viewhttp_http
        property string service:viewhttp_service
        property string user:viewhttp_user
        property string password:viewhttp_password
        property string timeout:viewhttp_timeout
        property  int kbnum:0
        property  int kbalpha:0


        Timer {
              id: timeri
                  interval: 1000; repeat:false;running:false
                  onTriggered:{
                      //set_proxy_web("");
                      signal_ihm_process_set_http("init","","","","","","");
                      webbrowser_setup.run("http://127.0.0.1/setup/http");
                      serveur = viewhttp_serveur
                      https = viewhttp_https
                      http = viewhttp_http
                      service = viewhttp_service
                      user = viewhttp_user
                      password = viewhttp_password
                      timeout = viewhttp_timeout
                      }
               }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_httpi.opacity=0;
           r0_text.state ='Off';r1_text.state ='Off';r2_text.state ='Off';r3_text.state ='Off';r4_text.state ='Off';r5_text.state ='Off';r6_text.state ='Off';
           keyboard_num.text=""; keyboard_alpha.text=""; keyboard_num.opacity=0;keyboard_alpha.opacity=0;
        }


        function mode_http()
        {
            webbrowser_setup.stop();
            if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_num.text}
            if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_num.text}
            if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
            if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
            if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
            if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
            signal_ihm_process_set_http(serveur,https,http,service,user,password,timeout)
            timeri.start()
            viewhttpi.clear();
        }


       Column {
          spacing: 15
          Row {
              spacing: 10
          }
          Row {
              spacing: 20


               CLButton{
                   id: httpi
                   text: qsTr(" SERVEUR HTTP ")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer.stop();
                    if (ok_httpi.opacity ==0)
                        {
                        ok_httpi.opacity=1
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                       }
                    else
                        {
                         clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/http");
                       }
                    }
                 }

                Image {
                       id:ok_httpi
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_http();viewid9_timer.stop();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_httpi.opacity
               Text{
                    id:r0_text
                    text:qsTr("Serveur :")
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
                                 r0.text.state ='Off';
                                 serveur=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_num.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_num.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
                                if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                r0_text.state ='On';
                                keyboard_alpha.maxlength=20
                                keyboard_alpha.text=serveur
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0
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
              opacity: ok_httpi.opacity
               Text{
                    id:r1_text
                    text:qsTr("Port Https : ")
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
                                 https=keyboard_num.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_num.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
                                if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                r1_text.state ='On';
                                keyboard_num.maxlength=5
                                keyboard_num.text=https
                                keyboard_alpha.opacity=0;
                                keyboard_num.opacity=1
                                keyboard_alpha.enabled=0;
                                keyboard_num.enabled=1
                                }

                           }
                        }
                    }

                 TextInput {
                    text: r1_text.state =='Off' ? https : keyboard_num.text
                    maximumLength:5
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false

                  }

                }



                Row {
                    spacing: 20
                    opacity: ok_httpi.opacity

                      Text{
                          id:r2_text
                          text:qsTr("Port Http : ")
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
                                       http=keyboard_num.text;
                                      }
                                  else
                                      {
                                      if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                      if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_num.text}
                                      if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_num.text}
                                      if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                      if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                      if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                     r2_text.state ='On';
                                      keyboard_num.maxlength=5
                                      keyboard_num.text=http
                                      keyboard_alpha.opacity=0;
                                      keyboard_num.opacity=1
                                      keyboard_alpha.enabled=0;
                                      keyboard_num.enabled=1
                                      }

                                 }
                            }
                         }

                      TextInput {
                         text: r2_text.state =='Off' ? http : keyboard_num.text
                         maximumLength:5
                         font.family: "Helvetica"
                         font.pixelSize: 30
                         color: "black"
                         cursorVisible: r2_text.state =='On'? true:false
                         }

                     }

                Row {
                      spacing: 20
                      opacity:ok_httpi.opacity
                        Text{
                            id:r3_text
                            text:qsTr("Service :")
                            state:"Off"
                            color: r3_text.state =='Off' ? "blue" : "#e89101"
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
                                         service=keyboard_alpha.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_num.text}
                                        if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                        if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                        if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                        r3_text.state ='On';
                                        keyboard_alpha.maxlength=50
                                        keyboard_alpha.text=service
                                        keyboard_alpha.opacity=1;
                                        keyboard_num.opacity=0
                                        keyboard_alpha.enabled=1;
                                        keyboard_num.enabled=0
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r3_text.state =='Off' ? service : keyboard_alpha.text
                        maximumLength:50
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r3_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_httpi.opacity
                        Text{
                            id:r4_text
                            text:qsTr("User :")
                            state:"Off"
                            color: r4_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 30

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {viewid9_timer.stop();
                                    if ( r4_text.state =='On')
                                        {
                                         r4_text.state ='Off';
                                         user=keyboard_alpha.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_num.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
                                        if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                        if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                        r4_text.state ='On';
                                        keyboard_alpha.maxlength=10
                                        keyboard_alpha.text=user
                                        keyboard_alpha.opacity=1;
                                        keyboard_num.opacity=0
                                        keyboard_alpha.enabled=1;
                                        keyboard_num.enabled=0
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r4_text.state =='Off' ? user : keyboard_alpha.text
                        maximumLength:10
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r4_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_httpi.opacity
                        Text{
                            id:r5_text
                            text:qsTr("Password :")
                            state:"Off"
                            color: r5_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 30

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {viewid9_timer.stop();
                                    if ( r5_text.state =='On')
                                        {
                                         r5_text.state ='Off';
                                         password=keyboard_alpha.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_alpha.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_alpha.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
                                        if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                        if ( r6_text.state =='On')  {r6_text.state ='Off';timeout=keyboard_num.text}
                                        r5_text.state ='On';
                                        keyboard_alpha.maxlength=10
                                        keyboard_alpha.text=password
                                        keyboard_alpha.opacity=1;
                                        keyboard_num.opacity=0
                                        keyboard_alpha.enabled=1;
                                        keyboard_num.enabled=0
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r5_text.state =='Off' ? password : keyboard_alpha.text
                        maximumLength:10
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r5_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_httpi.opacity
                        Text{
                            id:r6_text
                            text:qsTr("Timeout :")
                            state:"Off"
                            color: r6_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 30

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {viewid9_timer.stop();
                                    if ( r6_text.state =='On')
                                        {
                                         r6_text.state ='Off';
                                         timeout=keyboard_num.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';serveur=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';https=keyboard_alpha.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';http=keyboard_alpha.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';service=keyboard_alpha.text}
                                        if ( r4_text.state =='On')  {r4_text.state ='Off';user=keyboard_alpha.text}
                                        if ( r5_text.state =='On')  {r5_text.state ='Off';password=keyboard_alpha.text}
                                        r6_text.state ='On';
                                        keyboard_num.maxlength=2
                                        keyboard_num.text=timeout
                                        keyboard_alpha.opacity=0;
                                        keyboard_num.opacity=1
                                        keyboard_alpha.enabled=0;
                                        keyboard_num.enabled=1
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r6_text.state =='Off' ? timeout : keyboard_num.text
                        maximumLength:2
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r6_text.state =='On'? true:false
                       }

             }


            }

/*      Numerickeyboard_alpha {
          id:keyboard_num
          opacity: ok_httpi.opacity & kbnum
          anchors.bottom: parent.bottom
          anchors.right: parent.right
          anchors.bottomMargin: 10
          anchors.rightMargin: 10
         }


          Alphakeyboard_alpha {
             id:keyboard_alpha
             opacity: ok_httpi.opacity & kbalpha
             anchors.bottom: parent.bottom
             anchors.right: parent.right
             anchors.bottomMargin: 2
             anchors.rightMargin: 0
             width:480
             height:195
             RegExpValidator {
                 id:s0
                 regExp:/[6-9.]+/
              }
             RegExpValidator {
                 id:s1
                 regExp:/[0-9]+/
              }

             onClicked:
                {
             }

            }
*/
}

