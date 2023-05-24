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


#include "Hashes.h"

using namespace CryptoPP;
using namespace Weak;
using namespace std;

string Hashes::Adler32Function(QString qstring)
{
    Adler32 Adler32Hash;
    byte Adler32Digest[ Adler32::DIGESTSIZE ];
    string message = qstring.toStdString();

    Adler32Hash.CalculateDigest( Adler32Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Adler32Output;
    encoder.Attach( new StringSink( Adler32Output ) );
    encoder.Put( Adler32Digest, sizeof(Adler32Digest) );
    encoder.MessageEnd();

    return Adler32Output;
}

string Hashes::Crc32Function(QString qstring)
{
    CRC32 Crc32Hash;
    byte Crc32Digest[ CRC32::DIGESTSIZE ];
    string message = qstring.toStdString();

    Crc32Hash.CalculateDigest( Crc32Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Crc32Output;
    encoder.Attach( new StringSink( Crc32Output ) );
    encoder.Put( Crc32Digest, sizeof(Crc32Digest) );
    encoder.MessageEnd();

    return Crc32Output;
}

string Hashes::Md2Function(QString qstring)
{
    MD2 Md2Hash;
    byte Md2Digest[ MD2::DIGESTSIZE ];
    string message = qstring.toStdString();

    Md2Hash.CalculateDigest( Md2Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Md2Output;
    encoder.Attach( new StringSink( Md2Output ) );
    encoder.Put( Md2Digest, sizeof(Md2Digest) );
    encoder.MessageEnd();

    return Md2Output;
}

string Hashes::Md4Function(QString qstring)
{
    MD4 Md4Hash;
    byte Md4Digest[ MD4::DIGESTSIZE ];
    string message = qstring.toStdString();

    Md4Hash.CalculateDigest( Md4Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Md4Output;
    encoder.Attach( new StringSink( Md4Output ) );
    encoder.Put( Md4Digest, sizeof(Md4Digest) );
    encoder.MessageEnd();

    return Md4Output;
}

string Hashes::Md5Function(QString qstring)
{
    MD5 Md5Hash;
    byte Md5Digest[ MD5::DIGESTSIZE ];
    string message = qstring.toStdString();

    Md5Hash.CalculateDigest( Md5Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Md5Output;
    encoder.Attach( new StringSink( Md5Output ) );
    encoder.Put( Md5Digest, sizeof(Md5Digest) );
    encoder.MessageEnd();

    return Md5Output;
}


string Hashes::Ripemd128Function(QString qstring)
{
    RIPEMD128 Ripemd128Hash;
    byte Ripemd128Digest[ RIPEMD128::DIGESTSIZE ];
    string message = qstring.toStdString();

    Ripemd128Hash.CalculateDigest( Ripemd128Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Ripemd128Output;
    encoder.Attach( new StringSink( Ripemd128Output ) );
    encoder.Put( Ripemd128Digest, sizeof(Ripemd128Digest) );
    encoder.MessageEnd();

    return Ripemd128Output;
}


string Hashes::Ripemd160Function(QString qstring)
{
    RIPEMD160 Ripemd160Hash;
    byte Ripemd160Digest[ RIPEMD160::DIGESTSIZE ];
    string message = qstring.toStdString();

    Ripemd160Hash.CalculateDigest( Ripemd160Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Ripemd160Output;
    encoder.Attach( new StringSink( Ripemd160Output ) );
    encoder.Put( Ripemd160Digest, sizeof(Ripemd160Digest) );
    encoder.MessageEnd();

    return Ripemd160Output;
}


string Hashes::Ripemd320Function(QString qstring)
{
    RIPEMD320 Ripemd320Hash;
    byte Ripemd320Digest[ RIPEMD320::DIGESTSIZE ];
    string message = qstring.toStdString();

    Ripemd320Hash.CalculateDigest( Ripemd320Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Ripemd320Output;
    encoder.Attach( new StringSink( Ripemd320Output ) );
    encoder.Put( Ripemd320Digest, sizeof(Ripemd320Digest) );
    encoder.MessageEnd();

    return Ripemd320Output;
}


string Hashes::Sha1Function(QString qstring)
{
    SHA ShaHash;
    byte ShaDigest[ SHA::DIGESTSIZE ];
    string message = qstring.toStdString();

    ShaHash.CalculateDigest( ShaDigest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string ShaOutput;
    encoder.Attach( new StringSink( ShaOutput ) );
    encoder.Put( ShaDigest, sizeof(ShaDigest) );
    encoder.MessageEnd();

    return ShaOutput;
}

string Hashes::Sha224Function(QString qstring)
{
    SHA224 Sha224Hash;
    byte Sha224Digest[ SHA224::DIGESTSIZE ];
    string message = qstring.toStdString();

    Sha224Hash.CalculateDigest( Sha224Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Sha224Output;
    encoder.Attach( new StringSink( Sha224Output ) );
    encoder.Put( Sha224Digest, sizeof(Sha224Digest) );
    encoder.MessageEnd();

    return Sha224Output;
}

string Hashes::Sha256Function(QString qstring)
{
    SHA256 Sha256Hash;
    byte Sha256Digest[ SHA256::DIGESTSIZE ];
    string message = qstring.toStdString();

    Sha256Hash.CalculateDigest( Sha256Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Sha256Output;
    encoder.Attach( new StringSink( Sha256Output ) );
    encoder.Put( Sha256Digest, sizeof(Sha256Digest) );
    encoder.MessageEnd();

    return Sha256Output;
}

string Hashes::Sha384Function(QString qstring)
{
    SHA384 Sha384Hash;
    byte Sha384Digest[ SHA384::DIGESTSIZE ];
    string message = qstring.toStdString();

    Sha384Hash.CalculateDigest( Sha384Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Sha384Output;
    encoder.Attach( new StringSink( Sha384Output ) );
    encoder.Put( Sha384Digest, sizeof(Sha384Digest) );
    encoder.MessageEnd();

    return Sha384Output;
}

string Hashes::Sha512Function(QString qstring)
{
    SHA512 Sha512Hash;
    byte Sha512Digest[ SHA512::DIGESTSIZE ];
    string message = qstring.toStdString();

    Sha512Hash.CalculateDigest( Sha512Digest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string Sha512Output;
    encoder.Attach( new StringSink( Sha512Output ) );
    encoder.Put( Sha512Digest, sizeof(Sha512Digest) );
    encoder.MessageEnd();

    return Sha512Output;
}

string Hashes::TigerFunction(QString qstring)
{
    Tiger TigerHash;
    byte TigerDigest[ Tiger::DIGESTSIZE ];
    string message = qstring.toStdString();

    TigerHash.CalculateDigest( TigerDigest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string TigerOutput;
    encoder.Attach( new StringSink( TigerOutput ) );
    encoder.Put( TigerDigest, sizeof(TigerDigest) );
    encoder.MessageEnd();

    return TigerOutput;
}

string Hashes::WhirlpoolFunction(QString qstring)
{
    Whirlpool WhirlpoolHash;
    byte WhirlpoolDigest[ Whirlpool::DIGESTSIZE ];
    string message = qstring.toStdString();

    WhirlpoolHash.CalculateDigest( WhirlpoolDigest, (byte*) message.c_str(), message.length() );

    HexEncoder encoder;
    string WhirlpoolOutput;
    encoder.Attach( new StringSink( WhirlpoolOutput ) );
    encoder.Put( WhirlpoolDigest, sizeof(WhirlpoolDigest) );
    encoder.MessageEnd();

    return WhirlpoolOutput;
}

