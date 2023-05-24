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
import QtQuick.Controls 2.0
import "content"


Item {
    id: main
    width: 800; height: 480
    state: "view0"
    objectName : "MainView"

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



    // SIGNAUX

    signal signal_supervision_ihm_post_badge(string text,string d,string t,string a);
    signal signal_window_set_proxy_web(string proxy);
    signal signal_qml_set_date_heure(string an,string mois,string jour,string heure,string minute,string seconde);
    signal signal_qml_ts_calibrate();
    signal signal_qml_ts_calibrate_init();
    signal signal_screen_touch();
    signal signal_window_orientation_ecran();
    signal audio_set_volume(string a);
    signal badge_clear();
    //  signal signal_viewip(string ip,string netmask,string gateway);

    // Signaux MORPHO
    signal signal_morpho_enrolluser(string text);
    signal signal_morpho_deleteuserindex(string text);
    signal signal_morpho_deletealluser();
    signal signal_morpho_cancel();
    signal signal_morpho_destroy_database();
    signal signal_morpho_create_database();
    signal signal_morpho_getbaseconfig();
    signal signal_morpho_reload_database();
    signal signal_morpho_stop();
    signal signal_morpho_start();

    // Signaux play audio
    signal signal_audio_play(string audio);

    // Signaux ViewLaunchSecurit
    signal qml_message(string message);

    // Signaux viewip
    signal signal_x7GestConfig_set_mode_network(int mode,string nom,string ip,string netmask,string gateway,string dns1,string dns2);

    // Signaux viewlecteur
    signal signal_ihm_process_set_lecteur_badge(string lecteur,string option,string format,string masque);

    // Signaux viewhttp
    signal signal_ihm_process_set_http(string serveur,string https,string http,string service,string user,string password,string timeout);

    // Signaux viewntp
    signal signal_ihm_process_set_ntp(string serveur,string cadence,string timezone,string langue);

    // Signaux viewdivers
    signal signal_ihm_process_set_divers(string ecran,string bl1,string delai,string bl2,string delai_off,string sleep_mode);

    // Signaux viewwifi
    signal signal_X7GestConfig_setWifi(string ssid, string psk);
    //signal signal_set_wifi_network(string ssid,string psk);

    // Signaux view3G
    signal signal_process_set_3G(string apn,string pin,string phone,string user,string password);

    // Signaux viewSSH
    signal signal_process_set_SSH(string start_stop);

    // Signaux viewSwUpdate
    signal signal_process_set_SwUpdate(string start_stop);

    // SIGNAUX test
    signal signal_time2bii_tests(int a);

    // SLOTS
    function screen_touch()
    {
        signal_screen_touch();
        if(main.state==="view2") viewid2.timer_init(webbrowser_tempo);
        if(main.state==="view9") viewid9.timer_init(30);
    }


    function viewid2_timer_init()
    {//browser popup fonction
        if(main.state==="view2") viewid2.timer_init(2);
    }

    // SLOT ViewIp
    function ihm_init_mode_network(nom0,ip0,netmask0,gateway0,dns10,dns20)
    {
        //    console.debug("init mode net"+ ip0)
        viewsetup.viewip_nom=nom0
        viewsetup.viewip_ip=viewbase.adresseip=ip0
        viewsetup.viewip_netmask=netmask0
        viewsetup.viewip_gateway=gateway0
        viewsetup.viewip_dns1=dns10
        viewsetup.viewip_dns2=dns20
    }

    // SLOT ViewLecteur
    function ihm_init_lecteur_badge(lecteur0,option0,format0,masque0)
    {
        viewsetup.viewlecteur_lecteur=lecteur0
        viewsetup.viewlecteur_option=option0;
        viewsetup.viewlecteur_format=format0
        viewsetup.viewlecteur_masque=masque0
    }

    // SLOT ViewHttp
    function ihm_init_http(serveur0,https0,http0,service0,user0,password0,timeout0)
    {
        viewsetup.viewhttp_serveur=serveur0
        viewsetup.viewhttp_https=https0
        viewsetup.viewhttp_http=http0
        viewsetup.viewhttp_service=service0
        viewsetup.viewhttp_user=user0
        viewsetup.viewhttp_password=password0
        viewsetup.viewhttp_timeout=timeout0
    }

    // SLOT ViewNtp
    function ihm_init_ntp(serveur0,cadence0,timezone0,langue0)
    {
        viewsetup.viewntp_serveur=serveur0
        viewsetup.viewntp_cadence=cadence0
        viewsetup.viewntp_timezone=timezone0
        viewsetup.viewntp_langue=langue0
    }
    // SLOT ViewDivers
    function ihm_init_divers(ecran0,bl10,delai0,bl20,delai_off0,sleep_mode0)
    {
        viewsetup.viewdivers_ecran=ecran0;
        viewsetup.viewdivers_bl1=bl10;
        viewsetup.viewdivers_delai=delai0;
        viewsetup.viewdivers_bl2=bl20;
        viewsetup.viewdivers_delai_off=delai_off0;
        viewsetup.viewdivers_sleep_mode=sleep_mode0
    }

    // SLOT ViewWifi
    function ihm_init_wifi(ssid0, psk0)
    {
        viewsetup.viewwifi_ssid=ssid0
        viewsetup.viewwifi_psk=psk0
    }

    // SLOT View3G
    function ihm_init_3G(apn0,pin0,phone0,user0,password0)
    {
        viewsetup.view3G_apn=apn0
        viewsetup.view3G_pin=pin0
        viewsetup.view3G_phone=phone0
        viewsetup.view3G_user=user0
        viewsetup.view3G_password=password0

    }

    // SLOT ViewSSH
    function ihm_init_SSH(start_stop0)
    {
        viewsetup.viewSSH_start_stop=start_stop0
    }

    // SLOT ViewSwUpdate
    function ihm_init_SwUpdate(start_stop0)
    {
        viewsetup.viewSwUpdate_start_stop=start_stop0
    }

    function ihm_http_status(a)
    {
        a=1-a // inversion valeur
        //      console.debug(a)
        viewbase.alerte_http_opacity=a
        viewbase.alerte_http_enabled=a
    }


    function ihm_badge_db_status(a)
    {
        a=1-a // inversion valeur
        //      console.debug(a)
        viewbase.alerte_badge_db_opacity=a
        viewbase.alerte_badge_db_enabled=a
    }

    function usbkey_status(a)
    {
        //console.debug("usbkey status "+ a)
        a=a%2 // modulo 2 car valeurs possibles 0,1 et 2
        viewbase.usbkey_opacity=a
        viewbase.usbkey_enabled=a
    }

    function retour_test(a,b)
    {
        if (a===1) {
            viewtests.color_tests[b].color="#00FF00"
        }
        else if(!a) {
            viewtests.color_tests[b].color="#FF0000"
        }
        else if(a===2) {
            viewtests.color_tests[b].color="#EFEFEF"
        }
        else  {
            viewtests.color_tests[b].color="transparent"
        }
        if (b===999) {viewtests.play_test_audio()}
    }

    function http_config(a,b,c,d)
    {
        //      console.debug("http config "+ a)
        viewbase.adresseip=a
        webbrowser_server=b
        webbrowser_port_https=c
        webbrowser_port_http=d
    }

    function ihm_orientation_ecran(a)
    {
        main.orientation=a%2
        main.angle=(a*90)
    }

    function morpho_message(text)
    {
        viewmorpho.message=text;
    }

    function morpho_status(a)
    {
        a=1-a // inversion valeur
        //      console.debug(a)
        viewbase.alerte_morpho_db_opacity=a
        viewbase.alerte_morpho_db_enabled=a
    }

    function morpho_enabled()
    {
        viewlaunchsecurit.morpho_enabled=1
    }



    function browser(url,proxy,audio,tempo)
    {
        console.debug("MainView function browser url" + url)
        webbrowser_tempo=tempo
        browserLoader.source = "";
        browser_url="";
        if (url.length >0)
        {
            console.debug("MainView function browser proxy : " + proxy)
            signal_window_set_proxy_web(proxy);
            console.debug("MainView function browser proxy set " + proxy)
            if (url.indexOf("http") === 0 ) { browser_url= url;}
            else
                browser_url= "http://"+url;
            console.debug("MainView function browser browser_url " + browser_url)
            browserLoader.source = "Browser.qml";
            //        webbrowserid.run(webbrowser_url);
            console.debug("MainView function Browser.qml started ")
   /*         if (audio.length >0)
            {
                //         browser_audio.source=audio
                //          browser_audio.play()
                signal_audio_play(audio)
            }*/
            console.debug("MainView function browser set view2")
            main.state="view2";
            viewid2.timer_init(tempo);
        }
        else viewid2.timer_init(1);
    }

    function video(nom,tempo)
    {
        webbrowser_url="";webbrowser_proxy="";
        if (main.state=="view2") browserLoader.source ="";browser_url="" ;
        videoPlayerItem.stopVideo();
        if (nom.length >0)
        {
            videoPlayerItem.playVideo(nom);
            main.state="view10";
            viewid10.timer_init(tempo);
        }
        else viewid10.timer_init(1);
    }


    function display_message(titre1,texte1,titre2,texte2,texte2_1,titre3,texte3,texte3_1,c1,c2,c3,c4,badgeMes,audio,tempo)
    {
        if (badgeMes==="" || badgeMes === dernier_badge)
        {
            message1_titre=""
            message2_titre=""
            message3_titre=""
            //       message_audio.source=""
            //       message_audio.stop()
            if ((titre1.length >0 || titre2.length >0 || titre3.length >0) && main.state=="view2" ) {viewid2.timer_init(1)}
            message1_titre=titre1
            message1_text=texte1
            message2_titre=titre2
            message2_text=texte2
            message2_text1=texte2_1
            message3_titre=titre3
            message3_text=texte3
            message3_text1=texte3_1
            message3_c1=c1
            message3_c2=c2
            message3_c3=c3
            message3_c4=c4
            if (audio.length >0)
            {
                signal_audio_play(audio)
                //          message_audio.source=audio
                //          message_audio.play()
            }
            viewid1.timer_init(tempo);
            badge.stop_animation();
        }

    }

    function ihm_badge_message(text)
    {
        badge.attente_badge_empreinte(text)
    }


    function reset_touche()
    {
        console.debug("MainView function reset_touche()")
        screen_touch();
        webbrowser_url="";webbrowser_proxy=""
        touche_activite='-'
        touchesfonction.reset();
        viewbase.adresseip_opacity=false;viewbase.adresseip_enabled=false
    }


    function appui_touche(activite,url,proxy,tempo)
    {
        // message_audio.source="";
        message1_titre="";message2_titre="";message3_titre="";
        webbrowser_url="";webbrowser_proxy="";

        viewid1.timer_init(15)
        touche_activite=activite
        if (url.length >0)
        {
            webbrowser_tempo=tempo;
            webbrowser_proxy=proxy;
            signal_window_set_proxy_web(proxy);
            if (url.indexOf("http") === 0 ) { webbrowser_url= url;}
            else
            {
                var pos = url.indexOf("/");
                if (pos !== 0) url="/"+url;
                if (webbrowser_port_https !="")
                { webbrowser_url= "https://"+webbrowser_server+":"+webbrowser_port_https+url;

                }
                else
                    webbrowser_url= "http://"+webbrowser_server+":"+webbrowser_port_http+url;
            }
        }
    }
    function new_Badge(text,date,heure,badge_encode,nomPre)
    {
        //    console.debug("badge: "+text+ " activite: "+ touche_activite+ " URL: "+webbrowser_url)

        dernier_badge=text
        message1_titre="";message2_titre="";message3_titre=""
        if (main.state==="view2" )  viewid2.timer_init(1)
        else if (main.state==="view10" ) viewid10.timer_init(1)

        /* SIMULATION */
        /*     webbrowser_tempo=2
       if (simulation===0 ) webbrowser_url=""
        if (simulation===1 ) {webbrowser_url="http://frortvdev002:18081/time2bii/demander.html?numero=";simulation=-1;}
         simulation++
   *//*   if (simulation===2 ) webbrowser_url="http://172.16.66.109/time_web/demander.html?numero="
         if (simulation===3 ) webbrowser_url="http://172.16.66.109/time_web/consulter.html?numero="
         if (simulation===4 ){ webbrowser_url="http://172.16.66.109/time_web/calendrier.html?numero=";simulation=-1;}

     webbrowser_url="http://frortvdev002:18081/time2bii/demander.html?numero="
     webbrowser_url="http://172.16.65.7/setup/ip"
     /*             */

        if(nomPre.length)
            badge.new_badge(nomPre,1);
        else
            badge.new_badge(text,1);

        if (webbrowser_url.length===0)
        {
            if (touche_activite=='-') signal_supervision_ihm_post_badge(text,date,heure,"F0");
            else signal_supervision_ihm_post_badge(text,date,heure,touche_activite);
        }

        else
        {
            var tmp;
            if (webbrowser_url.indexOf("www") <  0)
            {
                if (orientation ==0 || orientation==2) tmp="&m=h"
                else tmp="&m=v"
                if (webbrowser_url.search("numero=") !=-1) webbrowser_url=webbrowser_url+badge_encode+tmp
                else webbrowser_url=webbrowser_url
                //                 webbrowser_url=webbrowser_url+"NDcwMTU1"+tmp
            }
            //          console.debug("URL: "+webbrowser_url+" proxy: "+webbrowser_proxy+" activite: "+ touche_activite)
            browser(webbrowser_url,webbrowser_proxy,"",webbrowser_tempo)
        }
        reset_touche()
    }



    function erreur_Badge()
    {
        message1_titre="";message2_titre="";message3_titre=""
        if (main.state==="view2" )  viewid2.timer_init(1)
        else if (main.state==="view10" ) viewid10.timer_init(1)
        reset_touche()
        badge.erreur_badge()
        dernier_badge=""
    }


    states: [
        State {
            name: "view0"
            PropertyChanges {target: viewid0; visible: true}
            PropertyChanges {target: viewid1; visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false }
            PropertyChanges {target: viewid4;visible: false }
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view1"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1; visible: true}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view2"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: true}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view3"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: true}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view4"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: true}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view5"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: true}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view6"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: true}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view7"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: true}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view8"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: true}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view9"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: true}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view10"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: true}
            PropertyChanges {target: viewid11;visible: false}
        },
        State {
            name: "view11"
            PropertyChanges {target: viewid0; visible: false}
            PropertyChanges {target: viewid1;visible: false}
            PropertyChanges {target: viewid2; visible: false}
            PropertyChanges {target: viewid3;visible: false}
            PropertyChanges {target: viewid4;visible: false}
            PropertyChanges {target: viewid5;visible: false}
            PropertyChanges {target: viewid6;visible: false}
            PropertyChanges {target: viewid7;visible: false}
            PropertyChanges {target: viewid8;visible: false}
            PropertyChanges {target: viewid9;visible: false}
            PropertyChanges {target: viewid10;visible: false}
            PropertyChanges {target: viewid11;visible: true}
        }
    ]
    onStateChanged:{console.log("MainView onStateChanged");reset_touche()}


    //Rectangle Main
    Rectangle
    {
        // id:mainview
        width: main.orientation == 0 ? 800 : 480
        height: main.orientation == 0 ? 480 : 800
        color: "black"
        x: main.orientation == 0 ? 0 : +160
        y: main.orientation == 0 ? 0 : -160
        //width: 800
        //height: 480
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
     /*   Text{
            color: "red"
            text: "Hello World"
        }
*/
        Rectangle {
            id : viewid0
            anchors.fill: parent
            focus:true

            ViewIntro {
                id:viewintro
            }
        }//Fin viewid0


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

            ViewBase
            {
                id:viewbase
            }

            TouchesFonction {
                id:touchesfonction
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
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

            ViewMessage {
                id:message
            }


        }//Fin viewid1



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
                //console.debug("viewid2.timer_init tempo " + tempo)
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




        }//Fin viewid2

        Rectangle {
            id : viewid3
        }//Fin viewid3

        Rectangle {
            id : viewid4
            anchors.fill: parent
            ViewHorodate {
                id:viewhorodate
            }
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

        }//Fin viewid4

        Rectangle {
            id : viewid5
            anchors.fill: parent
            ViewTests {
                id:viewtests
            }

        }//FIn viewid5

        Rectangle {
            id : viewid6
            anchors.fill: parent
            focus:true

            ViewLaunch {
                id:viewlaunch
            }

       }//Fin viewid6


        Rectangle {
            id : viewid7
            anchors.fill: parent
            focus:true

            ViewMorphoOutils {
                id:viewmorpho

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
            }//Fin Image Exit.png

        }//Fin viewid7

        Rectangle {
            id : viewid8
            anchors.fill: parent
            focus:true

            ViewLaunchSecurit {
                id:viewlaunchsecurit
            }

        }//Fin viewid8

        Rectangle {
            id : viewid9
            anchors.fill: parent
            focus:true
            onVisibleChanged: {viewsetup.init()}
            onActiveFocusChanged: {viewsetup.init()}
            onEnabledChanged: {viewsetup.init()}

            function timer_init(tempo)
            {
                var t=0;
                viewid9_timer.running=false
                if (tempo.length===0) t=0
                else t=parseInt(tempo);
                if (t<1) t=1
                else t=t*1000
                viewid9_timer.interval= t;
                viewid9_timer.repeat=false;
                viewid9_timer.running=true
            }
            Timer {
                id: viewid9_timer
                interval: 30000; repeat:false;running:false
                onTriggered:{//browser_audio.source="";
                    webbrowser_url="";webbrowser_proxy=""; browser_url=""; browserLoader.source = ""; badge_clear();  main.state="view1"}
              }
            ViewSetup{
                id:viewsetup;
              }
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
        }//Fin viewid9


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


                Video{
                    id:videoPlayer
                    anchors.fill: videoPlayerItem // never forget to mention the size and position
                    focus: true

                    onStatusChanged: {
                        if(videoPlayer.status == Video.EndOfMedia) { videoPlayer.play()}
                    }
                }
            }


        }//Fin viewid10



        Rectangle {
            id : viewid11
            anchors.fill: parent
            color: "transparent"

        }//Fin viewid11

        BadgeView {
            id: badge
            anchors.top: parent.top
            anchors.topMargin: main.orientation == 0 ? 100 : 200
        }

        Audio
        {
            id:audio
            onStopped:{source=""}
        }

    }//Fin rectangle main

} // FIN iTEM
