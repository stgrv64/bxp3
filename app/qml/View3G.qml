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
        id:view3Gi
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
*/
        property string apn:view3G_apn
        property string pin:view3G_pin
        property string phone:view3G_phone
        property string user:view3G_user
        property string password:view3G_password
        property string opt1:view3G_opt1
        property string opt2:view3G_opt2


        Timer {
              id: timeri
                  interval: 1000; repeat:false;running:false
                  onTriggered:{
                      //Cette blague ce signal est une folie
                      //utilise pour initialiser la config ...
                      signal_process_set_3G("init","","","","");
                      webbrowser_setup.stop()
                      apn = view3G_apn
                      pin = view3G_pin
                      phone = view3G_phone
                      user = view3G_user
                      password = view3G_password
                      opt1 = view3G_opt1
                      opt2 = view3G_opt2

                  }
        }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_3Gi.opacity=0;
           r0_text.state ='Off';r1_text.state ='Off';r2_text.state ='Off';r3_text.state ='Off';r4_text.state ='Off'
           keyboard_num.text=""; keyboard_alpha.text=""; keyboard_num.opacity=0;keyboard_alpha.opacity=0;
        }


        function mode_3G()
        {
            webbrowser_setup.stop();
            if ( r0_text.state =='On')  {r0_text.state ='Off';apn=keyboard_alpha.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';pin=keyboard_num.text}
            if ( r2_text.state =='On')  {r2_text.state ='Off';phone=keyboard_alpha.text}
            if ( r3_text.state =='On')  {r3_text.state ='Off';user=keyboard_alpha.text}
            if ( r4_text.state =='On')  {r4_text.state ='Off';password=keyboard_alpha.text}
            signal_process_set_3G(apn,pin,phone,user,password)
            timeri.start()
            view3Gi.clear();
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
                   id: s3Gi
                   text: qsTr("  3G  ")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer_restart();
                    if (ok_3Gi.opacity ==0)
                        {
                        ok_3Gi.opacity=1
                        webbrowser_setup.stop();
                       }
                    else
                        {
                         clear()
                       }
                    }
                 }

                Image {
                       id:ok_3Gi
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_3G();viewid9_timer_restart();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_3Gi.opacity
               Text{
                    id:r0_text
                    text:qsTr("Apn      : ")
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
                                 apn=keyboard_alpha.text;
                                 }
                            else
                                {
                                if ( r1_text.state =='On')  {r1_text.state ='Off';pin=keyboard_num.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';phone=keyboard_alpha.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';user=keyboard_alpha.text}
                                if ( r4_text.state =='On')  {r4_text.state ='Off';password=keyboard_alpha.text}
                                 r0_text.state ='On';
                                keyboard_alpha.maxlength=20
                                keyboard_alpha.text=apn
                                keyboard_alpha.opacity=1;
                                keyboard_num.opacity=0
                                keyboard_alpha.enabled=1;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r0_text.state =='Off' ? apn : keyboard_alpha.text
                    maximumLength:20
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r0_text.state =='On'? true:false
                    }
              }

          Row {
              spacing: 20
              opacity: ok_3Gi.opacity
               Text{
                    id:r1_text
                    text:qsTr("Pin      : ")
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
                                 pin=keyboard_num.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';apn=keyboard_alpha.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';phone=keyboard_alpha.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';user=keyboard_alpha.text}
                                if ( r4_text.state =='On')  {r4_text.state ='Off';password=keyboard_alpha.text}
                                r1_text.state ='On';
                                keyboard_num.maxlength=4
                                keyboard_num.text=pin
                                keyboard_alpha.opacity=0;
                                keyboard_num.opacity=1
                                keyboard_alpha.enabled=0;
                                keyboard_num.enabled=1
                                }

                           }
                        }
                    }

                 TextInput {
                    text: r1_text.state =='Off' ? pin : keyboard_num.text
                    maximumLength:4
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false

                  }

                }



                Row {
                    spacing: 20
                    opacity: ok_3Gi.opacity

                      Text{
                          id:r2_text
                          text:qsTr("Phone    : ")
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
                                       phone=keyboard_alpha.text;
                                      }
                                  else
                                      {
                                      if ( r0_text.state =='On')  {r0_text.state ='Off';apn=keyboard_alpha.text}
                                      if ( r1_text.state =='On')  {r1_text.state ='Off';pin=keyboard_num.text}
                                      if ( r3_text.state =='On')  {r3_text.state ='Off';user=keyboard_alpha.text}
                                      if ( r4_text.state =='On')  {r4_text.state ='Off';password=keyboard_alpha.text}
                                       r2_text.state ='On';
                                      keyboard_num.maxlength=10
                                      keyboard_alpha.text=phone
                                      keyboard_alpha.opacity=1;
                                      keyboard_num.opacity=0
                                      keyboard_alpha.enabled=1;
                                      keyboard_num.enabled=0
                                      }

                                 }
                            }
                         }

                      TextInput {
                         text: r2_text.state =='Off' ? phone : keyboard_alpha.text
                         maximumLength:10
                         font.family: "Helvetica"
                         font.pixelSize: 30
                         color: "black"
                         cursorVisible: r2_text.state =='On'? true:false
                         }

                     }

                Row {
                      spacing: 20
                      opacity:ok_3Gi.opacity
                        Text{
                            id:r3_text
                            text:qsTr("User    : ")
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
                                         user=keyboard_alpha.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';apn=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';pin=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';phone=keyboard_alpha.text}
                                       if ( r4_text.state =='On')  {r4_text.state ='Off';password=keyboard_alpha.text}
                                         r3_text.state ='On';
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
                        text: r3_text.state =='Off' ? user : keyboard_alpha.text
                        maximumLength:10
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r3_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_3Gi.opacity
                        Text{
                            id:r4_text
                            text:qsTr("Password: ")
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
                                         password=keyboard_alpha.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';apn=keyboard_alpha.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';pin=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';phone=keyboard_alpha.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';user=keyboard_alpha.text}
                                        r4_text.state ='On';
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
                        text: r4_text.state =='Off' ? password : keyboard_alpha.text
                        maximumLength:10
                        font.family: "Helvetica"
                        font.pixelSize: 30
                        color: "black"
                        cursorVisible: r4_text.state =='On'? true:false
                       }

             }


            }

}

