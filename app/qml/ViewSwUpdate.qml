import QtQuick 2.7
import QtMultimedia 5.8
import "colibri/"

Rectangle {
      id:viewSwUpdatei
      width:800
      height:480
      property string start_stop:viewSwUpdate_start_stop

      Timer {
            id: timeri
                interval: 1000; repeat:false;running:false
                onTriggered:{
                    signal_process_set_SwUpdate("init");
                    webbrowser_setup.stop()
                    start_stop = viewSwUpdate_start_stop
                }
      }

      function timer_start() {timeri.start()}

      function clear()
      {
         ok_SwUpdatei.opacity=0;
         //keyboard_num.text=""; keyboard_alpha.text=""; keyboard_num.opacity=0;keyboard_alpha.opacity=0;*
      }

      function mode_SwUpdate()
      {
          webbrowser_setup.stop();
          timeri.start()
          viewSwUpdatei.clear();
      }

      Column {
         spacing: 15
         Row {
             spacing: 10
         }
         Row {
             spacing: 20


              CLButton{
                  id: sSwUpdatei
                  text:qsTr(" SWUPDATE ")
                  fontSize: 20
                  onClicked:
                  {
                   }
                }

               Image {
                      id:ok_SwUpdatei
                      opacity:0
                      height:source.height
                      width: source.width
                      source :"qrc:/qml/content/Ok.png"
                      MouseArea {
                                  anchors.fill: parent
                                  onClicked:mode_SwUpdate()

                                 }

                  }
             }
         Row {
             spacing: 50
             opacity:1
             CLButton{
                 id: bSwUpdatestart
                 text:qsTr(" Start ")
                 fontSize: 20
                 color: "#00FF00"
                 gradient: nullGradient
                 onClicked:
                 {viewid9_timer.start();
                     signal_process_set_SwUpdate("start")

                 }
             }
         }
         Row {
             opacity:1
             CLButton{
                 id: bSwUpdatestop
                 text:qsTr(" Stop ")
                 fontSize: 20
                 color: "#FF0000"
                 gradient: nullGradient
                 onClicked:
                 {viewid9_timer.start();
                     signal_process_set_SwUpdate("stop")

                 }
             }
         }

      }


}

