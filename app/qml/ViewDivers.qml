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
        id:viewdiversi
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
*/
        property string ecran:viewdivers_ecran
        property string bl1:viewdivers_bl1
        property string delai:viewdivers_delai
        property string bl2:viewdivers_bl2
        property string delai_off:viewdivers_delai_off
        property string sleep_mode:viewdivers_sleep_mode


        Timer {
              id: timeri
                  interval: 1000; repeat:false;running:false
                  onTriggered:{
                      signal_window_set_proxy_web("");
                      signal_ihm_process_set_divers("init","","","","","");
                      webbrowser_setup.run("http://127.0.0.1/setup/divers");
                      ecran = viewdivers_ecran
                      bl1 = viewdivers_bl1
                      delai = viewdivers_delai
                      bl2 = viewdivers_bl2
                      delai_off = viewdivers_delai_off
                      sleep_mode = viewdivers_sleep_mode
                                }
               }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_diversi.opacity=0
           r0_text.state ='Off';r1_text.state ='Off';
            //r2_text.state ='Off';r3_text.state ='Off';r4_text.state ='Off';r5_text.state ='Off';
            keyboard_num.text=""; keyboard_num.text=""; keyboard_alpha.opacity=0;keyboard_num.opacity=0;
        }


        function mode_divers()
        {
            webbrowser_setup.stop();
           if ( r0_text.state =='On')   {r0_text.state ='Off';ecran=keyboard_num.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
           /* if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
            if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
            if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
            if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}*/
            signal_ihm_process_set_divers(ecran,bl1,delai,bl2,delai_off,sleep_mode)
            timeri.start()
            viewdiversi.clear();
        }


       Column {
          spacing: 15
          Row {
              spacing: 10
          }
          Row {
              spacing: 20


               CLButton{
                   id: diversi
                   text: qsTr(" DIVERS ")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer.stop();
                    if (ok_diversi.opacity ==0)
                        {
                        ok_diversi.opacity=1
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                       }
                    else
                        {
                         clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/divers");
                       }
                    }
                 }

                Image {
                       id:ok_diversi
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_divers();viewid9_timer.stop();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_diversi.opacity
               Text{
                    id:r0_text
                    text:qsTr("Position écran :")
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
                                ecran=keyboard_num.text;
                                }
                           else
                               {
                               if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
                               /*if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
                               if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
                               if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
                               if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}*/
                               r0_text.state ='On';
                               keyboard_num.maxlength=2
                               keyboard_num.text=ecran
                               keyboard_num.opacity=1;
                               keyboard_alpha.opacity=0;
                               keyboard_alpha.enabled=0;
                               keyboard_num.enabled=1
                               }
                          }
                       }
                    }

            TextInput {
               text: r0_text.state =='Off' ? ecran : keyboard_num.text
               maximumLength:2
               font.family: "Helvetica"
               font.pixelSize: 30
               color: "black"
               cursorVisible: r0_text.state =='On'? true:false
               }
             }

          Row {
              spacing: 20
              opacity: ok_diversi.opacity
               Text{
                    id:r1_text
                    text:qsTr("Rétro éclairage (1-25) : ")
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
                                 bl1=keyboard_num.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';ecran=keyboard_num.text}
                                /*if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
                                if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
                                if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}*/
                                r1_text.state ='On';
                                keyboard_num.maxlength=3
                                keyboard_num.text=bl1
                                keyboard_num.opacity=1;
                                keyboard_alpha.opacity=0;
                                keyboard_alpha.enabled=0;
                                keyboard_num.enabled=1

                                }

                           }
                        }
                    }

                 TextInput {
                    text: r1_text.state =='Off' ? bl1 : keyboard_num.text
                    maximumLength:2
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false

                  }

                }


