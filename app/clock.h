/***************************************************************************
 *   Copyright (C) 2007 by michel haget                                    *
 *   mhaget@taillon                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef DEFINE_GLOBAL
#define GLOBAL
#define INIT(x)=x
#else
#define GLOBAL extern
#define INIT(x)
#endif

#ifndef Clock_H
#define Clock_H

#include <QMetaObject>
#include <QApplication>
#include <QVariant>
#include <QObject>
#include <QThread>
#include <QString>
#include <QEvent>
#include <QTimer>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QIODevice>



class Clock_qobject : public QObject {
	Q_OBJECT

signals:
    void signal_clock_badge(void);
    void signal_first_clock_badge(void);

public slots:
    void data_clock_attente_clock(void);

public:
    void init(void);
    // void release(void);

private:
    char clock_image;
    QFile *clockf;
    QTimer *timer_attente_clock;
    QByteArray clocki;
 //   QTextStream clockts;
};


class Clock_qthread : public QThread {
public: 
	
    void init(void);
    void run();
    Clock_qobject clock;


};


 
#endif // Clock_H

GLOBAL Clock_qthread clock_thread;
