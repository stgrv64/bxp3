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
import QtWebEngine 1.3
import "content"

Rectangle {
   id: webBrowser

    property string urlString :"";

    width:parent.width; height: parent.height
    color: "transparent"
    opacity:parent.opacity

    function run(url){webBrowser.urlString="";webBrowser.urlString=url;webBrowser.opacity=1;}

    function stop(){
        urlString =""
        webBrowser.opacity=0
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
        interactive:false

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
            //preferredWidth: flickable.width
            //preferredHeight: flickable.height
            //contentsScale: 1
            //settings.javaEnabled: false
            settings.javascriptEnabled: true
            settings.javascriptCanOpenWindows:false
            settings.javascriptCanAccessClipboard: false
            //settings.developerExtrasEnabled: false
            settings.pluginsEnabled:false
            //settings.offlineWebApplicationCacheEnabled: false
            settings.autoLoadImages: true
            activeFocusOnPress: false
            transformOrigin: Item.TopLeft
            url: webBrowser.urlString
            //pressGrabTime :0
        //    onLoadStart: screen_touch()

            onUrlChanged: {
                screen_touch();
                if (url !="")
                {
                flickable.contentX = 0
                flickable.contentY = 0
                }
            }
            onLoadingChanged: screen_touch()
            //onLoadFinished: screen_touch()

        /*                 evaluateJavaScript("document.getElementById('login').value = '" + webBrowser.user +"'")
                            evaluateJavaScript("document.getElementById('password').value = '" +  webBrowser.password +"'")
                            evaluateJavaScript("close()")
                            evaluateJavaScript("trylog()")
        */


    }
   }
}
