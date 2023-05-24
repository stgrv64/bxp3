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


#ifndef HEADER_HASHES
#define HEADER_HASHES

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/adler32.h>
#include <cryptopp/crc.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/md2.h>
#include <cryptopp/md4.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/sha.h>
#include <cryptopp/tiger.h>
#include <cryptopp/whrlpool.h>


#include <QtWidgets>

#include "Conversions.h"

using namespace std;

class Hashes
{

public:
    string Adler32Function(QString qstring);
    string Crc32Function(QString qstring);
    string Md2Function(QString qstring);
    string Md4Function(QString qstring);
    string Md5Function(QString qstring);
    string Ripemd128Function(QString qstring);
    string Ripemd160Function(QString qstring);
    string Ripemd320Function(QString qstring);
    string Sha1Function(QString qstring);
    string Sha224Function(QString qstring);
    string Sha256Function(QString qstring);
    string Sha384Function(QString qstring);
    string Sha512Function(QString qstring);
    string TigerFunction(QString qstring);
    string WhirlpoolFunction(QString qstring);
};

#endif
