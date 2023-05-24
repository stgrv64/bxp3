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

#include "BruteForce.h"

using namespace std;

log4cpp::Category *bruteforcelog;

BruteForce::BruteForce(QWidget *parent) : QDialog(parent)
{
    bruteforcelog = &log4cpp::Category::getInstance("bruteforce");
    bruteforcelog->debug("BruteForce::creator");

    QStringList hashList;
    hashList << "Hashing ------------------"
    << "Adler32 (Checksum)" << "CRC32 (Cyclic Redundancy Check)" << "MD2 (Message Digest 2)" << "MD4 (Message Digest 4)" << "MD5 (Message Digest 5)"
    << "RIPEMD-128 (RACE Integrity Primitives Evaluation Message Digest)"
    << "RIPEMD-160 (RACE Integrity Primitives Evaluation Message Digest)" << "RIPEMD-320 (RACE Integrity Primitives Evaluation Message Digest)"
    << "SHA-1 (Secure Hash Algorithm)" << "SHA-224 (Secure Hash Algorithm)" << "SHA-256 (Secure Hash Algorithm)" << "SHA-384 (Secure Hash Algorithm)"
    << "SHA-512 (Secure Hash Algorithm)" << "Tiger (192bit Cryptographic Hash Function)" << "Whirlpool (512bit Cryptographic Hash Function)";

    hashSelect = new QComboBox();
    hashSelect->addItems(hashList);

    QHBoxLayout *hashTypeLayout = new QHBoxLayout();
    hashTypeLayout->addWidget(hashSelect);
    hashTypeBox = new QGroupBox(tr("Hash type"), this);
    hashTypeBox->setLayout(hashTypeLayout);

    upperCase = new QCheckBox("Uppercase Alphabet");
    connect(upperCase, SIGNAL(stateChanged(int)), this, SLOT(alphabetChanSlot()));
    lowerCase = new QCheckBox("Lowercase Alphabet");
    connect(lowerCase, SIGNAL(stateChanged(int)), this, SLOT(alphabetChanSlot()));
    numbers = new QCheckBox("Numbers");
    connect(numbers, SIGNAL(stateChanged(int)), this, SLOT(alphabetChanSlot()));
    speChar = new QCheckBox("Special Characteres");
    connect(speChar, SIGNAL(stateChanged(int)), this, SLOT(alphabetChanSlot()));
    customBox = new QCheckBox(tr("Customized :"));
    connect(customBox, SIGNAL(stateChanged(int)), this, SLOT(alphabetChanSlot()));
    custom = new QLineEdit();

    maxKeySizeLabel = new QLabel(tr("Max lenght :"));
    maxKeySize = new QSpinBox;
    maxKeySize->setMinimum(1);

    QGridLayout *layoutOptions = new QGridLayout;
    layoutOptions->addWidget(upperCase, 0, 0);
    layoutOptions->addWidget(lowerCase, 1, 0);
    layoutOptions->addWidget(numbers, 0, 1);
    layoutOptions->addWidget(speChar, 1, 1);
    layoutOptions->addWidget(customBox, 2, 0);
    layoutOptions->addWidget(custom, 2, 1);
    layoutOptions->addWidget(maxKeySizeLabel, 3, 0);
    layoutOptions->addWidget(maxKeySize, 3, 1);

    keyTypeBox = new QGroupBox(tr("Alphabet to Use"), this);
    keyTypeBox->setLayout(layoutOptions);

    hashtoBFLabel = new QLabel(tr("Hash to Brute Force :"));
    hashtoBF = new QLineEdit();

    statusLabel = new QLabel(tr("Status :"));
    status = new QLineEdit();
    status->setReadOnly(true);

    QVBoxLayout *setUpLayout = new QVBoxLayout();
    setUpLayout->addWidget(hashtoBFLabel);
    setUpLayout->addWidget(hashtoBF);
    setUpLayout->addWidget(statusLabel);
    setUpLayout->addWidget(status);

    setUpBox = new QGroupBox(tr("Setup"), this);
    setUpBox->setLayout(setUpLayout);

    startBut = new QPushButton(QIcon(":/data/start.png"), tr("&Start"), this);
    connect(startBut, SIGNAL(clicked()), this, SLOT(startSlot()));
    stopBut = new QPushButton(QIcon(":/data/stop.png"), tr("Sto&p"), this);
    connect(stopBut, SIGNAL(clicked()), this, SLOT(stopSlot()));
    stopBut->setEnabled(false);
    closeBut = new QPushButton(QIcon(":/data/close.png"), tr("&Close"), this);
    connect(closeBut, SIGNAL(clicked()), this, SLOT(accept()));

    QHBoxLayout *butLayout = new QHBoxLayout();
    butLayout->addWidget(startBut);
    butLayout->addWidget(stopBut);
    butLayout->addWidget(closeBut);
    butLayout->setAlignment(Qt::AlignLeft);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(hashTypeBox);
    mainLayout->addWidget(keyTypeBox);
    mainLayout->addWidget(setUpBox);
    mainLayout->addLayout(butLayout);

    setLayout(mainLayout);
    readSettings();
}

