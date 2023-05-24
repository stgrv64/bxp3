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
    id:viewipi
    width:800
    height:480
    property string nom:viewip_nom
    property string ip:viewip_ip
    property string netmask:viewip_netmask
    property string gateway:viewip_gateway
    property string dns1:viewip_dns1
    property string dns2:viewip_dns2

    Timer {
        id: timeri
        interval: 1000; repeat:false;running:false
        onTriggered:{
            signal_window_set_proxy_web("");
            signal_x7GestConfig_set_mode_network(-1,"","","","","","");
            webbrowser_setup.run("http://127.0.0.1/setup/ip");
            nom = viewip_nom
            ip = viewip_ip
            netmask = viewip_netmask
            gateway = viewip_gateway
            dns1 = viewip_dns1
            dns2 = viewip_dns2
        }
    }

    function timer_start() {timeri.start()}


    function clear()
    {
        dhcp.opacity=1;statique.opacity=1;ok_dhcp.opacity=0;ok_statique.opacity=0;
        r0_text.state ='Off';r1_text.state ='Off';r2_text.state ='Off';r3_text.state ='Off';r4_text.state ='Off';r5_text.state ='Off';
        keyboard_num.text=""; keyboard_alpha.text=""; keyboard_num.opacity=0;keyboard_alpha.opacity=0;
    }


    function mode_network(mode)
    {
        webbrowser_setup.stop();
        if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
        if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
        if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
        if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
        if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
        if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
        //           nom=nom_base+nom
        signal_x7GestConfig_set_mode_network(mode,nom,ip,netmask,gateway,dns1,dns2)
        timeri.start()
        viewipi.clear();
    }

    Column {
        spacing: 15
        Row {
            spacing: 10
        }

        Row {
            spacing: 20

            CLButton{
                id: dhcp
                text: qsTr("  DHCP  ")
                fontSize: 20
                onClicked:
                {   viewid9_timer.stop();
                    if (ok_dhcp.opacity ==0)
                    {
                        statique.opacity=0
                        ok_dhcp.opacity=1
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                    }
                    else
                    {
                        clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/ip");
                    }
                }
            }

            Image {
                id:ok_dhcp
                opacity:0
                height:source.height
                width: source.width
                source :"qrc:/qml/content/Ok.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed:{ok_dhcp.opacity=0;viewid9_timer.stop();}
                    onReleased: mode_network(0)

                }

            }

            CLButton{
                id:statique
                text: qsTr("STATIQUE")
                fontSize: 20
                onClicked:
                {   viewid9_timer.stop();
                    if (ok_statique.opacity==0)
                    {
                        ok_statique.opacity=1;
                        dhcp.opacity=0
                        ok_dhcp.opacity=0;
                        webbrowser_setup.stop();
                        viewid9_timer.start()
                    }
                    else
                    {
                        clear()
                        signal_window_set_proxy_web("");webbrowser_setup.run("http://127.0.0.1/setup/ip");
                    }
                }
            }

            Image {
                id:ok_statique
                opacity:0
                height:source.height
                width: source.width
                source :"qrc:/qml/content/Ok.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed:{ok_statique.opacity=0;viewid9_timer.stop();}
                    onReleased: mode_network(1)
                }

            }
        }

        Row {
            spacing: 20
            opacity:ok_dhcp.opacity | ok_statique.opacity
            Text{
                id:r0_text
                text:qsTr("Nom       : ")
                state:"Off"
                color: r0_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r0_text.state =='On')
                        {
                            r0_text.state ='Off';
                            nom=keyboard_alpha.text;
                        }
                        else
                        {
                            if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
                            if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
                            if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
                            if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
                            if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
                            r0_text.state ='On';
                            keyboard_alpha.opacity=1
                            keyboard_num.opacity=0
                            keyboard_alpha.enabled=1
                            keyboard_num.enabled=0
                            keyboard_alpha.maxlength=25
                            keyboard_alpha.text=nom
                        }
                    }
                }
            }

            TextInput {
                text: r0_text.state =='Off' ? nom : keyboard_alpha.text
                maximumLength:25
                cursorVisible: r0_text.state =='On'? true:false
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
            }

        }

        Row {
            spacing: 20
            opacity: ok_statique.opacity
            Text{
                id:r1_text
                text:qsTr("Adresse IP: ")
                state:"Off"
                color: r1_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r1_text.state =='On')
                        {
                            r1_text.state ='Off';
                            ip=keyboard_num.text;
                        }
                        else
                        {
                            if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
                            if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
                            if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
                            if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
                            if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
                            r1_text.state ='On';
                            keyboard_num.maxlength=15
                            keyboard_num.text=ip
                            keyboard_alpha.opacity=0;
                            keyboard_num.opacity=1
                            keyboard_alpha.enabled=0;
                            keyboard_num.enabled=1
                        }

                    }
                }
            }

            TextInput {
                text: r1_text.state =='Off' ? ip : keyboard_num.text
                maximumLength:15
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
                cursorVisible: r1_text.state =='On'? true:false

            }

        }

        Row {
            spacing: 20
            opacity: ok_statique.opacity

            Text{
                id:r2_text
                text:qsTr("Netmask   : ")
                state:"Off"
                color: r2_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r2_text.state =='On')
                        {
                            r2_text.state ='Off';
                            netmask=keyboard_num.text;
                        }
                        else
                        {
                            if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
                            if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
                            if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
                            if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
                            if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
                            r2_text.state ='On';
                            keyboard_num.maxlength=15
                            keyboard_num.text=netmask
                            keyboard_alpha.opacity=0;
                            keyboard_num.opacity=1
                            keyboard_alpha.enabled=0;
                            keyboard_num.enabled=1
                        }

                    }
                }
            }

            TextInput {
                text: r2_text.state =='Off' ? netmask : keyboard_num.text
                maximumLength:15
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
                cursorVisible: r2_text.state =='On'? true:false
            }

        }

        Row {
            spacing: 20
            opacity:ok_statique.opacity

            Text{
                id:r3_text
                text:qsTr("Gateway  :")
                state:"Off"
                color: r3_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r3_text.state =='On')
                        {
                            r3_text.state ='Off';
                            gateway=keyboard_num.text;
                        }
                        else
                        {
                            if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
                            if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
                            if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
                            if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
                            if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
                            r3_text.state ='On';
                            keyboard_num.maxlength=15
                            keyboard_num.text=gateway
                            keyboard_alpha.opacity=0;
                            keyboard_num.opacity=1
                            keyboard_alpha.enabled=0;
                            keyboard_num.enabled=1
                        }

                    }
                }
            }

            TextInput {
                text: r3_text.state =='Off' ? gateway : keyboard_num.text
                maximumLength:15
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
                cursorVisible: r3_text.state =='On'? true:false
            }

        }

        Row {
            spacing: 20
            opacity:ok_statique.opacity

            Text{
                id:r4_text
                text:qsTr("Dns1     :")
                state:"Off"
                color: r4_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r4_text.state =='On')
                        {
                            r4_text.state ='Off';
                            dns1=keyboard_num.text;
                        }
                        else
                        {
                            if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
                            if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
                            if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
                            if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
                            if ( r5_text.state =='On')  {r5_text.state ='Off';dns2=keyboard_num.text}
                            r4_text.state ='On';
                            keyboard_num.maxlength=15
                            keyboard_num.text=dns1
                            keyboard_alpha.opacity=0;
                            keyboard_num.opacity=1
                            keyboard_alpha.enabled=0;
                            keyboard_num.enabled=1
                        }

                    }
                }
            }

            TextInput {
                text: r4_text.state =='Off' ? dns1 : keyboard_num.text
                maximumLength:15
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
                cursorVisible: r4_text.state =='On'? true:false
            }

        }

        Row {
            spacing: 20
            opacity:ok_statique.opacity

            Text{
                id:r5_text
                text:qsTr("Dns2     :")
                state:"Off"
                color: r5_text.state =='Off' ? "blue" : "#e89101"
                width:text.width
                font.family: "Helvetica"
                font.pixelSize: 30

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {   viewid9_timer.stop();
                        if ( r5_text.state =='On')
                        {
                            r5_text.state ='Off';
                            dns2=keyboard_num.text;
                        }
                        else
                        {
                            if ( r0_text.state =='On')  {r0_text.state ='Off';nom=keyboard_alpha.text}
                            if ( r1_text.state =='On')  {r1_text.state ='Off';ip=keyboard_num.text}
                            if ( r2_text.state =='On')  {r2_text.state ='Off';netmask=keyboard_num.text}
                            if ( r3_text.state =='On')  {r3_text.state ='Off';gateway=keyboard_num.text}
                            if ( r4_text.state =='On')  {r4_text.state ='Off';dns1=keyboard_num.text}
                            r5_text.state ='On';
                            keyboard_num.maxlength=15
                            keyboard_num.text=dns2
                            keyboard_alpha.opacity=0;
                            keyboard_num.opacity=1
                            keyboard_alpha.enabled=0;
                            keyboard_num.enabled=1
                        }

                    }
                }
            }

            TextInput {
                text: r5_text.state =='Off' ? dns2 : keyboard_num.text
                maximumLength:15
                font.family: "Helvetica"
                font.pixelSize: 30
                color: "black"
                cursorVisible: r5_text.state =='On'? true:false
            }

        }

    }
}
