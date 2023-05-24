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


#ifndef HEADER_CONVERSIONS
#define HEADER_CONVERSIONS

#include "cryptopp/base32.h"
#include "cryptopp/base64.h"
#include "cryptopp/hex.h"

#include <QtWidgets>

using namespace std;

class Conversion
{

public:
    string Base32EncodeFun(QString Text);
    string Base32DecodeFun(QString Text);
    string Base64EncodeFun(QString Text);
    string Base64DecodeFun(QString Text);
    string HexEncodeFun(QString Text);
    string HexDecodeFun(QString Text);
    string LeetSpeakEncodeFun(QString str);
    string LeetSpeakDecodeFun(QString str);
    string ReverseFun(QString str);
    string Rot13EncodeFun(QString src);
    int UrlDecode(const char *source, char *dest);
    int UrlEncode(const char *source, char *dest, unsigned max);
    string UrlDecodeString(QString encoded);
    string UrlEncodeString(QString decoded);
    string UUEncodeString(QString encoded);
    string UUDecodeString(QString decoded);

};

#endif
