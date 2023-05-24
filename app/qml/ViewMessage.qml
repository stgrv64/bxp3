import QtQuick 2.7
import QtMultimedia 5.8

Rectangle
{
id:message_webservice
width:478
height:entete.height+info.height+4
border.width:2
border.color:"black"
anchors.horizontalCenter:parent.horizontalCenter
anchors.verticalCenter: parent.verticalCenter
opacity:(message1_titre.length ===0 && message2_titre.length===0 && message3_titre.length ===0) ? 0:1


Rectangle
    {
     id:entete
     height:36
     width:474
     anchors.horizontalCenter: parent.horizontalCenter
     color: "black"
     Image
     {
        id:entete_i
        //height:source.height
        //width: source.width
        height:0
        width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.left:parent.left
        anchors.leftMargin:5
        source:"qrc:/qml/content/lettre.png"
     }
     Image {
              //id : exit
              height:source.height
              width: source.width
              anchors.verticalCenter: parent.verticalCenter
              anchors.right:parent.right
              anchors.rightMargin:5
              source :"qrc:/qml/content/Quit.png"
        }

     MouseArea {
                anchors.fill: parent
                onClicked:  viewid1_timer.interval= 1
               }
    }


Rectangle{
     id:info
     width:474
     height:col.height

     anchors.horizontalCenter:parent.horizontalCenter
     anchors.top: entete.bottom
     MouseArea {
                anchors.fill: parent
                onClicked:  {}
                onPressed:  {}
               }

     Column{
         id:col
         width: parent.width
         height:mess1.height+mess2.height+mess3.height+((mess1.visible+mess2.visible+mess3.visible)*spacing)
         spacing:20
         Rectangle
             {
              id:mess1
              visible:message1_titre ==="" ? 0 : 1
              height:message1_titre ==="" ? 0 : mess1_col.height
              width:parent.width
              Column
                   {
                   id:mess1_col
                   spacing:0
                   height:mess1_col1.height+mess1_col2.height
                   width:parent.width
                   visible:parent.visible
                   Rectangle{
                       id:mess1_col1
                       visible:parent.visible
                       width:parent.width
                       height:message1_titre==="" ? 0 : mess1_t1.height+5
                       anchors.left:parent.left
                       color: "#0F7DFF"

                       Text{
                             id:mess1_t1
//                           maximumLineCount :2
//                           width:474
//                           wrapMode: Text.Wrap
//                           anchors.left:parent.left
//                           anchors.leftMargin:4
                             height:paintedHeight
                             visible:parent.visible
                             anchors.horizontalCenter:parent.horizontalCenter
                             anchors.verticalCenter:parent.verticalCenter
                             font.family: "Liberation Sans Narrow"
                             font.italic: false
                             font.pixelSize: 18
                             text:message1_titre
                             color: "white"
                           }
                      }


                   Rectangle {
                       id:mess1_col2
                       visible:parent.visible
                       width:parent.width
                       height:message1_text==="" ? 0 : mess1_t2.height+5
                       anchors.left:parent.left
                       color: "#0F7DFF"
                       Text{
                            id:mess1_t2
                            height:paintedHeight
                            visible:parent.visible
                            anchors.verticalCenter:parent.verticalCenter
                            anchors.horizontalCenter:parent.horizontalCenter
                            font.family: "Liberation Sans Narrow"
                            font.italic: false
                            font.pixelSize: 18
                            text:message1_text
                            color: "white"
                          }
                       }
                  }
             }


         Rectangle
             {
              id:mess2
              visible:message2_titre ==="" ? 0 : 1
              height:message2_titre ==="" ? 0 : mess2_col.height
              width:parent.width
              color: "White"
              Column
                   {
                   id:mess2_col
                   spacing:5
                   property int space:mess2_col2.height===0 ? 0 : spacing
                   property int space1:mess2_col3.height===0 ? 0 : spacing
                   height:mess2_col1.height+mess2_col2.height+mess2_col3.height+space+space1
                   width:parent.width
                   visible:parent.visible
                   Text{
                        id:mess2_col1
                        visible:parent.visible
                        height:message2_titre ==="" ? 0 : paintedHeight
                        anchors.left:parent.left
                        anchors.leftMargin:2
                        font.family: "Liberation Sans Narrow"
                        font.italic: false
                        font.underline: true
                        font.pixelSize: 18
                        text:message2_titre
                        color: "#AFAFAF"
                         }
                   Text{
                       id:mess2_col2
                       visible:parent.visible
                       height:message2_text ==="" ? 0 :paintedHeight
                       anchors.horizontalCenter:parent.horizontalCenter
                       font.family: "Liberation Sans Narrow"
                       font.italic: false
                       font.pixelSize: 18
                       text:message2_text
                       color: "black"
                       }
                   Text{
                       id:mess2_col3
                       visible:parent.visible
                       height:message2_text1 ==="" ? 0 :paintedHeight
                       anchors.horizontalCenter:parent.horizontalCenter
                       font.family: "Liberation Sans Narrow"
                       font.italic: false
                       font.pixelSize: 18
                       text:message2_text1
                       color: "black"
                       }
                      }
               }

         Rectangle
             {
              id:mess3
              visible:message3_titre ==="" ? 0 : 1
              height:message3_titre ==="" ? 0 : mess3_col.height
              width:parent.width
              color: "White"
              Column
                   {
                    id: mess3_col
                    visible:parent.visible
                    height:mess3_col1.height+mess3_col2.height+mess3_col2_1.height+mess3_col3.height+mess3_col4.height+spacing+space1+space2
                    spacing: 5
                    property int space:mess3_col2.height===0 ? 0 : spacing
                    property int space1:mess3_col3.height===0 ? 0 : spacing
                    property int space2:mess3_col4.height===0 ? 0 : spacing
                    width:parent.width
                    Text{
                        id:mess3_col1
                        height:message3_titre ==="" ? 0 : paintedHeight
                         anchors.left:parent.left
                         anchors.leftMargin:2
                         font.family: "Liberation Sans Narrow"
                         font.italic: false
                         font.underline: true
                         font.pixelSize: 18
                         text:message3_titre
                         color: "#AFAFAF"
                         }

                    Text{
                        id:mess3_col2
                        visible:parent.visible
                        height:message3_text ==="" ? 0 : paintedHeight
                         anchors.horizontalCenter:parent.horizontalCenter
                         font.family: "Liberation Sans Narrow"
                         font.italic: false
                         font.pixelSize: 18
                         text:message3_text
                         color: "black"
                         }
                    Text{
                        id:mess3_col2_1
                         visible:parent.visible
                         height:message3_text1 ==="" ? 0 : paintedHeight
                         anchors.horizontalCenter:parent.horizontalCenter
                         font.family: "Liberation Sans Narrow"
                         font.italic: false
                         font.pixelSize: 18
                         text:message3_text1
                         color: "black"
                         }

                    Row{
                       id:mess3_col3
                       visible:parent.visible
                       height: (mess3_col3_r1.height >= mess3_col3_r2.height) ? mess3_col3_r1.height : mess3_col3_r2.height
                       width:parent.width
                       anchors.horizontalCenter:parent.horizontalCenter
                       spacing:6
                       Rectangle
                              {
                               id:mess3_col3_r1
                               visible:parent.visible
                               height: message3_c1==="" ? 0: mess3_col3_t1.height+3
                               width:message3_c2==="" ? (parent.width)-2 : (parent.width/2)-3
                               color:"#DADDDF"
                               Text{
                                   id:mess3_col3_t1
                                   visible:parent.visible
                                   height: message3_c1 ===""  ? 0 : paintedHeight
                                   anchors.horizontalCenter:parent.horizontalCenter
                                   anchors.verticalCenter: parent.verticalCenter
                                   font.family: "Liberation Sans Narrow"
                                   font.italic: false
                                   font.pixelSize: 18
                                   text:message3_c1
                                   color: "black"
                                  }
                               }

                       Rectangle
                              {
                               id:mess3_col3_r2
                               visible:parent.visible
                               height: message3_c2==="" ? 0: mess3_col3_t2.height+3
                               width:message3_c1==="" ? (parent.width)-2 : (parent.width/2)-3
                               color:"#DADDDF"
                               Text{
                                   id:mess3_col3_t2
                                   visible:parent.visible
                                   height: message3_c2 ===""  ? 0 : paintedHeight
                                   anchors.horizontalCenter:parent.horizontalCenter
                                   anchors.verticalCenter: parent.verticalCenter
                                   font.family: "Liberation Sans Narrow"
                                   font.italic: false
                                   font.pixelSize: 18
                                   text:message3_c2
                                   color: "black"
                                  }
                               }
                        }

                    Row{
                       id:mess3_col4
                       visible:parent.visible
                       height: (mess3_col4_r1.height >= mess3_col4_r2.height) ? mess3_col4_r1.height : mess3_col4_r2.height
                       width:parent.width
                       anchors.horizontalCenter:parent.horizontalCenter
                       spacing:6
                       Rectangle
                              {
                               id:mess3_col4_r1
                               visible:parent.visible
                               height: message3_c3==="" ? 0: mess3_col4_t1.height+3
                               width:message3_c4==="" ? (parent.width)-2 : (parent.width/2)-3
                               color:"#DADDDF"
                               Text{
                                   id:mess3_col4_t1
                                   visible:parent.visible
                                   height:message3_c3==="" ? 0:paintedHeight
                                   anchors.horizontalCenter:parent.horizontalCenter
                                   anchors.verticalCenter: parent.verticalCenter
                                   font.family: "Liberation Sans Narrow"
                                   font.italic: false
                                   font.pixelSize: 18
                                   text:message3_c3
                                   color: "black"
                                  }
                               }

                       Rectangle
                              {
                               id:mess3_col4_r2
                               visible:parent.visible
                               height: message3_c4==="" ? 0: mess3_col4_t2.height+3
                               width:message3_c3==="" ? (parent.width)-2 : (parent.width/2)-3
                               color:"#DADDDF"
                               Text{
                                   id:mess3_col4_t2
                                   visible:parent.visible
                                   height:message3_c4==="" ? 0:paintedHeight
                                   anchors.horizontalCenter:parent.horizontalCenter
                                   anchors.verticalCenter: parent.verticalCenter
                                   font.family: "Liberation Sans Narrow"
                                   font.italic: false
                                   font.pixelSize: 18
                                   text:message3_c4
                                   color: "black"
                                  }
                               }
                        }
                       }
                    }
             }

           }
         }