/*
                Row {
                    spacing: 20
                    opacity: ok_diversi.opacity

                      Text{
                          id:r2_text
                          text:qsTr("Delai activation backlight N2 (en s) : ")
                          state:"Off"
                          color: r2_text.state =='Off' ? "blue" : "#e89101"
                          width:text.width
                          font.family: "Helvetica"
                          font.pixelSize: 50

                          MouseArea {
                              anchors.fill: parent
                              onClicked:
                                  {
                                  if ( r2_text.state =='On')
                                      {
                                       r2_text.state ='Off';
                                       delai=keyboard_num.text;
                                      }
                                  else
                                      {
                                      if ( r0_text.state =='On')  {r0_text.state ='Off';ecran=keyboard_num.text}
                                      if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
                                      if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
                                      if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
                                      if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}
                                      r2_text.state ='On';
                                      keyboard_num.maxlength=4
                                      keyboard_num.text=delai
                                      keyboard_num.opacity=1;
                                      keyboard_alpha.opacity=0;
                                      }

                                 }
                            }
                         }

                      TextInput {
                         text: r2_text.state =='Off' ? delai : keyboard_num.text
                         maximumLength:4
                         font.family: "Helvetica"
                         font.pixelSize: 50
                         color: "black"
                         cursorVisible: r2_text.state =='On'? true:false
                         }

                     }

                Row {
                      spacing: 20
                      opacity:ok_diversi.opacity
                        Text{
                            id:r3_text
                            text:qsTr("% backlight N2 : ")
                            state:"Off"
                            color: r3_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 50

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {
                                    if ( r3_text.state =='On')
                                        {
                                         r3_text.state ='Off';
                                         bl2=keyboard_num.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';ecran=keyboard_num.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
                                        if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
                                        if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}
                                        r3_text.state ='On';
                                        keyboard_num.maxlength=3
                                        keyboard_num.text=bl2
                                        keyboard_num.opacity=1;
                                        keyboard_alpha.opacity=0;
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r3_text.state =='Off' ? bl2 : keyboard_num.text
                        maximumLength:2
                        font.family: "Helvetica"
                        font.pixelSize: 50
                        color: "black"
                        cursorVisible: r3_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_diversi.opacity
                        Text{
                            id:r4_text
                            text:qsTr("Delai extinction backlight (en s) : ")
                            state:"Off"
                            color: r4_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 50

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {
                                    if ( r4_text.state =='On')
                                        {
                                         r4_text.state ='Off';
                                         delai_off=keyboard_num.text;
                                        }
                                    else
                                        {
                                        if ( r0_text.state =='On')  {r0_text.state ='Off';ecran=keyboard_num.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
                                        if ( r5_text.state =='On')  {r5_text.state ='Off';sleep_mode=keyboard_num.text}
                                       r4_text.state ='On';
                                        keyboard_num.maxlength=4
                                        keyboard_num.text=delai_off
                                        keyboard_num.opacity=1;
                                        keyboard_alpha.opacity=0;
                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r4_text.state =='Off' ? delai_off : keyboard_num.text
                        maximumLength:4
                        font.family: "Helvetica"
                        font.pixelSize: 50
                        color: "black"
                        cursorVisible: r4_text.state =='On'? true:false
                       }

             }
                Row {
                      spacing: 20
                      opacity:ok_diversi.opacity
                        Text{
                            id:r5_text
                            text:qsTr("Sleep mode autorisé ? (0/1) :")
                            state:"Off"
                            color: r5_text.state =='Off' ? "blue" : "#e89101"
                            width:text.width
                            font.family: "Helvetica"
                            font.pixelSize: 50

                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                    {
                                    if ( r5_text.state =='On')
                                        {
                                         r5_text.state ='Off';
                                         sleep_mode=keyboard_num.text;
                                        }
                                    else
                                        {
                                       if ( r0_text.state =='On')  {r0_text.state ='Off';ecran=keyboard_num.text}
                                        if ( r1_text.state =='On')  {r1_text.state ='Off';bl1=keyboard_num.text}
                                        if ( r2_text.state =='On')  {r2_text.state ='Off';delai=keyboard_num.text}
                                        if ( r3_text.state =='On')  {r3_text.state ='Off';bl2=keyboard_num.text}
                                        if ( r4_text.state =='On')  {r4_text.state ='Off';delai_off=keyboard_num.text}
                                        r5_text.state ='On';
                                        keyboard_num.maxlength=1
                                        keyboard_num.text=sleep_mode
                                        keyboard_num.opacity=1;
                                        keyboard_alpha.opacity=0;

                                        }

                                   }
                            }
                        }

                    TextInput {
                        text: r5_text.state =='Off' ? sleep_mode : keyboard_num.text
                        maximumLength:1
                        font.family: "Helvetica"
                        font.pixelSize: 50
                        color: "black"
                        cursorVisible: r5_text.state =='On'? true:false
                       }

             }*/

            }



}

