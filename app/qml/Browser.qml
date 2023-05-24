/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import QtWebEngine 1.10
//import QtWebView 1.1
import "content"

Rectangle {
   id: browser
    width: 100; height: 100
    property string urlString :browser_url;
    color: "transparent"
    opacity:1
   /* WebEngineView{
         anchors.fill: parent
         onCertificateError: function(error){
             console.log("ignore certificateError : ");
             error.ignoreCertificateError();
         }
         onLoadingChanged: {
             console.log("loadRequest.errorCode : " + loadRequest.errorCode);
             console.log("loadRequest.errorDomain : " + loadRequest.errorDomain);
             console.log("loadRequest.status : " + loadRequest.status);
         }
         onJavaScriptDialogRequested: {
             request.accepted = true;
             console.log("JavaScriptDialogRequest accepted");
         }
         onJavaScriptConsoleMessage:console.log("JavaScriptConsole message");

        // url: "https://212.129.14.241/time2bii/accueil.html"
          url: "https://mobdev.chronotime-inetum.services/time2bii/accueil.html"
     }
*/


 Image {
      id:background

      anchors.fill:parent
      opacity: parent.opacity
      source: background_file
    }


  Flickable {
        id: flickable
        opacity:parent.opacity
        Behavior on opacity { SmoothedAnimation { velocity: 0.5 } }
        clip:true
        width:parent.width; height: parent.height
        contentHeight: Math.max(parent.height,webView.height)
        contentWidth: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        pressDelay:0
        interactive:urlString==="http://127.0.0.1/setup" ? true : false

        onFlickEnded:screen_touch() ;
        onFlickStarted:screen_touch() ;

        onWidthChanged :  {
           screen_touch()
            // Expand (but not above 1:1) if otherwise would be smaller that available width.
            if (width > webView.width*webView.contentsScale && webView.contentsScale < 1.0)
                  webView.contentsScale = width / webView.width * webView.contentsScale;

          }

        WebEngineView {
            id: webView
            opacity:parent.opacity
            width: flickable.width
            height: flickable.height
            scale : 1
  /*          preferredWidth: flickable.width
            preferredHeight: flickable.height
            contentsScale: 1
            settings.javaEnabled: false
  */
            settings.javascriptEnabled: true
            settings.javascriptCanOpenWindows:false
            settings.javascriptCanAccessClipboard: false
            //settings.developerExtrasEnabled: false
            settings.pluginsEnabled:false
            //settings.offlineWebApplicationCacheEnabled: false
            settings.autoLoadImages: true
           transformOrigin: Item.TopLeft
           url: browser.urlString
           profile.httpCacheType: WebEngineProfile.NoCache
           //pressGrabTime :0
           //onLoadStarted:screen_touch()
           //onAlert: {screen_touch();if (message.length) popup.show(message)}
           //onLoadFailed: {screen_touch();popup.show("Pas d'accès serveur") }
           onUrlChanged: {
               //console.debug("URL CHANGED: "+url)
                profile.clearHttpCache();
                screen_touch();
                if (url !="")
                {
                // got to topleft
                flickable.contentX = 0
                flickable.contentY = 0
                }
            }
           onContextMenuRequested: request.accepted = true ;
           onCertificateError: function(error){
               console.log("ignore certificateError : ");
               error.ignoreCertificateError();
           }
           onJavaScriptDialogRequested: {
               request.accepted = true;
               popup.show("Erreur accès serveur :" + request.message);
               console.log("JavaScriptDialogRequest accepted");               
           }
           onLoadingChanged: {
                              console.log("loadRequest.errorCode : " + loadRequest.errorCode);
                              console.log("loadRequest.errorDomain : " + loadRequest.errorDomain);
                              console.log("loadRequest.status : " + loadRequest.status);
                              if(loadRequest.status === WebEngineLoadRequest.LoadFailedStatus){
                                  webView.opacity = 0;
                                  popup.show("Pas d'accès serveur");
                                  console.log("Pas d'accès serveur");
                              }
                              screen_touch();
           }
           //onLoadFinished: screen_touch()

        /*                            evaluateJavaScript("document.getElementById('login').value = '" + browser.user +"'")
                            evaluateJavaScript("document.getElementById('password').value = '" +  browser.password +"'")
                            evaluateJavaScript("close()")
                            evaluateJavaScript("trylog()")
        */
        }
      }

            Rectangle {
                id: popup

                color: "red"
                border.color: "black"; border.width: 2
                radius: 4

                y: browser.height // off "screen"
                width: label.width + 5
                height: label.height + 5
                opacity: 0

                MouseArea {
                  anchors.fill: parent
                  onClicked: { popup.state = "";timer.stop();screen_touch()}
                  onPressed: { popup.state = "";timer.stop();screen_touch()}
                  }

                function show(text) {
                    if (text==="OK") popup.color="green"
                    else  popup.color="red"
                    label.text = text
                    popup.state = "visible"
                    timer.start()
                }
                states: State {
                    name: "visible"
                    PropertyChanges { target: popup; opacity: 1 }
                    PropertyChanges { target: popup; y: (browser.height-popup.height)/2 }
                }

                transitions: [
                    Transition { from: ""; PropertyAnimation { properties: "opacity,y"; duration: 500 } },
                    Transition { from: "visible"; PropertyAnimation { properties: "opacity,y"; duration: 500 }}
                ]

                Timer {
                    id: timer
                    interval: 5000
                    onTriggered: {popup.state = ""; viewid2_timer_init()}
 //                   onTriggered: {popup.state = ""}
                }

                Text {
                    id: label
                    anchors.centerIn: parent
                    width: browser.width  *0.75

                    color: "white"
                    font.pixelSize: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    smooth: true
                }
            }


}
