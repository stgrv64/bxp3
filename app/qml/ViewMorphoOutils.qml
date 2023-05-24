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
        id:morpho
        color: "#F0F0F0"
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 5
        property string message


       function clear()
       {
           keyboard.text="";
           r1_text.color="blue"
           r2_text.color="blue"
           r3_text.color="blue"
           r4_text.color="blue"
           r5_text.color="blue"
           r1_image.visible=0
           r2_image.visible=0
           r3_image.visible=0
           r4_image.visible=0
           r5_image.visible=0
           r1_message.visible=0
           r2_message.visible=0
           r3_message.visible=0
           r4_message.visible=0
           r5_message.visible=0
           r6_text.color="blue"
           r6_image.visible=0
           r6_message.visible=0
           keyboard.visible=0
      }

       Column {
          spacing: 10

          Row {
              spacing: 25


               Text{
                    id:r3_text
                    text:qsTr("Effacer toute la base")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (!r3_image.visible)
                                    {
                                    keyboard.text="";
                                    parent.color="#e89101"
                                    r2_text.color="blue"
                                    r1_text.color="blue"
                                    r4_text.color="blue"
                                    r5_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=0
                                    r3_image.visible=1
                                    r4_image.visible=0
                                    r5_image.visible=0
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    r6_text.color="blue"
                                    r6_image.visible=0
                                    r6_message.visible=0
                                   keyboard.visible=0
                                    }
                                else
                                {
                                    parent.color="blue"
                                    r3_image.visible=0
                                    keyboard.visible=0
                                }
                                }
                        }
                    }

                    Image {
                            id:r3_image
                            visible:false
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r3_message.visible=1;keyboard.visible=0;r3_text.color="blue";r3_image.visible=0;signal_morpho_cancel();signal_morpho_deletealluser();}
                                       }

                        }

                    Text  {
                        id: r3_message
                           width:text.width
                           color: "#e89101";
                           font.family: "Liberation Sans Narrow"
                           font.pixelSize: 20
                           text:message
                            }
               }


          Row {
              spacing: 25


               Text{
                    id:r4_text
                    text:qsTr("Supprimer la base")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (!r4_image.visible)
                                    {
                                    keyboard.text="";
                                    parent.color="#e89101"
                                    r2_text.color="blue"
                                    r1_text.color="blue"
                                    r3_text.color="blue"
                                    r5_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=0
                                    r1_image.visible=0
                                    r3_image.visible=0
                                    r4_image.visible=1
                                    r5_image.visible=0
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    keyboard.visible=0
                                    r6_text.color="blue"
                                    r6_image.visible=0
                                    r6_message.visible=0
                                    }
                                else
                                {
                                    parent.color="blue"
                                    r4_image.visible=0
                                    keyboard.visible=0
                                }
                                }
                        }
                    }

                    Image {
                            id:r4_image
                            visible:false
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r4_message.visible=1;keyboard.visible=0;r4_text.color="blue";r4_image.visible=0;signal_morpho_cancel();signal_morpho_destroy_database();}
                                       }

                        }

                    Text  {
                        id: r4_message
                           width:text.width
                           color: "#e89101";
                           font.family: "Liberation Sans Narrow"
                           font.pixelSize: 20
                           text:message
                            }
               }


          Row {
              spacing: 25


               Text{
                    id:r5_text
                    text:qsTr("Construire la base")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (!r5_image.visible)
                                    {
                                    keyboard.text="";
                                    parent.color="#e89101"
                                    r2_text.color="blue"
                                    r1_text.color="blue"
                                    r4_text.color="blue"
                                    r3_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=0
                                    r3_image.visible=0
                                    r4_image.visible=0
                                    r5_image.visible=1
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    r6_text.color="blue"
                                    r6_image.visible=0
                                    r6_message.visible=0
                                   keyboard.visible=0
                                    }
                                else
                                {
                                    parent.color="blue"
                                    r5_image.visible=0
                                    keyboard.visible=0
                                }
                                }
                        }
                    }

                    Image {
                            id:r5_image
                            visible:false
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r5_message.visible=1;keyboard.visible=0;r5_text.color="blue";r5_image.visible=0;signal_morpho_cancel();signal_morpho_create_database();}
                                       }

                        }

                    Text  {
                        id: r5_message
                           width:text.width
                           color: "#e89101";
                           font.family: "Liberation Sans Narrow"
                           font.pixelSize: 20
                           text:message
                            }
               }

          Row {
              spacing: 10


               Text{
                    id:r2_text
                    text:qsTr("Installer la base de sauvegarde")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (r2_image.visible==0)
                                    {
                                    keyboard.text="";
                                    keyboard.visible=0
                                    parent.color="#e89101"
                                    r1_text.color="blue"
                                    r3_text.color="blue"
                                    r4_text.color="blue"
                                    r5_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=1
                                    r3_image.visible=0
                                    r4_image.visible=0
                                    r5_image.visible=0
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    r6_text.color="blue"
                                    r6_image.visible=0
                                    r6_message.visible=0
                                   }
                                else
                                {
                                    parent.color="blue"
                                    r2_image.visible=0
                                    keyboard.visible=0

                                }
                                }
                        }
                    }



                    Image {
                            id:r2_image
                            visible:false
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r2_message.visible=1;keyboard.visible=0; r2_text.color="blue";signal_morpho_cancel();signal_morpho_reload_database();r2_image.visible=0;}
                                       }

                        }

                 }

          Text  {
           id: r2_message
              width:text.width
              color: "#e89101";
              font.family: "Liberation Sans Narrow"
              font.pixelSize: 20
              text:message
               }
