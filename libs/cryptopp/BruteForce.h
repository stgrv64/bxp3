// This file is part of the Qrypto project
// Copyright (C) 2008-2010 Amine Roukh <amineroukh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; If not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.


#ifndef HEADER_BRUTEFORCE
#define HEADER_BRUTEFORCE

#include <QtWidgets>
#include "Hashes.h"

enum functions {Adler32=1, CRC32, MD2, MD4, MD5, RIPEMD128, RIPEMD160, RIPEMD320, SHA1, SHA224, SHA256, SHA384, SHA512,
                Tiger, Whirlpool
               };

class BruteForceThread : public QThread
{
    Q_OBJECT

public:
    void init(string alphabet, string targetHash, int MaxPasswordLength);
    void setHashFun(functions fun);

signals:
    void process(QString);
    void finished(QString, bool);
    
protected:
    void run();

private:
    string m_alphabet;
    string m_targetHash;
    unsigned int m_maxPasswordLength;
    functions m_fun;

    int get_index(string str, char ch);

    Hashes *hash;
};

class BruteForce : public QDialog
{
    Q_OBJECT

public :

    BruteForce(QWidget *parent=0);

    QCheckBox *upperCase;
    QCheckBox *lowerCase;
    QCheckBox *numbers;
    QCheckBox *speChar;
    QCheckBox *customBox;
    QSpinBox *maxKeySize;

public slots:
    void alphabetChanSlot();
    void startSlot();
    void stopSlot();
    void search(QString);
    void done(QString, bool);

private :
    void readSettings();
    void inputsEnable(bool enabled);
    bool isValidHash(QString input);

    QLabel *maxKeySizeLabel;
    QLabel *hashtoBFLabel;
    QLabel *statusLabel;
    QVBoxLayout *mainLayout;
    QGroupBox *hashTypeBox;
    QGroupBox *keyTypeBox;
    QGroupBox *setUpBox;
    QComboBox *hashSelect;
    QLineEdit *custom;
    QLineEdit *hashtoBF;
    QLineEdit *status;
    QPushButton *startBut;
    QPushButton *stopBut;
    QPushButton *closeBut;
	
    Hashes *hash;
    BruteForceThread bruteForceThread;
};

#endif
