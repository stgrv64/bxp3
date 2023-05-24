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
    property list<Rectangle> color_tests:
        [
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"},
        Rectangle {color:"transparent"}
    ]


    property real w:main.orientation == 0 ? 230 : 220;
    property real h:60;
    property string textcolor: "black"
    property string textfontfamily: "Liberation Sans Narrow"
    property bool textfontitalic: false
    property real textfontpixelSize: main.orientation == 0 ? 25 : 22

    function reset_color() {
        for (var i=0; i<10;i++) {
            color_tests[i].color="transparent";
        }
    }

    function play_test_audio() {
        test_audio.source="file:/var/x7/contents/m4a/test_audio.m4a";
        test_audio.play()
    }

    Audio
     {
         id:test_audio
         onStopped:source=""
     }

     Column{
         anchors.top: parent.top
         anchors.topMargin: 5
         anchors.left: parent.left
         anchors.leftMargin: 10
         spacing: 10
          Rectangle
             {
              border.color: "black"
              color: color_tests[0].color
              width:w
              height:h
              radius:10
              MouseArea {
                   anchors.fill: parent
                   onClicked:{signal_time2bii_tests(0);}
                  }

             Text {
                 anchors.verticalCenter:parent.verticalCenter
                 anchors.horizontalCenter:parent.horizontalCenter
                 font.family: textfontfamily
                 font.italic: textfontitalic
                 font.pixelSize:textfontpixelSize
                 text:"TEST READER"
                 }
               }
 /*          Rectangle
             {
              border.color: "black"
              color: color_tests[1].color
              width:w
              height:h
              MouseArea {
                   anchors.fill: parent
                   //onClicked:{time2bii_tests(1);}
                  }

            Text {
                anchors.verticalCenter:parent.verticalCenter
                anchors.horizontalCenter:parent.horizontalCenter
                font.family: textfontfamily
                font.italic: textfontitalic
                font.pixelSize:textfontpixelSize
                 text:""
                 }
               }*/

           Rectangle
                 {
                  border.color: "black"
                  color: color_tests[2].color
                  width:w
                  height:h
                  radius:10
                  MouseArea {
                       anchors.fill: parent
                       onClicked:{signal_time2bii_tests(2);}
                      }

              Text {
                  anchors.verticalCenter:parent.verticalCenter
                  anchors.horizontalCenter:parent.horizontalCenter
                  font.family: textfontfamily
                  font.italic: textfontitalic
                  font.pixelSize:textfontpixelSize
                    text:"TEST USB"
                     }
                   }


 /*           Rectangle
                 {
                  border.color: "black"
                  color: color_tests[3].color
                  width:w
                  height:h
                 MouseArea {
                       anchors.fill: parent
                       //onClicked:{time2bii_tests(3);}
                      }

                Text {
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    font.family: textfontfamily
                    font.italic: textfontitalic
                    font.pixelSize:textfontpixelSize
                     text:""
                     }
                   }*/

            Rectangle
                 {
                  border.color: "black"
                  color: color_tests[4].color
                  width:w
                  height:h
                  radius:10
                 MouseArea {
                       anchors.fill: parent
                       onClicked:{signal_time2bii_tests(4);}
                      }

                Text {
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    font.family: textfontfamily
                    font.italic: textfontitalic
                    font.pixelSize:textfontpixelSize
                     text:"TEST ETHERNET"
                     }
                   }
        }

     Column{
         anchors.top: parent.top
         anchors.topMargin: 5
         anchors.right: parent.right
         anchors.rightMargin: 10
         spacing: 10

            Rectangle
                 {
                  border.color: "black"
                  color: color_tests[5].color
                  width:w
                  height:h
                  radius:10
                  MouseArea {
                       anchors.fill: parent
                       onClicked:{signal_time2bii_tests(5);}
                      }

                Text {
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    font.family: textfontfamily
                    font.italic: textfontitalic
                    font.pixelSize:textfontpixelSize
                     text:"TEST RELAY"
                     }
                   }

/*             Rectangle
                 {
                  border.color: "black"
                  color: color_tests[6].color
                  width:w
                  height:h
                 MouseArea {
                       anchors.fill: parent
                       //onClicked:{signal_time2bii_tests(6);}
                      }

                Text {
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    font.family: textfontfamily
                    font.italic: textfontitalic
                    font.pixelSize:textfontpixelSize
                     text:""
                     }
                   }*/

             Rectangle
                 {
                  border.color: "black"
                  color: color_tests[7].color
                  width:w
                  height:h
                  radius:10
                 MouseArea {
                       anchors.fill: parent
                       onClicked:{signal_time2bii_tests(7);}
                      }

                Text {
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    font.family: textfontfamily
                    font.italic: textfontitalic
                    font.pixelSize:textfontpixelSize
                    text:"TEST BACKLIGHT"
                     }
                   }

             Rectangle
                     {
                      border.color: "black"
                      color: color_tests[8].color
                      width:w
                      height:h
                      radius:10
                     MouseArea {
                           anchors.fill: parent
                           onClicked:{signal_time2bii_tests(8);}
                          }

                   Text {
                        anchors.verticalCenter:parent.verticalCenter
                        anchors.horizontalCenter:parent.horizontalCenter
                        font.family: textfontfamily
                        font.italic: textfontitalic
                        font.pixelSize:textfontpixelSize
                        text:"TEST AUDIO"
                        }
                      }

 /*            Rectangle
                     {
                      border.color: "black"
                      color: color_tests[9].color
                      width:w
                      height:h
                     MouseArea {
                           anchors.fill: parent
                           //onClicked:{time2bii_tests(9);}
                          }

                   Text {
                        anchors.verticalCenter:parent.verticalCenter
                        anchors.horizontalCenter:parent.horizontalCenter
                        font.family: textfontfamily
                        font.italic: textfontitalic
                        font.pixelSize:textfontpixelSize
                        text:""
                        }
                     }*/
            }


     Image {
            height:source.height
            width: source.width
            anchors.bottom:parent.bottom
            anchors.bottomMargin:10
            anchors.left:parent.left
            anchors.leftMargin:10
           source :"qrc:/qml/content/Exit.png"
            MouseArea {
                       anchors.fill: parent
                       onClicked: {reset_color();main.state="view1"}
                      }
          }
}