/*
          Row {
              spacing: 10


               Text{
                    id:r2_text
                    text:"Supprimer Index";
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (r2_image.visible==0)
                                    {
                                    keyboard.text="";
                                    keyboard.visible=1
                                    parent.color="#e89101"
                                    r1_text.color="blue"
                                    r3_text.color="blue"
                                    r4_text.color="blue"
                                    r5_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=1
                                    r3_image.visible=0
                                    r4_image.visible=0
                                    r5_image.visible=0
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    r6_text.color="blue"
                                    r6_image.visible=0
                                    r6_message.visible=0
                                    keyboard.maxlength=textInput1.maximumLength
                                   }
                                else
                                {
                                    parent.color="blue"
                                    r2_image.visible=0
                                    keyboard.visible=0

                                }
                                }
                        }
                    }



                    Image {
                            id:r2_image
                            visible:0
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r2_message.visible=1;keyboard.visible=0; r2_text.color="blue";signal_morpho_cancel();signal_morpho_deleteuserindex(textInput1.text);r2_image.visible=0;}
                                       }

                        }

                       Text  {
                        id: r2_message
                           width:text.width
                           color: "#e89101";
                           font.family: "Liberation Sans Narrow"
                           font.pixelSize: 20
                           text:message
                            }
                }
          TextInput{
              id:textInput1
              text: r2_image.visible == 0 ? "" : keyboard.text
              maximumLength:5
              font.family: "Liberation Sans Narrow"
              font.pixelSize: 20
              color: "black"
              cursorVisible: r2_image.visible == 0 ? false:true
          }
*/
          Row {
              spacing: 10


               Text{
                    id:r6_text
                    text:qsTr("Nb empreintes/Nb max")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                    MouseArea {
                            anchors.fill: parent
                            onClicked:
                                {
                                if (r6_image.visible==0)
                                    {
                                    keyboard.text="";
                                    keyboard.visible=0
                                    parent.color="#e89101"
                                    r1_text.color="blue"
                                    r2_text.color="blue"
                                    r3_text.color="blue"
                                    r4_text.color="blue"
                                    r5_text.color="blue"
                                    r1_image.visible=0
                                    r2_image.visible=0
                                    r3_image.visible=0
                                    r4_image.visible=0
                                    r5_image.visible=0
                                    r6_image.visible=1
                                    r1_message.visible=0
                                    r2_message.visible=0
                                    r3_message.visible=0
                                    r4_message.visible=0
                                    r5_message.visible=0
                                    r6_message.visible=0
                                   }
                                else
                                {
                                    parent.color="blue"
                                    r6_image.visible=0
                                    keyboard.visible=0

                                }
                                }
                        }
                    }



                    Image {
                            id:r6_image
                            visible:false
                             height:source.height
                             width: source.width
                            // anchors.right:morpho.right
                             source :"qrc:/qml/content/Ok.png"
                             MouseArea {
                                        anchors.fill: parent
                                        onClicked: {message="";r6_message.visible=1;keyboard.visible=0; r6_text.color="blue";signal_morpho_cancel();signal_morpho_getbaseconfig();r6_image.visible=0;}
                                       }

                        }

               }

          Text  {
           id: r6_message
              width:text.width
              color: "#e89101";
              font.family: "Liberation Sans Narrow"
              font.pixelSize: 20
              text:message
               }


          Row {
              spacing: 10

                Text{
                    id:r1_text
                    text:qsTr("Ajouter Identifiant")
                    color: "blue";
                    width:text.width
                    font.family: "Helvetica"
                    font.pixelSize: 50

                MouseArea {
                        anchors.fill: parent
                        onClicked:
                            {
                            if (r1_image.visible==0)
                                {
                                keyboard.text="";
                                keyboard.visible=1
                                parent.color="#e89101"
                                r2_text.color="blue"
                                r3_text.color="blue"
                                r4_text.color="blue"
                                r5_text.color="blue"
                                r1_image.visible=1
                                r2_image.visible=0
                                r3_image.visible=0
                                r4_image.visible=0
                                r5_image.visible=0
                                r1_message.visible=0
                                r2_message.visible=0
                                r3_message.visible=0
                                r4_message.visible=0
                                r5_message.visible=0
                                r6_text.color="blue"
                                r6_image.visible=0
                                r6_message.visible=0
                                keyboard.maxlength=textInput.maximumLength
                               }
                            else
                            {
                                parent.color="blue"
                                r1_image.visible=0
                                keyboard.visible=0
                            }
                            }
                    }
                }

                Image {
                        id:r1_image
                        visible:false
                        height:source.height
                         width: source.width
                        source :"qrc:/qml/content/Ok.png"
                         MouseArea {
                                    anchors.fill: parent
                                    onClicked: {message="";r1_message.visible=1;keyboard.visible=0;r1_text.color="blue";signal_morpho_cancel();signal_morpho_enrolluser(textInput.text);r1_image.visible=0;}
                                   }

                    }


/*                Text  {
                    id: r1_message
                       width:text.width
                       color: "#e89101";
                       font.family: "Liberation Sans Narrow"
                       font.pixelSize: 20
                       text:message
                    }
  */
          }

                Text  {
              id: r1_message
                 width:text.width
                 color: "#e89101";
                 font.family: "Liberation Sans Narrow"
                 font.pixelSize: 20
                 text:message
              }

          TextInput {
              id:textInput
              text: r1_image.visible == 0 ? "" : keyboard.text
              maximumLength:10
              font.family: "Liberation Sans Narrow"
              font.pixelSize: 20
              color: "black"
              cursorVisible: r1_image.visible == 0 ? false:true
              }




       }




       NumericKeyboard {
          id:keyboard
          visible: false
          anchors.bottom: parent.bottom
          anchors.right: parent.right
          anchors.bottomMargin: 10
          anchors.rightMargin: 10
         }

}

