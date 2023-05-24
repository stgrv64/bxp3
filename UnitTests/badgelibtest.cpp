#include <QString>
#include <QtTest>
#include <../BadgeLib/badgelib.h>

class BadgeLibTest : public QObject
{
    Q_OBJECT

private:
    BadgeLib badgelib;

public:
    BadgeLibTest();

private Q_SLOTS:
    void testTraitMotorolaHid();
    void testWiegChklrc4b();
    void testWiegChklrc4b_2();
    void testWiegChklrc4b_3();
    void testTraitWiegandStruct();
    void testTraitWiegandStd();
};

BadgeLibTest::BadgeLibTest()
{
}

QBitArray convertToBitArray(QString str)
{
    int len=str.length();
    QBitArray bits(len);

    for(int i=0; i < len; i++)
    {
        bits.setBit(i,(str.at(i)=='1'));
    }
    return bits;
}

void BadgeLibTest::testWiegChklrc4b()
{
    QBitArray bits = convertToBitArray("01000010000000000000000000010000001000000010011100000000010111101011");

    int ret = badgelib.wieg_chklrc4b(bits, 0);
    QVERIFY2(ret!=0, "Erreur de controle de checksum");
}

void BadgeLibTest::testWiegChklrc4b_2()
{
    QBitArray bits = convertToBitArray("0100001000000000000000010000001000000010011100000000010111101010");

    int ret = badgelib.wieg_chklrc4b(bits, 1);
    QVERIFY2(ret!=0, "Erreur de controle de checksum");
}

void BadgeLibTest::testWiegChklrc4b_3()
{
    QBitArray bits = convertToBitArray("0100001000010000001000000010011100000000010111100011");

    int ret = badgelib.wieg_chklrc4b(bits, 8);
    QVERIFY2(ret!=0, "Erreur de controle de checksum");
}

void BadgeLibTest::testTraitWiegandStruct()
{
    QBitArray bits = convertToBitArray("01000010000000000000000000010000001000000010011100000000010111101011");
    QString resultAttendu = "420000102027005E";

    QString badge = badgelib.trait_wiegand_struct(bits, (sWiegandFormat *)&Wieg68bitshexa);
    QVERIFY2(!badge.isNull(), "Failure");
    QCOMPARE(resultAttendu, badge);
}

void BadgeLibTest::testTraitWiegandStd()
{
    QBitArray bits = convertToBitArray("01000010000000000000000000010000001000000010011100000000010111101011");
    QString resultAttendu = "420000102027005E";

    QString badge = badgelib.trait_wiegand_std(bits, 0);
    QVERIFY2(!badge.isNull(), "Failure");
    QCOMPARE(resultAttendu, badge);
}

void BadgeLibTest::testTraitMotorolaHid()
{
    QBitArray bits = convertToBitArray("01000010000000000000000000010000001000000010011100000000010111101011");
    QString resultAttendu = "420000102027005E";

    QString badge = badgelib.trait_motorola_hid(bits, "0");
    QVERIFY2(!badge.isNull(), "Failure");
    QCOMPARE(resultAttendu, badge);
}

QTEST_APPLESS_MAIN(BadgeLibTest)

#include "tst_unitteststest.moc"
