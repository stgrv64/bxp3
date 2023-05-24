import QtQuick 2.7
import QtMultimedia 5.8
import "colibri/"

Rectangle {
      id:viewSSHi
      width:800
      height:480
      property string start_stop:viewSSH_start_stop

      Timer {
            id: timeri
                interval: 1000; repeat:false;running:false
                onTriggered:{
                    signal_process_set_SSH("init");
                    webbrowser_setup.stop()
                    start_stop = viewSSH_start_stop
                }
      }

      function timer_start() {timeri.start()}

      function clear()
      {
         ok_SSHi.opacity=0;
      }


      function mode_SSH()
      {
          webbrowser_setup.stop();
          timeri.start()
          viewSSHi.clear();
      }

      Column {
         spacing: 15
         Row {
             spacing: 10
         }
         Row {
             spacing: 20


              CLButton{
                  id: sSSHi
                  text:qsTr(" SERVEUR SSH ")
                  fontSize: 20
                  onClicked:
                  {
                   }
                }

               Image {
                      id:ok_SSHi
                      opacity:0
                      height:source.height
                      width: source.width
                      source :"qrc:/qml/content/Ok.png"
                      MouseArea {
                                  anchors.fill: parent
                                  onClicked:mode_SSH()

                                 }

                  }
             }
         Row {
             spacing: 50
             opacity:1
             CLButton{
                 id: bSSHstart
                 text:qsTr(" Start ")
                 fontSize: 20
                 color: "#00FF00"
                 gradient: nullGradient
                 onClicked:
                 {viewid9_timer.start();
                     signal_process_set_SSH("start")

                 }
             }
         }
         Row {
             opacity:1
             CLButton{
                 id: bSSHstop
                 text:qsTr(" Stop ")
                 fontSize: 20
                 color: "#FF0000"
                 gradient: nullGradient
                 onClicked:
                 {viewid9_timer.start();
                     signal_process_set_SSH("stop")

                 }
             }
         }

      }


}