void BruteForce::alphabetChanSlot()
{
    if (customBox->isChecked())
    {
        upperCase->setEnabled(false);
        lowerCase->setEnabled(false);
        numbers->setEnabled(false);
        speChar->setEnabled(false);
    }

    else
    {
        upperCase->setEnabled(true);
        lowerCase->setEnabled(true);
        numbers->setEnabled(true);
        speChar->setEnabled(true);
    }

    if (upperCase->isChecked() || lowerCase->isChecked() || numbers->isChecked() || speChar->isChecked())
    {
        customBox->setEnabled(false);
        custom->setEnabled(false);
    }
    else
    {
        customBox->setEnabled(true);
        custom->setEnabled(true);
    }
}

void BruteForce::startSlot()
{
    if (hashSelect->currentIndex() != 0)
    {
        if (upperCase->isChecked() || lowerCase->isChecked() || numbers->isChecked() || speChar->isChecked() || (customBox->isChecked() && !custom->text().isEmpty()))
        {
            if (isValidHash(hashtoBF->text()))
            {
                string keyType;

                if (upperCase->isChecked())
                    keyType += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

                if (lowerCase->isChecked())
                    keyType += "abcdefghijklmnopqrstuvwxyz";

                if (numbers->isChecked())
                    keyType += "0123456789";

                if (speChar->isChecked())
                    keyType += "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

                if (customBox->isChecked())
                    keyType = custom->text().toStdString();

                inputsEnable(false);
                
                status->clear();

                connect(&bruteForceThread, SIGNAL(process(QString)), this, SLOT(search(QString)));
                connect(&bruteForceThread, SIGNAL(finished(QString, bool)), this, SLOT(done(QString, bool)));
                
                bruteForceThread.init(keyType, hashtoBF->text().toStdString(), maxKeySize->value());
                bruteForceThread.setHashFun(functions(hashSelect->currentIndex()));
                bruteForceThread.start();

            }
            else
                status->setText("Please write a correct hash.");
        }
        else
            status->setText("Please check the alphabet to use.");
    }
    else
        status->setText("Please select a hash from the drop down box.");
}

// TODO: Remove White Space from input
bool BruteForce::isValidHash(QString input)
{
    if (!input.isEmpty())
    {
        int n = input.size();
        bruteforcelog->debug(input.simplified().toStdString());
        if (n == 8 || n == 32 || n == 40 || n == 48 || n == 80 || n == 56 || n == 64 || n == 96 || n == 128)
            return true;
        return false;
    }
    return false;
}

void BruteForce::stopSlot()
{
    bruteForceThread.terminate();
    inputsEnable(true);
}

void BruteForce::inputsEnable(bool enabled)
{
    hashTypeBox->setEnabled(enabled);
    keyTypeBox->setEnabled(enabled);
    hashtoBF->setEnabled(enabled);
    startBut->setEnabled(enabled);
    stopBut->setEnabled(!enabled);
}

