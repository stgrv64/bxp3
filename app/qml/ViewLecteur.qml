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
        id:viewlecteuri
        width:800
        height:480
/*        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.topMargin: 20
        anchors.left:parent.left
        anchors.right:parent.right
        */
        property string lecteur:viewlecteur_lecteur
        property string option:viewlecteur_option
        property string masque:viewlecteur_masque
        property string format:viewlecteur_format


        Timer {
              id: timeri
                  interval: 1000; repeat:false;running:false
                  onTriggered:{
                      signal_window_set_proxy_web("");
                      signal_ihm_process_set_lecteur_badge("init","","","");
                      webbrowser_setup.run("http://127.0.0.1/setup/lecteur");
                      lecteur = viewlecteur_lecteur
                      option = viewlecteur_option
                      masque = viewlecteur_masque
                      format = viewlecteur_format
                                }
               }

        function timer_start() {timeri.start()}

        function clear()
        {
           ok_badge.opacity=0;
           r0_text.state ='Off';r1_text.state ='Off';r2_text.state ='Off';r3_text.state ='Off';
            keyboard_lecteur.text="";keyboard_num.text=""; keyboard_alpha.text="";keyboard_alpha.opacity=0;keyboard_lecteur.opacity=0;keyboard_num.opacity=0;
        }


        function mode_badge()
        {
            webbrowser_setup.stop();
            if ( r0_text.state =='On')  {r0_text.state ='Off';lecteur=keyboard_lecteur.text}
            if ( r1_text.state =='On')  {r1_text.state ='Off';option=keyboard_lecteur.text}
            if ( r2_text.state =='On')  {r2_text.state ='Off';masque=keyboard_lecteur.text}
            if ( r3_text.state =='On')  {r3_text.state ='Off';format=keyboard_lecteur.text}
            signal_ihm_process_set_lecteur_badge(lecteur,option,masque,format)
            timeri.start()
            viewlecteuri.clear();
        }


       Column {
          spacing: 15
          Row {
              spacing: 10
          }
          Row {
              spacing: 20


               CLButton{
                   id: badge
                   text: qsTr(" LECTEUR BADGE ")
                   fontSize: 20
                   onClicked:
                   {viewid9_timer.stop();
                    if (ok_badge.opacity ==0)
                        {
                        ok_badge.opacity=1
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                       }
                    else
                        {
                         clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/lecteur");
                       }
                    }
                 }

                Image {
                       id:ok_badge
                       opacity:0
                       height:source.height
                       width: source.width
                       source :"qrc:/qml/content/Ok.png"
                       MouseArea {
                                   anchors.fill: parent
                                   onClicked:{mode_badge();viewid9_timer.stop();}

                                  }

                   }
              }

          Row {
              spacing: 20
              opacity:ok_badge.opacity
               Text{
                    id:r0_text
                    text:qsTr("Type lecteur :")
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
                                 lecteur=keyboard_lecteur.text;
                                 }
                            else
                                {
                                if ( r1_text.state =='On')  {r1_text.state ='Off';option=keyboard_lecteur.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';masque=keyboard_lecteur.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';format=keyboard_lecteur.text}
                                r0_text.state ='On';
                                keyboard_lecteur.maxlength=1
                                keyboard_lecteur.text=lecteur
                                keyboard_alpha.opacity=0;
                                keyboard_num.opacity=0;
                                keyboard_lecteur.opacity=1;
                                keyboard_alpha.enabled=0;
                                keyboard_num.enabled=0
                                }
                           }
                        }
                    }

                TextInput {
                    text: r0_text.state =='Off' ? lecteur : keyboard_lecteur.text
                    maximumLength:1
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r0_text.state =='On'? true:false
                    }
              }

          Row {
              spacing: 20
              opacity: ok_badge.opacity
               Text{
                    id:r1_text
                    text:qsTr("Option    : ")
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
                                 option=keyboard_lecteur.text;
                                 }
                            else
                                {
                                if ( r0_text.state =='On')  {r0_text.state ='Off';lecteur=keyboard_lecteur.text}
                                if ( r2_text.state =='On')  {r2_text.state ='Off';masque=keyboard_lecteur.text}
                                if ( r3_text.state =='On')  {r3_text.state ='Off';format=keyboard_lecteur.text}
                                r1_text.state ='On';
                                keyboard_lecteur.maxlength=3
                                keyboard_lecteur.text=option
                                keyboard_alpha.opacity=0;
                                keyboard_num.opacity=0;
                                keyboard_lecteur.opacity=1;
                                keyboard_alpha.enabled=0;
                                keyboard_num.enabled=0
                                }

                           }
                        }
                    }

                 TextInput {
                    text: r1_text.state =='Off' ? option : keyboard_lecteur.text
                    maximumLength:3
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    cursorVisible: r1_text.state =='On'? true:false

                  }

                }



                Row {
                    spacing: 20
                    opacity: ok_badge.opacity

                      Text{
                          id:r2_text
                          text:qsTr("Masque   : ")
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
                                       masque=keyboard_lecteur.text;
                                      }
                                  else
                                      {
                                      if ( r0_text.state =='On')  {r0_text.state ='Off';lecteur=keyboard_lecteur.text}
                                      if ( r1_text.state =='On')  {r1_text.state ='Off';option=keyboard_lecteur.text}
                                      if ( r3_text.state =='On')  {r3_text.state ='Off';format=keyboard_lecteur.text}
                                      r2_text.state ='On';
                                      keyboard_lecteur.maxlength=20
                                      keyboard_lecteur.text=masque
                                      keyboard_alpha.opacity=0;
                                      keyboard_num.opacity=0;
                                      keyboard_lecteur.opacity=1;
                                      keyboard_alpha.enabled=0;
                                      keyboard_num.enabled=0
                                      }

                                 }
                            }
                         }

                      TextInput {
                         text: r2_text.state =='Off' ? masque : keyboard_lecteur.text
                         maximumLength:20
                         font.family: "Helvetica"
                         font.pixelSize: 30
                         color: "black"
                         cursorVisible: r2_text.state =='On'? true:false
                         }

                     }

                    Row {
                          spacing: 20
                          opacity:ok_badge.opacity
                            Text{
                                id:r3_text
                                text:qsTr("Format  :")
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
                                             format=keyboard_lecteur.text;
                                            }
                                        else
                                            {
                                            if ( r0_text.state =='On')  {r0_text.state ='Off';lecteur=keyboard_lecteur.text}
                                            if ( r1_text.state =='On')  {r1_text.state ='Off';option=keyboard_lecteur.text}
                                            if ( r2_text.state =='On')  {r2_text.state ='Off';masque=keyboard_lecteur.text}
                                            r3_text.state ='On';
                                            keyboard_lecteur.maxlength=20
                                            keyboard_lecteur.text=format
                                            keyboard_alpha.opacity=0;
                                            keyboard_num.opacity=0;
                                            keyboard_lecteur.opacity=1;
                                            keyboard_alpha.enabled=0;
                                            keyboard_num.enabled=0
                                            }

                                       }
                                }
                            }

                        TextInput {
                            text: r3_text.state =='Off' ? format : keyboard_lecteur.text
                            maximumLength:20
                            font.family: "Helvetica"
                            font.pixelSize: 30
                            color: "black"
                            cursorVisible: r3_text.state =='On'? true:false
                           }

                 }


            }



          NumericKeyboard {
             id:keyboard_lecteur
             opacity:0
             anchors.bottom: parent.bottom
             anchors.right: parent.right
             anchors.bottomMargin: 10
             anchors.rightMargin: 10

             RegExpValidator {
                 id:s0
                 regExp:/[7-9.]+/
              }
             RegExpValidator {
                 id:s1
                 regExp:/[0-9]+/
              }

             onClicked:
                {viewid9_timer.stop();
                 if ( r0_text.state =='On') text=text.replace(s0.regExp,"0")
                 if ( r2_text.state =='On') text=text.replace(s1.regExp,"-")
                 text=text.replace(".","*")
                 }

            }
}

