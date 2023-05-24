import QtQuick 2.7
import QtMultimedia 5.8
import QtQuick.Controls 2.0


Item {
    id: main
    width: 800; height: 480
    state: "view0"


    //property int simulation:0
    property int orientation
    property int angle
    property string webbrowser_orientation

    property string webbrowser_tempo
    property string webbrowser_server
    property string webbrowser_port_http
    property string webbrowser_port_https
    property string webbrowser_url
    property string webbrowser_proxy
    property string browser_url
    /*  property string badge_text
  property string badge_encode_text
  property string badge_date
  property string badge_heure
  */
    property string touche_activite
    property int touche_index

    property string message1_titre
    property string message1_text
    property string message2_titre
    property string message2_text
    property string message2_text1
    property string message3_titre
    property string message3_text
    property string message3_text1
    property string message3_c1
    property string message3_c2
    property string message3_c3
    property string message3_c4
    property string dernier_badge



    /*  property int alerteopacity
  property bool alerteenabled
*/
    /*
  Repeater {
         id:tr0
         model:touchesfonctionmodel
  }
*/
    property string heure
    property string date
    property string année
    property string date_sans_année
    property string date_numerique

    function timeChanged() {
        heure=Qt.formatTime(new Date(), "hh:mm:ss")
        date= Qt.formatDate(new Date(), "dddd d MMMM yyyy")
        date_sans_année= Qt.formatDate(new Date(), "dddd d MMMM")
        année= Qt.formatDate(new Date(), "yyyy")
        date_numerique= Qt.formatDate(new Date(), "d/MM/yyyy")
    }

    Timer {
        interval: 1000; running: true; repeat: true;
        onTriggered: main.timeChanged()
    }




    Rectangle
    {
        // id:mainview
        color: "black"
        x: main.orientation == 0 ? 0 : +160
        y: main.orientation == 0 ? 0 : -160
        width: 800
        height: 480
        transform: Rotation {
            origin.x:  main.orientation == 0 ? 400 : 240
            origin.y: main.orientation == 0 ? 240 : 400
            axis.x:0; axis.y:0; axis.z:1
            angle: main.angle
        }

        Rectangle{
            color: "transparent"
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {reset_touche()}
                onPressed:{reset_touche()}
            }
        }


        Rectangle {
            id : viewid0
            anchors.fill: parent
            focus:true

        }


        Rectangle {
            id : viewid1
            anchors.fill: parent
            focus:true

            function timer_init(tempo)
            {
                var t=0;
                viewid1_timer.running=false
                if (tempo.length===0) t=0
                else t=parseInt(tempo);
                if (t<1) t=1
                else t=t*1000
                viewid1_timer.interval= t;
                viewid1_timer.repeat=false;
                viewid1_timer.running=true
            }

            Timer {
                id: viewid1_timer
                interval: 10000; repeat:false;running:false
                onTriggered:{reset_touche(); message1_titre=""; message2_titre=""; message3_titre="";badge_clear();}

            }


            Image {
                opacity: main.orientation == 0 ? (nbtouchesfonction > 10 ? 1:0):(nbtouchesfonction > 8 ? 1:0)
                height:source.height
                width: source.width
                anchors.top: touchesfonction.bottom
                anchors.topMargin:main.orientation == 0 ? 5:5
                anchors.right: touchesfonction.right
                anchors.rightMargin:main.orientation == 0 ? 5:5
                source :"qrc:/qml/content/more.png"
            }



        }



        Rectangle {
            id : viewid2
            anchors.fill: parent
            color: "transparent"

            function timer_init(tempo)
            {
                var t=0;
                viewid2_timer.running=false
                if (tempo.length===0) t=0
                else t=parseInt(tempo);
                if (t<2) t=1
                else t=t*1000
                viewid2_timer.interval= t;
                viewid2_timer.repeat=false;
                viewid2_timer.running=true
            }

            Timer {
                id: viewid2_timer
                interval: 10000; repeat:false;running:false
                onTriggered:{//browser_audio.source="";
                    webbrowser_url="";webbrowser_proxy=""; browser_url=""; browserLoader.source = ""; badge_clear();  main.state="view1"}
            }


            Rectangle {
                id:horodate
                width: main.orientation == 0 ? 800 : 480
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:parent.top
                height: 40
                //            color: "#f36a25"
                color: "black"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {viewid2_timer.interval= 1}
                    onPressed: {viewid2_timer.interval= 1}
                }


                Text {
                    id:date_text
                    text: main.date_numerique
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin:main.orientation == 0 ? 10:5
                    color: "white"
                    font.family: "FreeMono"
                    font.capitalization: Font.Capitalize
                    font.italic: true
                    font.pixelSize: main.orientation == 0 ? 25 : 23
                }


                Text {
                    text:main.heure
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "FreeMono"
                    font.bold: true
                    font.pixelSize: main.orientation == 0 ? 25 : 23
                    color:"white"
                }


                Image {
                    //id : exit
                    height:source.height
                    width: source.width
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right:parent.right
                    anchors.rightMargin:main.orientation == 0 ? 10:5
                    source :"qrc:/qml/content/Quit.png"

                }
            }

            Loader { id: browserLoader
                width: main.orientation == 0 ? 800 :480
                height: main.orientation == 0 ? 440 :760
                anchors.left:parent.left
                anchors.bottom:parent.bottom
            }




        }

        Rectangle {
            id : viewid3
            /*     anchors.fill: parent
     color: "transparent"

     MouseArea {
          anchors.fill: parent
          onClicked: {screen_touch()}
          }


     Image {
              //id : exit
              height:source.height
              width: source.width
              anchors.bottom:parent.bottom
              anchors.right:parent.right
              anchors.rightMargin:10
              anchors.bottomMargin:10
              source :"qrc:/qml/content/Exit.png"
              MouseArea {
                         anchors.fill: parent
                         onClicked: {main.state="view1";webbrowser_setup.stop();}
                        }

         }

  */
        }


        Rectangle {
            id : viewid4
            anchors.fill: parent
            Image {
                id : exit
                height:source.height
                width: source.width
                anchors.bottom:parent.bottom
                anchors.left:parent.left
                anchors.leftMargin:10
                anchors.bottomMargin:10
                source :"qrc:/qml/content/Exit.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {main.state="view8";}
                }

            }

        }

        Rectangle {
            id : viewid5
            anchors.fill: parent

        }

        Rectangle {
            id : viewid6
            anchors.fill: parent
            focus:true

        }


        Rectangle {
            id : viewid7
            anchors.fill: parent
            focus:true


            }
            Image {
                //id : exit
                height:source.height
                width: source.width
                anchors.top:parent.top
                anchors.right:parent.right
                anchors.rightMargin:10
                anchors.topMargin:10
                source :"qrc:/qml/content/Exit.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {main.state="view8";viewmorpho.clear();signal_morpho_cancel();signal_morpho_start()}
                }
            }

        }

        Rectangle {
            id : viewid8
            anchors.fill: parent
            focus:true


        }

        Rectangle {
            id : viewid9
            anchors.fill: parent
            focus:true
            onVisibleChanged: {viewsetup.init()}
            onActiveFocusChanged: {viewsetup.init()}
            onEnabledChanged: {viewsetup.init()}

            Image {
                //id : exit
                height:source.height
                width: source.width
                anchors.top:parent.top
                anchors.right:exit9.left
                anchors.rightMargin:10
                anchors.topMargin:10
                source :"qrc:/qml/content/next.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked:    {viewsetup.next()}
                }
            }

            Image {
                id : exit9
                height:source.height
                width: source.width
                anchors.top:parent.top
                anchors.right:parent.right
                anchors.rightMargin:10
                anchors.topMargin:10
                source :"qrc:/qml/content/Exit.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {viewsetup.clear(); main.state="view8"}
                }

            }
        }


        Rectangle {
            id : viewid10
            anchors.fill: parent
            focus:true
            color: "transparent"

            function timer_init(tempo)
            {
                var t=0;
                viewid10_timer.running=false
                if (tempo.length===0) t=0
                else t=parseInt(tempo);
                if (t<2) t=1
                else t=t*1000
                viewid10_timer.interval= t;
                viewid10_timer.repeat=false;
                viewid10_timer.running=true
            }

            Timer {
                id: viewid10_timer
                interval: 10000; repeat:false;running:false
                onTriggered:{reset_touche();videoPlayerItem.stopVideo();  badge_clear();  main.state="view1"}
            }


            Rectangle {
                id:horodate_video
                width: main.orientation == 0 ? 800 : 480
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:parent.top
                height: 40
                //           color: "#f36a25"
                color: "black"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {viewid10_timer.interval= 1}
                    onPressed: {viewid10_timer.interval= 1}
                }


                Text {
                    id:date_text_video
                    text: main.date_numerique
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin:main.orientation == 0 ? 10:5
                    color: "white"
                    font.family: "FreeMono"
                    font.capitalization: Font.Capitalize
                    font.italic: true
                    font.pixelSize: main.orientation == 0 ? 25 : 23
                }


                Text {
                    text:main.heure
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "FreeMono"
                    font.bold: true
                    font.pixelSize: main.orientation == 0 ? 25 : 23
                    color:"white"
                }


                Image {
                    //id : exit
                    height:source.height
                    width: source.width
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right:parent.right
                    anchors.rightMargin:main.orientation == 0 ? 10:5
                    source :"qrc:/qml/content/Quit.png"

                }
            }


            Rectangle {
                id: videoPlayerItem
                width: main.orientation == 0 ? 800 :480
                height: 440
                //height: main.orientation == 0 ? 440 :760
                anchors.top: horodate_video.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                //main.orientation == "0" ? 10 : 20
                color: "transparent"
                enabled: false
                opacity: 0

                function playVideo(nom){
                    videoPlayer.volume=1
                    videoPlayer.source=nom
                    videoPlayer.play()
                    videoPlayerItem.opacity=1;
                    videoPlayerItem.visible = true;
                }

                function stopVideo(){
                    videoPlayer.stop()
                    videoPlayer.source="";
                    videoPlayerItem.opacity=0
                    videoPlayerItem.enabled = false
                }

                function pauseVideo(){
                    videoPlayer.pause();

                }

                MouseArea{
                    anchors.fill: parent
                    onClicked:
                        videoPlayerItem.stopVideo()
                }


            }


        }



        Rectangle {
            id : viewid11
            anchors.fill: parent
            color: "transparent"

        }



    }