void BruteForce::readSettings()
{
    QSettings settings(tr("Qrypto", "Qt apps"));

    upperCase->setChecked(settings.value("BruteForce/Uppercase", true).toBool());
    lowerCase->setChecked(settings.value("BruteForce/Lowercase").toBool());
    numbers->setChecked(settings.value("BruteForce/Numbers", true).toBool());
    speChar->setChecked(settings.value("BruteForce/SpecialChar").toBool());
    customBox->setChecked(settings.value("BruteForce/Customized").toBool());
    maxKeySize->setValue(settings.value("BruteForce/maxKeySize", 1).toInt());

    setWindowTitle(tr("Hash Brute Forcer"));
}

void BruteForce::done(QString str, bool found)
{
    if(!found)
        status->setText("No match found");
    else
        status->setText("Solution : " + str);

    inputsEnable(true);
}

void BruteForce::search(QString str)
{
    status->setText("Current : " + str);
    qApp->processEvents();
}

void BruteForceThread::run()
{
    string hashBuf;
    unsigned int a,b,c;
    char password[14];

    hash = new Hashes();

    for (a = 0; a < m_maxPasswordLength; a++)
    {
        for (b=0; b <= a; b++)
        {
            password[b] = m_alphabet[0];
        }

        password[b] = '\0';
        b = 0;	// Start at position zero

        while (b <= a)
        {
            if (!b)
            {
                // Generate the Hash
                switch (m_fun)
                {
                case Adler32:
                    hashBuf = hash->Adler32Function(password);
                    break;
                case CRC32:
                    hashBuf = hash->Crc32Function(password);
                    break;
                case MD2:
                    hashBuf = hash->Md2Function(password);
                    break;
                case MD4:
                    hashBuf = hash->Md4Function(password);
                    break;
                case MD5:
                    hashBuf = hash->Md5Function(password);
                    break;
                case RIPEMD128:
                    hashBuf = hash->Ripemd128Function(password);
                    break;
                case RIPEMD160:
                    hashBuf = hash->Ripemd160Function(password);
                    break;
                case RIPEMD320:
                    hashBuf = hash->Ripemd320Function(password);
                    break;
                case SHA1:
                    hashBuf = hash->Sha1Function(password);
                    break;
                case SHA224:
                    hashBuf = hash->Sha224Function(password);
                    break;
                case SHA256:
                    hashBuf = hash->Sha256Function(password);
                    break;
                case SHA384:
                    hashBuf = hash->Sha384Function(password);
                    break;
                case SHA512:
                    hashBuf = hash->Sha512Function(password);
                    break;
                case Tiger:
                    hashBuf = hash->TigerFunction(password);
                    break;
                case Whirlpool:
                    hashBuf = hash->WhirlpoolFunction(password);
                    break;
                }

                // Print out the password
                if (hashBuf ==  m_targetHash)
                {
                    emit finished(QString::fromStdString(password), true);
                    bruteforcelog->debug(password);
                    return;
                }
                //qApp->processEvents();
                emit process(QString::fromStdString(password));
                bruteforcelog->debug(password);
            }
            c = get_index(m_alphabet, password[b]);
            c++;

            if (c >= m_alphabet.size())
            {
                password[b] = m_alphabet[0];	// Reset this to first char
                b++;				// increment POSITION
                continue;			// and keep going with loop
            }
            password[b] = m_alphabet[c];		// Set new alpha
            b = 0;				// Back to zero position
        }
    }

    emit finished(QString::fromStdString(password), false);

}

int BruteForceThread::get_index(string str, char ch)
{
    int i = 0;
    while (str[i])
    {
        if (ch == str[i])
        {
            return(i);
        }
        i++;
    }
    return(i);
}

void BruteForceThread::init(string alphabet, string targetHash, int maxPasswordLength)
{
    m_alphabet = alphabet;
    m_targetHash = targetHash;
    m_maxPasswordLength = maxPasswordLength;
    bruteforcelog->debug("m_alphabet=%d, m_targetHash=%d, m_maxPasswordLength=%d", m_alphabet, m_targetHash, m_maxPasswordLength);
}

void BruteForceThread::setHashFun(functions fun)
{
    m_fun = fun;
}
