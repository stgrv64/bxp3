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



Rectangle{
    id : viewsetup
    property int model
    property string viewip_nom
    property string viewip_ip
    property string viewip_netmask
    property string viewip_gateway
    property string viewip_dns1
    property string viewip_dns2

    property string viewlecteur_lecteur
    property string viewlecteur_option
    property string viewlecteur_masque
    property string viewlecteur_format

    property string viewhttp_serveur
    property string viewhttp_https
    property string viewhttp_http
    property string viewhttp_service
    property string viewhttp_user
    property string viewhttp_password
    property string viewhttp_timeout

    property string viewntp_serveur
    property string viewntp_cadence
    property string viewntp_timezone
    property string viewntp_langue

    property string viewdivers_ecran
    property string viewdivers_bl1
    property string viewdivers_delai
    property string viewdivers_bl2
    property string viewdivers_delai_off
    property string viewdivers_sleep_mode

    property string viewwifi_ssid
    property string viewwifi_psk
    property string viewwifi_password


    property string view3G_apn
    property string view3G_pin
    property string view3G_phone
    property string view3G_user
    property string view3G_password
    property string view3G_opt1
    property string view3G_opt2

    property string viewSSH_start_stop

    property string viewSwUpdate_start_stop

   property int index:0

    height:main.orientation == 0 ? 480 :800
    width: main.orientation == 0 ? 800 :480

    function init(){
        index=0
        clear()
        row.showPage()
    }

    function next()
        {
        if (index==0) {index=-1;viewip.clear();viewhttp.timer_start();}
        else if (index==-1) {index=-2;viewhttp.clear();viewlecteur.timer_start();}
        else if (index==-2) {index=-3;viewlecteur.clear();viewntp.timer_start();}
        else if (index==-3) {index=-4;viewntp.clear();viewdivers.timer_start();}
        else if (index==-4) {index=-5;viewdivers.clear();viewwifi.timer_start();}
        else if (index==-5) {index=-6;viewwifi.clear();view3G.timer_start();}
        else if (index==-6) {index=-7;view3G.clear();viewSSH.timer_start();}
        else if (index==-7) {index=-8;viewSSH.clear();viewSwUpdate.timer_start();}
        else if (index==-8) {index=0;viewSwUpdate.clear();viewip.timer_start();}
        ;row.showPage();viewid9_timer.stop();viewid9_timer.start();
        }

    function clear()
    {
        index=0;viewip.clear();
        viewhttp.clear();
        viewlecteur.clear();
        viewntp.clear();
        viewdivers.clear();
        viewwifi.clear();
        view3G.clear();
        viewSSH.clear();
        viewSwUpdate.clear();
        viewip.timer_start();
    }

    Row {
      id: row
      spacing: 0
      function showPage() { row.x = index*viewsetup.width }
      Behavior on x { NumberAnimation {} }

      ViewIp{
          id:viewip
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
          }

      ViewHttp{
          id:viewhttp
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480

      }
      ViewLecteur{
          id:viewlecteur
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      ViewNtp{
          id:viewntp
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      ViewDivers{
          id:viewdivers
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      ViewWifi{
          id:viewwifi
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      View3G{
          id:view3G
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      ViewSSH{
          id:viewSSH
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
      ViewSwUpdate{
          id:viewSwUpdate
          color: "#FFFFFF"
          height:main.orientation == 0 ? 480 :800
          width: main.orientation == 0 ? 800 :480
        }
    }

    Webbrowser {
           id:webbrowser_setup
           width: 480
           height:370
           anchors.left:parent.left
           anchors.bottom:parent.bottom
           anchors.horizontalCenter: parent.horizontalCenter
           }

    NumericKeyboard {
       id:keyboard_num
 //      opacity:keyboard_opacity
//       opacity: (ok_dhcp.opacity | ok_statique.opacity) & kbnum
       anchors.bottom: parent.bottom
       anchors.right: parent.right
       anchors.bottomMargin: 10
       anchors.rightMargin: 10
      }

    AlphaKeyboard {
       id:keyboard_alpha
//       opacity: (ok_dhcp.opacity | ok_statique.opacity) & kbalpha
//       opacity:keyboard1_opacity
       anchors.bottom: parent.bottom
       anchors.right: parent.right
       anchors.bottomMargin: 2
       anchors.rightMargin: 0
       width:480
       height:200
     }


}


    /* repeater {
        model:rowModel
        delegate: Rectangle {
          width: layout.width
          height: layout.height
          Loader { source: qmlpath }
        }
      }
    }

  ListView{
        id:rowLayout
        x: 0
        y: 29
        width: layout.width
        height: layout.height-80
        cacheBuffer: 0
        flickDeceleration: 5000
        maximumFlickVelocity: 5000
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem

        model: rowModel
        delegate: rowDelegate
        focus: true
        }

  gradient: Gradient {
        GradientStop {
            position: 0
            color: "#000000"
        }

        GradientStop {
            position: 0.5
            color: "#343330"
        }

        GradientStop {
            position: 1
            color: "#000000"
        }
    }

    ListModel{
        id : rowModel
        ListElement{ qmlpath:"ViewHttp.qml"}
        ListElement{ qmlpath:"ViewIp.qml"}
        ListElement{ qmlpath:"ViewLecteur.qml"}
        ListElement{ qmlpath:"Qml3.qml"}
        ListElement{ qmlpath:"Qml4.qml"}
        ListElement{ qmlpath:"Qml5.qml"}
        ListElement{ qmlpath:"Qml6.qml"}
    }

    Component{
        id:rowDelegate
        Row{
            Loader{
                source: qmlpath
            }
        }
    }

//    property var model
*/

