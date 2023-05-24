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
   property string heure
   property string minute
   property string seconde
   property string an
   property string mois
   property string jour
   property string action:""

   function change(type,val)
   {
   screen_touch();
       var valr=val
       if (action==="+") valr=increment(type,val)
       else if (action==="-") valr=decrement(type,val)
   return valr
   }

   function increment(type,val)
    {
    var x=val
    x++;
    switch(type)
    {
    case 0:
        x=x%24;
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 1:
    case 2:
        x=x%60;
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 3:
        x=x%32;
        if (x ===0) x=1
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 4:
        x=x%13;
        if (x ===0) x=1
        val ="0"+x;
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 5:
        if (x <2014) val ="2014";
        else val=x;
        break;
    }
   return val
   }

   function decrement(type,val)
    {
    var x=val
    x--;
    switch(type)
    {
    case 0:
        if (x < 0) x=23
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 1:
    case 2:
        if (x < 0) x=59
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 3:
        if (x ===0) x=31
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 4:
        if (x === 0) x=12
        val ="0"+x;
        if (x <10) val ="0"+x;
        else val=x;
        break;
    case 5:
        if (x <2014) val ="2014";
        else val=x;
        break;
    }
   return val
   }

   Rectangle {
     anchors.fill: parent
     color: "transparent"



     Row{
         id: rheure
         spacing:10
         anchors.top:parent.top
         anchors.topMargin: 50
         anchors.horizontalCenter: parent.horizontalCenter


             Rectangle {
                  height:heure_text.height;
                  width: heure_text.width;

                  Text {
                      id: heure_text
                      text: heure
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#0d75ec"
                      font.family: "Liberation Sans Narrow"
                      font.italic: true
                      font.pixelSize: main.orientation == 0 ? 100: 80
                      }
                   MouseArea {
                             anchors.fill: parent
                             onClicked: {heure=change(0,heure)}
                            }
                  }


             Rectangle {
                 height:point1.height;
                  width: point1.width;

                  Text {
                      id: point1
                      text: ":"
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#0d75ec"
                      font.family: "Liberation Sans Narrow"
                      font.italic: true
                      font.pixelSize: main.orientation == 0 ? 100: 80
                     }

                 }


              Rectangle {
                  height:minute_text.height;
                   width: minute_text.width;

                   Text {
                       id: minute_text
                       text: minute
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                        font.pixelSize: main.orientation == 0 ? 100: 80
                   }

                   MouseArea {
                              anchors.fill: parent
                              onClicked: { minute=change(1,minute)}
                             }
               }

              Rectangle {
                  height:point2.height;
                   width: point2.width;

                   Text {
                       id: point2
                       text: ":"
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                       font.pixelSize: main.orientation == 0 ? 100: 80
                   }

                  }
              Rectangle {
                  height: seconde_text.height;
                  width: seconde_text.width;

                   Text {
                       id: seconde_text
                       text: seconde
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                       font.pixelSize: main.orientation == 0 ? 100: 80
                   }

                   MouseArea {
                              anchors.fill: parent
                              onClicked: { seconde=change(2,seconde)}
                             }

            }

    }

// Zone Date
     Row{
         id: rdate
         spacing:10
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.verticalCenter: parent.verticalCenter


             Rectangle {
                  height:jour_text.height;
                  width: jour_text.width;

                  Text {
                      id: jour_text
                      text: jour
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#0d75ec"
                      font.family: "Liberation Sans Narrow"
                      font.italic: true
                      font.pixelSize: main.orientation == 0 ? 100: 70
                      }
                   MouseArea {
                             anchors.fill: parent
                             onClicked: {jour=change(3,jour)}
                            }
                  }


             Rectangle {
                 height:barre1.height;
                  width: barre1.width;

                  Text {
                      id: barre1
                      text: "/"
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#0d75ec"
                      font.family: "Liberation Sans Narrow"
                      font.italic: true
                      font.pixelSize: main.orientation == 0 ? 100: 70
                     }

                 }


              Rectangle {
                  height:mois_text.height;
                   width: mois_text.width;

                   Text {
                       id: mois_text
                       text: mois
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                        font.pixelSize: main.orientation == 0 ? 100: 70
                   }

                   MouseArea {
                              anchors.fill: parent
                              onClicked: { mois=change(4,mois)}
                             }
               }

              Rectangle {
                  height:barre2.height;
                   width: barre2.width;

                   Text {
                       id: barre2
                       text: "/"
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                       font.pixelSize: main.orientation == 0 ? 100: 70
                   }

                  }
              Rectangle {
                  height: an_text.height;
                  width: an_text.width;

                   Text {
                       id: an_text
                       text: an
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       color: "#0d75ec"
                       font.family: "Liberation Sans Narrow"
                       font.italic: true
                       font.pixelSize: main.orientation == 0 ? 100: 70
                   }

                   MouseArea {
                              anchors.fill: parent
                              onClicked: { an=change(5,an)}
                             }

            }

    }


Row{
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom:parent.bottom
    spacing:50
    Rectangle {
     height:moins_text.height;
     width: moins_text.width;

     Text {
         id: moins_text
         text: "-"
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.verticalCenter: parent.verticalCenter
         color: "#0d75ec"
         font.family: "Liberation Sans Narrow"
         font.italic: true
         font.pixelSize: main.orientation == 0 ? 100: 80
     }
     MouseArea {
                anchors.fill: parent
                onClicked: {plus_text.color="#0d75ec";moins_text.color="#a7f706"; action="-"}
               }

    }

    Rectangle {
     height:plus_text.height;
     width: plus_text.width;

     Text {
         id: plus_text
         text: "+"
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.verticalCenter: parent.verticalCenter
         color: "#0d75ec"
         font.family: "Liberation Sans Narrow"
         font.italic: true
         font.pixelSize: main.orientation == 0 ? 100: 80
     }
     MouseArea {
                anchors.fill: parent
                onClicked: {moins_text.color="#0d75ec";plus_text.color="#a7f706"; action="+"}
               }
    }


}


Image {
         id : ok
         height:source.height
         width: source.width
         anchors.bottom:parent.bottom
         anchors.right:parent.right
         anchors.rightMargin:10
         anchors.bottomMargin:10
        source :"qrc:/qml/content/Ok.png"
         MouseArea {
                    anchors.fill: parent
                    onClicked: {signal_qml_set_date_heure(an,mois,jour,heure,minute,seconde);main.state="view8";}
                   }

    }

 }
}
