import QtQuick 2.7

 Item{
        id:back
        width:  main.orientation == 0 ? 760 : 310
        height:  main.orientation == 0 ? 310 : 620
//        color:"transparent"
        MouseArea {
                   anchors.fill: parent
                   onClicked:
                         {
                       grid.currentIndex = -1
                       screen_touch();
                       }
                   onPressed:
                       {
                     grid.currentIndex = -1
                     screen_touch();
                     }
                   }


 function reset( )
       {
       grid.currentIndex=-1
       console.log("TouchesFonction.qml reset()")
       }


        Component {
               id: myDelegate


               Item {
                   id:bouton
                   width: grid.cellWidth-20; height: grid.cellHeight-20
                   opacity:(bouton_text.text.length===0 && bouton_text1.text.length===0) ? 0 :1
                   property  string a:libelle.indexOf('$') ===-1 ? (libelle.indexOf('§') ===-1 ? libelle.slice(0,libelle.length) : libelle.slice(0,libelle.indexOf('§'))) : libelle.slice(0,libelle.indexOf('$'))
                   property  string label:a.replace(/#/,'\n');
                   property  string b:libelle.indexOf('$') ===-1 ?  (libelle.indexOf('§') ===-1 ? "" : libelle.slice(libelle.indexOf('§')+1,libelle.length)) : libelle.slice(libelle.indexOf('$')+1,libelle.length);
                   property  string label1:b.length ===0 ? "" : b.replace(/#/,'\n');
                   property  string tempovisu:"120"

                 Rectangle {

                       anchors.fill: parent
                       opacity:bouton.opacity===0 ? 0 :0.7
                       radius: height*0.20;
                       gradient:Gradient {
                            GradientStop {
                                position: 0.00;
                                color: "#ece5e5";
                            }
                            GradientStop {
                                position: 1.00;
                                color: "#b3afaf";
                            }
                          }
                      }


                   Text {
                         id:bouton_text
                         opacity:bouton.opacity===0 ? 0 :1
                         color:  grid.currentIndex===index ? "white": "black" ;
                         font.family: "Liberation Sans Narrow";
                         font.italic: false;
                         font.bold: true;
                         font.pixelSize: 18;
                         text: label
                         anchors { bottom: icon.top;bottomMargin:5; horizontalCenter: parent.horizontalCenter }
                         //anchors.bottomMargin: 10

                         }

                     Image {
                          id:icon
                          property real zoom:1
                          property string icon_path:"file:/var/x7/contents/images/"
                          SequentialAnimation {
                                  id:icon_animation
                                  PropertyAnimation  { target: icon; properties: "zoom"; to: 1.5;easing.type: Easing.Linear;duration: 200;}
                                  PropertyAnimation  { target: icon; properties: "zoom"; to: 1;easing.type: Easing.Linear;duration: 200;}
                                   }
                          opacity:bouton.opacity===0 ? 0 :1
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.horizontalCenter: parent.horizontalCenter
                          source: icon_path+(activite+".img")
                          width: sourceSize.width*zoom
                          height: sourceSize.height*zoom
                          onStatusChanged: {if (icon.status == Image.Error)
                                           {
                                               if (url.length ===0) icon.source="qrc:/qml/icones/touchesfonctions/badge.png";
                                               else icon.source="qrc:/qml/icones/touchesfonctions/intranet.png";
                                           };console.log("TouchesFonction onStatusChanged+ grid.index: " + grid.currentIndex)}
                     }

                   Text {
                         id:bouton_text1
                         opacity:bouton.opacity===0 ? 0 :1
                         color:  grid.currentIndex===index ? "white": "black" ;
                         font.family: "Liberation Sans Narrow";
                         font.italic: true;
                         font.pixelSize: 18;
                          text:label1
                         anchors { top: icon.bottom;topMargin:5; horizontalCenter: parent.horizontalCenter }
                         //anchors.bottomMargin: 10

                         }

                   MouseArea {
                      anchors.fill: parent
                      onClicked:
                           {
                            screen_touch();
                            grid.currentIndex=index
                            icon_animation.start()
                            if (tempo.length !=0) bouton.tempovisu=tempo
                            // console.debug(activite+" "+url + " " + bouton.tempovisu)
                            main.appui_touche(activite,url,proxy,bouton.tempovisu)
                            }
                      onPressed:
                           {
                            screen_touch();
                            grid.currentIndex=index
                            icon_animation.start()
                            if (tempo.length !=0) bouton.tempovisu=tempo
                           // console.debug(activite+" "+url + " " + bouton.tempovisu)
                            main.appui_touche(activite,url,proxy,bouton.tempovisu)
                            }
                    }
               }
           }

        Component {
                id: appHighlight
                Rectangle {
                    gradient:Gradient {
                    GradientStop {
                        position: 0.00;
                        color: "#0477f9";
                    }
                    GradientStop {
                        position: 1.00;
                        color: "#2630c0";
                    }
                    }
                    width: 135; height: 135; radius: height*0.20;
                }
            }

       GridView {
           id:grid
           clip:true
           cellWidth: 155; cellHeight: 155
           width: orientation == 0 ? ((Math.round(count/2))*cellWidth < back.width ? (Math.round(count/2))*cellWidth: back.width) : back.width
            height:orientation == 0 ? back.height : ((Math.round(count/2))*cellHeight < back.height ? (Math.round(count/2))*cellHeight: back.height)
/*           width: main.orientation == 0 ? ((count/2)*cellWidth > parent.width ? (count/2)*cellWidth: parent.width) : parent.width
           height:main.orientation == 0 ? parent.height : ((count/2)*cellHeight > parent.heigt ? (count/2)*cellHeight: parent.height)
 */
            //          anchors.fill: parent
           focus: true
           model: touchesfonctionmodel
           delegate: myDelegate
           flickableDirection: main.orientation == 0 ? Flickable.HorizontalFlick : Flickable.VerticalFlick
           flow:main.orientation == 0 ? GridView.TopToBottom : GridView.LeftToRight
currentIndex : -1
           highlight: appHighlight
           highlightFollowsCurrentItem: true
            snapMode: GridView.SnapToRow
            flickableChildren: MouseArea
                {
                anchors.fill: parent
                onClicked:
                        {
                        grid.currentIndex = -1
                        screen_touch();
                       }
                onPressed:
                       {
                       grid.currentIndex = -1
                       screen_touch();
                       }
                }
        }


    }

