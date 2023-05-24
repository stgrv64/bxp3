#include "badgelib.h"
#include <QDebug>
#include <QFile>


BadgeLib::BadgeLib()
{
    QFile f("badge_debug");
    if (f.exists()) badge_debug=true; else badge_debug=false;
}

//#define OPT_EUROCLEAR      1    /* lecteur Stid Marin 125 kHz wiegand 44 bits */
//const sWiegandFormat WiegEuroclear    = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_MIFARE_SAFRAN  3    /* Badge Mifare groupe Safran, lecteur Deister wiegand 37 bits */
//const sWiegandFormat WiegSafran       = { 37,  1, 2,36,0,  37, 2,36,1,  2,36,0,11,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_HID37CODESITE  4    /* Badge HID 125khz wiegand 37 bits avec code site */
//const sWiegandFormat Wieg37bitssite   = { 37,  1, 2,19,0,  37,19,36,1,  2,17,0, 5,  18,36,0, 6,  0, 0,0, 0};
//#define OPT_HID37HISEC     5   /* Badge HID 125khz wiegand 37 bits HISEC */
//const sWiegandFormat Wieg37bitsHisec  = { 37,  0, 0, 0,0,   0, 0, 0,0,  8,14,0, 3,  15,21,0, 3, 22,37,0, 5};
//#define OPT_DEISTER48HISEC 6   /* Badge Deister 125khz wiegand 48 bits HISEC */
//const sWiegandFormat Wieg48bitsHisec  = { 48,  0, 0, 0,0,   0, 0, 0,0, 33,48,0, 5,   0, 0,0, 0,  0, 0,0, 0};
//const sWiegandFormat Wieg58bitsHisec  = { 58,  0, 0, 0,0,   0, 0, 0,0, 41,56,0, 5,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_44B_26BDEC     7    /* lecteur Stid Marin 125 kHz wiegand 44 bits, decode 24 bits LSB en decimal */
//const sWiegandFormat Wieg44b_24dec    = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,16,0, 5,  17,40,0, 8,  0, 0,0, 0};
//#define OPT_FINGERKEY44    8   /* Badge Deister 125khz wiegand 44 bits Traitement FingerKey */
//const sWiegandFormat Wieg44bitsFinger = { 44,  0, 0, 0,0,   0, 0, 0,0, 12,43,3,10,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_ANATOR36       9   /* Badge HID 125khz wiegand 36 bits Traitement Anator */
//const sWiegandFormat Wieg36bitsAnator = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,18,0, 6,  19,36,0, 6,  0, 0,0, 0};
//            timer_timeout_badge_morpho->start(200);
//#define OPT_CORNING32     10   /* Badge Indala 125khz wiegand 32 bits Format specif Corning */
//const sWiegandFormat Wieg32bitsCorning = { 32,  1, 2,13,0,  32,14,31,1,  2,13,0, 4,  14,31,0, 6,  0, 0,0, 0};
//#define OPT_TECHTALK8     12   /* Badge Indala/HID 125khz wiegand 36 bits Format specif TechTalk decode 8 chiffres */
//const sWiegandFormat Wieg36btechtalk8  = { 36, 13,19,36,1,   0, 0, 0,0, 14,18,0, 2, 19,20,0, 1,  21,36,0, 5};
//#define OPT_STID68DEC     13   /* Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor */
//const sWiegandFormat Wieg68bstiddec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1,  9,64,0,17,  0, 0,0, 0};
//#define OPT_STID44AIA     14   /* Lecteur STID Marin 125kHz en interface 44 bits, decodage bdge DGA AIA Clermont Ferrand */
//const sWiegandFormat Wieg44bstidaia   = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
//            timer_timeout_badge_morpho->start(200);
//#define OPT_COTAG34       15   /* Lecteur COTAG xxxkHz en interface 34 bits, decodage badge MERIAL */
//const sWiegandFormat Wieg34bcotag     = { 34,  0, 0, 0,0,   0, 0, 0,0, 21,33,1, 4,  18,20,1, 1,   2,17,1, 5};   /* Sans ctrl parites + 3 champs */
//#define OPT_TETRAPAK      16   /* Bagde Marin 125 kHz Lecteur Wiegand 44 bits = inversion binaire avant decodage */
//const sWiegandFormat Wieg44btetrapak  = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_HID37BCD      17   /* Bagde iClass 37 bits 8 quartets BCD => 8 decimal ascii */
//const sWiegandFormat Wieg37bitsbcd    = { 37,  1, 2,19,0,  37,19,36,1,  5,36,2, 8,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_44B_44BDEC    18   /* Decodage Wiegand 44 bits => 13 decimale + */
/* Decodage lecteur Deister Mullion interface Clock Data 44 bits => 13 decimale */
//const sWiegandFormat Wieg44bitsdec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,13,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_44B_MSPASS    19   /* Decodage Lecteur Stid "Ritzenthaler" Wiegand 44 bits => 13 decimal format MSPASS ISO2 */
//const sWiegandFormat Wieg44bitsmspass  = { 44,  0, 0, 0,0,   0, 0, 0,0,  9,20,0,4,   21,23,0, 3,  24,40,0,6};
//#define OPT_68B_ASCII     20   /* Decodage Lecteur Stid Wiegand 64 bits ascii + 4 lrc => 8 ascii */
//const sWiegandFormat Wieg68bitsascii   = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,5,8,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_34B_URMET_8H  22   /* Decodage Lecteur Urmet Wiegand 34 bits => 8 hexadecimal majuscule */
//const sWiegandFormat Wieg34bitsUrmet_8H  = { 34,  1, 2,17,0,  34,18,33,1,  2,33,3,8,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_34B_URMET_10D 23   /* Decodage Lecteur Urmet Wiegand 34 bits => 10 decimal */
//const sWiegandFormat Wieg34bitsUrmet_10D = { 34,  1, 2,17,0,  34,18,33,1,  2,33,0,10,   0, 0,0, 0,  0, 0,0, 0};
//#define OPT_92B_ASCII 24  /*  Decodage Lecteur Stid Wiegand 92 bits ascii => 11 ascii  */
//const sWiegandFormat Wieg92bitsAscii   = { 92,  0, 0, 0,0,   0, 0, 0,0,  1,88,5,11,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_CORNING32_35  25   /* Badge Indala et iClass sur lecteur MultiClass = wiegand 32 et 35 bits Format specif Corning */
//const sWiegandFormat Wieg32bitsiCorning = { 32,  1, 2,13,0,  32,14,31,1,  2,13,0, 4,  14,31,0, 7,  0, 0,0, 0};
//const sWiegandFormat Wieg35bitsiCorning = { 35,  1, 2,35,1,   0, 0, 0,0,  3,14,0, 4,  15,34,0, 7,  0, 0,0, 0};
//#define OPT_68B_CG95      26   /* Decodage Lecteur Stid Wiegand 68 bits, 6 ascii decimal => 8 ascii decimal */
//const sWiegandFormat Wieg68bitscg95     = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,48,5,8,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_36B_CALNICE   27   /* Centre Antoine Lacassagne, Decodage Lecteur Stid Wiegand 36 bits, 8 BCD + LRC => 8 ascii decimal */
//const sWiegandFormat Wieg36bitscalnice  = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,32,2,8,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_64B_20DEC     28   /* S3V, Wiegand Stid 3T 68 bits, numero de serie ISO15693 64 bits converti en decimal sur 20 ascii */
//const sWiegandFormat Wieg68bit20dec     = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,0,20,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_64B_LSB_18DEC 29   /* Biomerieux, Wiegand Stid 3T 68 bits, decimal sur 1+17, decodage UID LSB first */
//const sWiegandFormat Wieg68b_56lsbdec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1,  9,64,6,17,  0, 0,0, 0};
//const sWiegandFormat Wieg68b_32lsbdec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1, 33,64,6,17,  0, 0,0, 0};
//#define OPT_84B_ASCII     30   /*  Decodage Lecteur Stid Wiegand 84 bits ascii => 10 ascii (DCNS Cherbourg) */
//const sWiegandFormat Wieg84bitsAscii   = { 84,  0, 0, 0,0,   0, 0, 0,0,  1,80,5,10,    0, 0,0, 0,  0, 0,0, 0};
//#define OPT_44B_28BDEC    31   /* wiegand 44 bits, 1 champ 12 bits en dec sur 4 + 1 champ 28 bits en decimal sur 9 */
//const sWiegandFormat Wieg44b_28dec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,12,0, 4,  13,40,0, 9,  0, 0,0, 0};
//#define OPT_44B_32BDEC    32   /* wiegand 44 bits, 1 champ 8 bits en dec sur 3 + 1 champ 32 bits en decimal sur 10 */
//const sWiegandFormat Wieg44b_32dec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1, 8,0, 3,   9,40,0,10,  0, 0,0, 0};
//#define OPT_68_ACCOR_UID  47   /* lecteur Stid LXS-RX1 bi freq, Wiegand 68, badge Mifare UID et HID 125kHz Accor = Rectorat Poitiers */
//const sWiegandFormat Wieg68bitsAccor  = { 68,  1, 0, 0,4,   0, 0, 0,0,  37,44,0,11, 45,48,0, 2, 49,64,0, 5};

const int techtalk_indP1[22] = {2,3,5,6,8,9,14,15,17,18,20,21,23,24,26,27,29,30,32,33,35,36};
const int techtalk_indP2[22] = {1,3,4,6,7,9,10,14,16,17,19,20,22,23,25,26,28,29,31,32,34,35};

const unsigned long masque_2s[24] = {  // masques binaires pour creation b23 a b0
     0x400000, 0x100000, 0x800000, 0x200000, 0x020000, 0x080000, 0x010000, 0x040000,
     0x004000, 0x001000, 0x008000, 0x002000, 0x000020, 0x000008, 0x000001, 0x000040,
     0x000200, 0x000800, 0x000002, 0x000004, 0x000010, 0x000400, 0x000100, 0x000080};

/* Format HID Corporate 1000, Numero des bits (de 1 a n) utilises pour le calcul des 2 bits de parité b2 et b35 */
const char HIDcorp_nbitPb02[22] = {3,4,6,7,9,10,12,13,15,16,18,19,21,22,24,25,27,28,30,31,33,34};
const char HIDcorp_nbitPb35[22] = {2,3,5,6,8,9,11,12,14,15,17,18,20,21,23,24,26,27,29,30,32,33};


void BadgeLib::Debug(QString a)
{
    if (badge_debug) qDebug() << a;
}

/*****************************************************/
/*  WIEG_CHKLRC4B()                                  */
/*  Controle le LRC par quartet d'un format Wiegand  */
/*  STId = n quartets de donnees + 1 quartet de LRC  */
/*  avec LRC = xor des quartets de donnees           */
/*                                                   */
/*  Entrees : _bit = buffer bit badge                */
/*              valeur lrc à tester                  */
/*  Sorties : 1 si checksum OK                       */
/*            0 si erreur checksum ou nbbits n'est   */
/*              un modulo 4                          */
/*****************************************************/
int BadgeLib::wieg_chklrc4b(QBitArray bit, char lrc)
{
    int i,byteCount;
    int nbbits=bit.size();
    int nb4bits=nbbits/4;

    if (nbbits > 1 && (nbbits % 4) == 0)
    {
        // on reconstruit des bytes à partir de 4 bits
        char chksum = 0; // pour le calcul du checksum
        for (byteCount=0; byteCount < nb4bits; byteCount++)
        {
            char data = 0; // pour le calcul d'un byte
            for (i=0; i<4; i++) { // reconstruction du byte
                data <<= 1;
                if (bit.testBit(i+(byteCount*4)))
                    data |=  1;
            }
            chksum = chksum ^ data;
        }

        if (chksum == lrc)
            return (1) ;    /* checksum OK */
    }
    return(0);
}

/*****************************************************/
/*  TRAIT_WIEGAND_STRUCT()                           */
/*  Traite les donnees wiegand en fonction des       */
/*  parametre definis dans la structure de format    */
/*  wiegand                                          */
/*                                                   */
/*  Entrees : bit =  zone badge bits                 */
/*            wiegform = pteur sur struct format     */
/*  Sortie : QString badge                           */
/*****************************************************/
QString BadgeLib::trait_wiegand_struct(QBitArray bit, sWiegandFormat *wiegform)
{
    int i, j,k, nbbits, offset, data, idata;
    qulonglong  varl;
    sWiegandParity *wiegparity;
    sWiegandData *wiegdata;
    QString badge("Badge inconnu");
    QString QS;
    QBitArray imagebit(bit);
    QByteArray byte;
    QByteArray imageByte;

    Debug(QString("nbbits:%1 ").arg(wiegform->nbbits));
    //test longueur
    if (wiegform->nbbits != bit.size()) return(badge);

    /** controle des bits de parite **/
    for (i = 0; i < WIEG_MAX_NBPARITY; i++)
    {
        wiegparity = &(wiegform->parity[i]);
        if (wiegparity->pos) // sinon pas de parite
        {   /* bit de parite defini */
            data = wiegparity->odd;
            if (data == 4) // parite byte (4 bits)
            {
                if(!wieg_chklrc4b(bit,0))
                    return(badge);
            }else
            {
                //calcul de la parite
                for (j=wiegparity->deb - 1; j < wiegparity->fin; j++)
                {
                    if (bit.testBit(j))
                        data ^= 1;
                }
                // si la parite calculee est differente, alors erreur
                if ((data!=0) != bit.at(wiegparity->pos - 1))
                    return(badge);
            }
        }
    }

    badge.clear();

    Debug("VERSION 2");
    /** conversion des champs de donnees **/
    for (idata = 0; idata < WIEG_MAX_NBDATA; idata++)
    {
        wiegdata = &(wiegform->data[idata]);
        Debug(QString("Champs:%1 format:%2 longueur:%3 debut:%4 fin:%5 ").arg(idata).arg(wiegdata->format).arg(wiegdata->len_out).arg(wiegdata->deb).arg(wiegdata->fin));

        if (wiegdata->deb)
        {   /* champ de donnee defini */
            nbbits = wiegdata->fin - wiegdata->deb + 1;  /* nb de bit */
            switch (wiegdata->format)
            {
            case WIEG_BINLSB_DEC :  /* croise MSB/LSB sur ce champ uniquement */
            case WIEG_BYTELSB_DEC : /* croise MSB/LSB au niveau octet sur ce champ uniquement */
            case WIEG_BINMSB_DEC :  /* Binaire MSB vers decimal ascii */

                varl= 0;
                for ( j= wiegdata->deb - 1; j < wiegdata->fin; j++)
                {
                    /* Binaire MSB vers decimal ascii */
                    varl <<= 1;
                    if (imagebit.testBit(j))
                        varl |= 1;
                }

                Debug("bit avant : " + QString::number(varl));
                nbbits = wiegdata->fin - wiegdata->deb + 1;  /* nb de bit */

                if (wiegdata->format == WIEG_BINLSB_DEC)
                {
                    Debug("trait_wiegand_struct : WIEG_BINLSB_DEC");
                    /* croise MSB/LSB au niveau bit sur ce champ uniquement */
                    for (k=wiegdata->deb - 1, j=nbbits-1; j>k; k++, j--)
                    {
                        imagebit.setBit(k,bit.testBit(j));
                        imagebit.setBit(j,bit.testBit(k));
                    }
                }
                else if (wiegdata->format == WIEG_BYTELSB_DEC)
                {
                    /* croise MSB/LSB au niveau octet sur ce champ uniquement */
                    Debug("trait_wiegand_struct : !WIEG_BYTELSB_DEC");
                    if ((nbbits % 8) == 0)
                    {
                        Debug("1 len_out=" + QString::number(wiegdata->len_out) + " nbbits=" + QString::number(nbbits));
                        byte.resize(bit.count()/8);
                        byte.fill(0);

                        // Convert from QBitArray to QByteArray
                        for(int b=wiegdata->deb - 1 ; b < wiegdata->fin ; ++b)
                        {
                            byte[b/8] = (byte.at(b/8) | ((bit[b]?1:0)<<(7-(b%8))));
                        }

                        Debug("2: " + byte.toHex());

                        imageByte.clear();
                        for (k=0; k<byte.count(); k++)
                        {
                            imageByte.prepend(byte.at(k));
                        }

                        //
                        imageByte = imageByte.mid(0,nbbits/8);

                        Debug("3: imageByte=" + imageByte.toHex() + " size=" + QString::number(imageByte.size()));

                        // Convert from QByteArray to QBitArray
                        for(int i=0; i<imageByte.size(); ++i)
                        {
                            for(int b=0; b<8;b++)
                            {
                                imagebit.setBit(wiegdata->deb-1 + i*8+b, imageByte.at(i)&(1<<(7-b)) );
                            }

                        }
                    }
                }

                varl= 0;
                for ( j= wiegdata->deb - 1; j < wiegdata->fin; j++)
                {
                    /* Binaire MSB vers decimal ascii */
                    varl <<= 1;
                    if (imagebit.testBit(j))
                        varl |= 1;
                }
                QS.clear();
                QS+=QString::number(varl, 10).rightJustified(wiegdata->len_out, '0');
                Debug( QS + " deb:" + QString::number(wiegdata->deb) + " fin:"  + QString::number(wiegdata->fin));


                badge.append(QS);
                break;


            case WIEG_BCD_DEC :     /* BCD vers decimal ascii */
            case WIEG_HEXMSB_HEX :  /* Hex msb vers hexa code ascii majuscule */
                if (((nbbits + 3) / 4) > wiegdata->len_out) break; /* trop de bits pour le format de sortie */
                offset = nbbits % 4;  /* le 1er caractere BCD est-il incomplet < 4 bits ? */
                Debug("trait_wiegand_struct : WIEG_HEXMSB_HEX offset=" + QString::number(offset) + " ");

                byte = QByteArray((wiegdata->len_out)+5,'\0');
                if (offset)
                {
                    for (i=0,j=wiegdata->deb - 1; i < offset; i++,j++)  byte[0] =  byte.at(0) | (bit.testBit(j) << (offset-i));
                    byte[0]=(byte.at(0) | '0');

                    for (k=1;j < wiegdata->fin ;k++)
                    {
                        for (i=0; i < 4; i++,j++) byte[k] =  byte.at(k) | (bit.testBit(j) << (3-i));
                        if (byte.at(k) <=9) byte[k]=(byte.at(k) + '0');
                        else byte[k]=((byte.at(k) -10) + 'A');
                    }
                }
                else
                {
                    for (k=0,j=wiegdata->deb - 1;j < wiegdata->fin ;k++)
                    {
                        for (i=0; i < 4; i++,j++)
                        {
                            byte[k] =  byte.at(k) | (bit.testBit(j) << (3-i));
                        }

                        //Debug("avant " + QString::number( byte.at(k)));
                        if (byte.at(k) <=9)
                        {
                            byte[k]=(byte.at(k) + '0');
                        }
                        else
                        {
                            //Debug(QString::number(byte.at(k)));
                            byte[k]=((byte.at(k) -10) + 'A');
                        }
                        //Debug("apres " + QString(byte.at(k)));
                    }
                }

                badge.append(QString(byte));
                Debug( QString(byte) + " deb:" + QString::number(wiegdata->deb) + " fin:"  + QString::number(wiegdata->fin));
                break;

            case WIEG_ASCII :   /* ascii direct */
                Debug("trait_wiegand_struct : WIEG_ASCII ");
                if ((nbbits / 8) > wiegdata->len_out) break;
                if ((nbbits % 8) != 0) break;

                /* lecture donnees ascii */
                for(k=0,j= wiegdata->deb - 1; j < wiegdata->fin;k++ )
                {
                    for (i=0; i < 8; i++,j++) byte[k] =  byte.at(k) | (bit.testBit(j) << (7-i));
                }

                badge.append(QString(byte));
                Debug( QString(byte) + " deb:" + QString::number(wiegdata->deb) + " fin:"  + QString::number(wiegdata->fin));
                break;


            case WIEG_HEXLSB_DEC :
            default :     /* format inconnu */
                Debug("trait_wiegand_struct : format inconnu ");
                break;

            }
        }
    }
    return(badge);
}

QString BadgeLib::trait_hid_mifare_32bits(QBitArray bit, int option)
{
    /*****************************************************/
    /*  TRAIT_HID_MIFARE_32BITS()                        */
    /*  Traitement des donnees lecteur HID Mifare 6055   */
    /*  en interface Wiegand 32 bits :                   */
    /*   -b1  a b8  = octet 0 numero serie, MSB first    */
    /*   -b9  a b16 = octet 1 numero serie, MSB first    */
    /*   -b17 a b24 = octet 2 numero serie, MSB first    */
    /*   -b25 a b32 = octet 3 numero serie, MSB first    */
    /*  Code le numero de serie en decimal sur 10        */
    /*  caracteres ascii.                                */
    /*                                                   */
    /*                                                   */
    /*  Entrees :  bit =  zone badge bits                */
    /*  Sorties : string badge                           */
    /*****************************************************/
    QString badge;
    QByteArray byte(8,'\0');
    int i, j,k;
    ulong mat;
    for (j=0,k=0;j<4;j++,k++)
    {
        for (i=0;i<8;i++)  byte[k] =  byte.at(k) | (bit.testBit(i+(j*8)) << (7-i));
    }

    badge.clear();
    switch (option)
    {
    case 0:
        mat=byte.at(3)*(unsigned int)16777216 + byte.at(2)*65536 + byte.at(1)*256 + byte.at(0);
        badge=QString::number(mat,10).rightJustified(10,'0');
        break;
    case 100:
        mat=byte.at(0)*(unsigned int)16777216 + byte.at(1)*65536 + byte.at(2)*256 + byte.at(3);
        badge=QString::number(mat,10).rightJustified(10,'0');
        break;
    case 101:
        badge=QString("%1%2%3%4").arg(byte.at(3),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(0),2,16,QChar ('0'));
        break;
    case 102:
        badge=QString("%1%2%3%4").arg(byte.at(0),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(3),2,16,QChar ('0'));
        break;
    }
    return(badge);
}

QString BadgeLib::trait_tech_talk(QBitArray bit, int option)
{
    /*****************************************************/
    /*  TRAIT_TECH_TALK()                                */
    /*  Traitement des donnees Motorola Indala et HID    */
    /*  format Tech Talk 36 bits :                       */
    /*     -b1 a 9   = constante = 100011000             */
    /*     -b10      = parite 1 (cf doc)                 */
    /*     -b11      = parite 2 (cf doc)                 */
    /*     -b12      = parite 3 (cf doc)                 */
    /*     -b13      = parite 4 (cf doc)                 */
    /*     -b14 a 18 = champ A (0 a 31)                  */
    /*     -b19 a 20 = champ B (0 a 3)                   */
    /*     -b21 a 36 = champ X (0 a 65535)               */
    /*  Vérifie les parites et contantes, code les       */
    /*  donnees en ASCII :                               */
    /*     -1 caractere pour le champ B                  */
    /*     -5 caracteres pour le champ X                 */
    /*  Si erreur, numero d'erreur dans result[0] :      */
    /*    2 : erreur sur constante b1 a b9               */
    /*    3 : erreur sur bits de parite                  */
    /*                                                   */
    /*                                                   */
    /*  Entrees :  bit =  zone badge bits                */
    /*  Sorties : QString badge                          */
    /*****************************************************/
    QString badge("Badge inconnu");
    QByteArray byte(7,'\0');
    int i;
    bool ok;
    QBitArray p(2,'\0');

    /* controle code fabricant b1 -> b9 */
    byte[0] =  byte.at(0) | bit.testBit(0);
    for (i=0;i<8;i++)  byte[1] =  byte.at(1) | (bit.testBit(i+1) << (7-i));

    if (byte.toInt(&ok,16) != 0x118) return(badge);  /* code fabricant faux */
    /* controle parite P1 b10 / P2 b11 */
    p.setBit(0,0);       /* P1 paire */
    p.setBit(0,1);       /* P2 impaire */
    for(i=0; i<22; i++){
        p.setBit(0,(p.testBit(0)^ bit.testBit(techtalk_indP1[i]-1))); /* calcul P1 */
        p.setBit(0,(p.testBit(0)^ bit.testBit(techtalk_indP2[i]-1))); /* calcul P2 */
    }
    p.setBit(0,p.testBit(0)^ bit.testBit(9)); /* calcul P1 */
    p.setBit(1,p.testBit(1)^ bit.testBit(10)); /* calcul P2 */
    if (p.testBit(0) || p.testBit(1)) return(badge); // erreur parite P1 ou p2
    /* controle parite P3 b12 */
    p.setBit(0,0);       /* P3 paire */
    for(i=0; i<18; i++){
        p.setBit(0,p.testBit(0)^ bit.testBit(i)); /* calcul P1 */
    }
    p.setBit(0,p.testBit(0)^ bit.testBit(11));
    p.setBit(0,p.testBit(0)^ bit.testBit(12));
    p.setBit(0,p.testBit(0)^ bit.testBit(11));

    if (p.testBit(0)) return(badge); // erreur parite P3
    if (option == OPT_TECHTALK8) {    /* decodage techtalk 8 chiffres */
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg36btechtalk8);
    }else {                                 /* decodage techtalk 6 chiffres */
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg36btechtalk6);
    }
    return(badge);
}

QString BadgeLib::trait_hid_56bits(QBitArray bit, int option)
{
    /*****************************************************/
    /*  TRAIT_HID_56BITS()                               */
    /*  Traitement des données HID au format Wiegand     */
    /*  56 bits. 2 possibilites                          */
    /*  - Lecture d'un badge Mifare :                    */
    /*    - 32 bits numero Mifare                        */
    /*    - 8 bits de checksum Mifare (lu dans la puce)  */
    /*    - 8 bits a zero, pas tjs (bug lecteur iclass)  */
    /*    - 8 bits a zero                                */
    /*  - Lecture d'un badge Mifare "Desfire" :          */
    /*    - 56 bits numero Desfire                       */
    /*                                                   */
    /*  Entrees :  bit =  zone badge bits                */
    /*  Sorties : string badge                           */
    /*****************************************************/
    QString badge("Badge inconnu");
    QByteArray byte(8,'\0');
    int i, j;
    ulong mat;

    for (j=0;j<7;j++)
    {
        for (i=0;i<8;i++)  byte[j] =  byte.at(j) | (bit.testBit(i+(j*8)) << (7-i));
    }

    if (!byte.at(6))   /* dernier octet a zero -> Mifare 32 bits + checksum */
    {
        for (i = 0; i < 4; i++)   /* calcul checksum Mifare = Xor sur SN0 a SN3 */
            byte[4] = byte.at(4) ^ byte.at(i);

        if (!byte.at(4))       /* checksum OK */
        {
            badge.clear();
            switch (option)
            {
            case 0:
                mat=byte.at(3)*(unsigned int)16777216 + byte.at(2)*65536 + byte.at(1)*256 + byte.at(0);
                badge=QString::number(mat,10).rightJustified(10,'0');
                break;
            case 100:
                mat=byte.at(0)*(unsigned int)16777216 + byte.at(1)*65536 + byte.at(2)*256 + byte.at(3);
                badge=QString::number(mat,10).rightJustified(10,'0');
                break;
            case 101:
                badge=QString("%1%2%3%4").arg(byte.at(3),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(0),2,16,QChar ('0'));
                break;
            case 102:
                badge=QString("%1%2%3%4").arg(byte.at(0),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(3),2,16,QChar ('0'));
                break;
            }
        }
    }
    else          /* Pas Mifare 32 bits donc Mifare Desfire 56 bits, remet ordre SN6 -> SN0, en 14 hexadecimal */
    {
        badge.clear();
        switch (option)
        {
        case 0:
            badge=QString("%1%2%3%4%5%6%7").arg(byte.at(6),2,16,QChar ('0')).arg(byte.at(5),2,16,QChar ('0')).arg(byte.at(4),2,16,QChar ('0')).arg(byte.at(3),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(0),2,16,QChar ('0'));
            break;
        case 100:
            badge=QString("%1%2%3%4%5%6%7").arg(byte.at(0),2,16,QChar ('0')).arg(byte.at(1),2,16,QChar ('0')).arg(byte.at(2),2,16,QChar ('0')).arg(byte.at(3),2,16,QChar ('0')).arg(byte.at(4),2,16,QChar ('0')).arg(byte.at(5),2,16,QChar ('0')).arg(byte.at(6),2,16,QChar ('0'));
            break;
        }
    }

    //      qDebug() << badge;
    return(badge);
}

QString BadgeLib::trait_wiegand44b_2s(QBitArray bit)
{
    /*****************************************************/
    /*  TRAIT_WIEGAND44B_2S                              */
    /*  Decodage des signaux Wiegand 44 bits en valeur   */
    /*  decimal 9 chiffres apres brassage binaire        */
    /*  format 2S (certains badges Marin 125kHz)         */
    /*  On utilise que les 3 octets LSB sur les 40 bits  */
    /*  de donnees Wiegand, soit b23 a b0 ou du 17eme au */
    /*  40eme bit dans ordre Rx                          */
    /*                                                   */
    /*  Entrees :  bit =  zone badge bits                */
    /*  Sorties : QString badge                          */
    /*****************************************************/
    QString badge("Badge inconnu");
    QByteArray byte(7,'\0');
    int i, j;

    if (bit.size() != 44) return(badge);

    badge.clear();
    for (i = 0; i < 3; i++)
    {  /* 24 bits de poids faible Rx => 3 octets LSB d'un mot de 32 bits */
        for (j=0;j<8;j++) { byte[i] =  byte.at(i) | (bit.testBit(16+(i*8)+j) << (7-j));}
    }

    unsigned long src= byte.at(0)*65536 + byte.at(1)*256 + byte.at(2);
    unsigned long dest= 0;

    for (i = 0; i < 24; i++)
    {
        dest <<= 1;
        if (src & masque_2s[i])dest |= 1;
    }
    badge=QString("%1").arg(dest,9,10,QChar ('0'));
    return(badge);
}

QString BadgeLib::trait_wiegand_std(QBitArray bit, int option)
{
    /*****************************************************/
    /*  TRAIT_WIEGAND_STD()                              */
    /*  Aiguille vers traitements standard               */
    /*  pour badge en interface Wiegand                  */
    /*                                                   */
    /*  Entrees : bit =  zone badge bits                 */
    /*  Sorties : QString badge                          */
    /*****************************************************/
    QString badge="Badge inconnu";
    QBitArray p(2);

    switch (bit.size()) {   // decode suivant longueur donnees

    case 25 :
        if (!bit.testBit(0)) badge=QString("Badge inconnu");  // erreur bit de start doit etre =1
        else badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg25bitsNCS);
        break;

    case 26 :
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg26bits);
        break;

    case 30:
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg30bitsAccor);
        break;

    case 32 :
        badge=trait_hid_mifare_32bits(bit, option);
        break;

    case 34 :
        if (bit.testBit(0)) badge=QString("Badge inconnu");  // erreur bit de start doit etre =0
        else badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg34bits);
        break;

    case 35 :  /* Evol V3.02t : ajout 35 bits HID Corporate 1000 */
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg35bHIDCorp);
        if (!badge.compare("Badge inconnu"))
        {  /* pas d'Erreur : contrôle les 2 bits de parite specifiques */
            p.setBit(1,true);
            for(int i = 0; i < 22; i++)
            {
                p[0]= p.at(0) ^ bit.at(HIDcorp_nbitPb02[i]-1);  // calcul Parite bit b2
                p[1]= p.at(1) ^ bit.at(HIDcorp_nbitPb35[i]-1);  // calcul Parite bit b35
            }
            if (p.testBit(0) != bit.testBit(1)  || p.testBit(1) != bit.testBit(34)) badge=("Badge inconnu");
        }
        break;

    case 36 :
        badge=trait_tech_talk(bit, option);
        if (!badge.compare("Badge inconnu"))
        { // Erreur : c'est peut etre pas du Tech Talk
            if (wieg_chklrc4b(bit,4))  // lrc wiegand  OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg36bitsclock);
        }
        break;

    case 37 :
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg37bitsnosite);
        break;


    case 44:
        if (wieg_chklrc4b(bit,0))   // lrc wiegand 44 bit OK */
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44bitshexa);

        break;

    case 56 :  /* format 56 bit lecteur HID iClass */
        badge=trait_hid_56bits(bit, option);
        break;

    case 68 :  /* format 68 bit lecteur STID Mifare format GFI */
        if (wieg_chklrc4b(bit,0))   /* lrc wiegand 68 bit OK */
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitshexa);

        break;

    case 112 :
        badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg112bitshexa);
        break;

    default :
        //    trace_write("trait_wiegand_std() format inconnu, %d bits", lect->nb_echant);
        break;
    }
    return(badge);
}

QString BadgeLib::trait_motorola_hid(QBitArray bit, QString strOption)
{
    /*****************************************************/
    /*  TRAIT_MOTOROLA_HID()                             */
    /*  traite les donnees lecteurs interface Wiegand    */
    /*                                                   */
    /*  Entrees : bit =  zone badge bits                 */
    /*            option = option associee au lecteur    */
    /*  Sorties : QString badge                          */
    /*****************************************************/
    QString badge="Badge inconnu";

    if (bit.size() > 10 && !strOption.isEmpty())     // nbr bit suffisants et option saisie
    {
        int option = strOption.toInt();
        switch (option) {    // traitement standard ou specif  ?

        case OPT_EUROCLEAR :
            if ( (wieg_chklrc4b(bit,0)))  // lrc wiegand 44 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&WiegEuroclear);
            break;

        case OPT_44B_26BDEC :
            if ( wieg_chklrc4b(bit,0) ) {  // lrc wiegand 44 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44b_24dec);
            }
            break;

        case OPT_44B_28BDEC :
            if ( wieg_chklrc4b(bit, 0) ) {  /* lrc wiegand 44 bit OK */
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44b_28dec);
            }
            break;

        case OPT_44B_32BDEC :
            if ( wieg_chklrc4b(bit, 0) ) {  /* lrc wiegand 44 bit OK */
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44b_32dec);
            }
            break;

        case OPT_FINGERKEY44 :
            if (wieg_chklrc4b(bit,0)) {  //lrc wiegand 44 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44bitsFinger);
            }
            break;

        case OPT_MIFARE_SAFRAN :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&WiegSafran);
            break;

        case OPT_ANATOR36 :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg36bitsAnator);
            break;

        case OPT_HID37CODESITE :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg37bitssite);
            break;

        case OPT_HID37HISEC :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg37bitsHisec);
            break;

        case OPT_HID37BCD :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg37bitsbcd);
            break;

        case OPT_DEISTER48HISEC :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg48bitsHisec);
            if (badge.compare("Badge inconnu")!=0)badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg58bitsHisec); // sur erreur lecture 48 bits => lecture 58 bits
            break;

        case OPT_CORNING32 : // Badge Indala 125khz wiegand 32 bits Format specif Corning
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg32bitsCorning);
            break;

        case OPT_TECHTALK8 : // Badge Indala/HID 125khz wiegand 36 bits Format specif TechTalk decode 8 chiffres
            badge=trait_tech_talk(bit, option);
            break;

        case OPT_STID68DEC : // Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 68 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bstiddec);
            }
            break;

        case OPT_STID44AIA :
            if (wieg_chklrc4b(bit,0))      // lrc wiegand 44 bit OK
            {
                QBitArray bitinv(4,'\0');

                for (int i= 0; i < 40; i += 4)
                {  // inversion des bits dans chaque quartet
                    bitinv.setBit(i,bit.testBit(i+3));
                    bitinv.setBit(i+1,bit.testBit(i+2));
                    bitinv.setBit(i+2,bit.testBit(i+1));
                    bitinv.setBit(i+3,bit.testBit(i));
                    bit.setBit(i,bitinv.testBit(i));
                    bit.setBit(i+1,bitinv.testBit(i+1));
                    bit.setBit(i+2,bitinv.testBit(i+2));
                    bit.setBit(i+3,bitinv.testBit(i+3));
                }
                badge=trait_wiegand_struct(bitinv, (sWiegandFormat *)&Wieg44bstidaia);  // decodage OK
            }
            break;

        case OPT_COTAG34 :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg34bcotag);
            break;

        case OPT_34B_URMET_8H :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg34bitsUrmet_8H);
            break;

        case OPT_34B_URMET_10D :
            badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg34bitsUrmet_10D);
            break;

        case OPT_TETRAPAK :
            if ( wieg_chklrc4b(bit,0))    // lrc wiegand 44 bit OK
            {
                for (int i=0;i<40;i++)   bit.setBit(i,(bit.testBit(i) ^ 1));   // inversion binaire des 40 bits
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44btetrapak);  // decodage OK
            }
            break;

        case OPT_44B_44BDEC :  // 44 bits Wiegand => 13 decimal
            if (wieg_chklrc4b(bit,0) ) {  // lrc wiegand 44 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44bitsdec);
            }
            break;

        case OPT_44B_2S_9D :  // 44 bits Wiegand => 9 decimal format 2S (brassage binaire)
            if ( wieg_chklrc4b(bit,0) ) {  // lrc wiegand 44 bit OK
                badge=trait_wiegand44b_2s(bit);
            }
            break;

        case OPT_44B_MSPASS :  // Lecteur Stid "Ritzenthaler" Wiegand 44 bits => 13 decimal format MSPASS ISO2
            if ( wieg_chklrc4b(bit,0) ) {  // lrc wiegand 44 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg44bitsmspass);
                if (badge.compare("Badge inconnu")!=0)
                {   // pas d'erreur de conversion
                    QString code_site=badge.mid(4,3); // extrait code site
                    int a=code_site.toInt();
                    a++;
                    QString s = QString::number(a, 10).rightJustified(3, '0');;
                    badge.replace(4,3,s);
                }
            }
            break;

        case OPT_68B_ASCII : // Lecteur STID programmable en interface 68 bits ascii
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 68 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitsascii);
            }
            break;

        case OPT_68B_CG95 :
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 68 bit dont 6 ASCIIOK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitscg95);
            }
            break;

        case OPT_64B_20DEC :
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 68 bit dont 6 ASCIIOK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bit20dec);
            }
            break;

        case OPT_64B_LSB_18DEC :
            Debug("OPT_64B_LSB_18DEC");
            if (wieg_chklrc4b(bit,0))
            {
                // Biomerieux, Wiegand Stid 3T 68 bits, decimal sur 1+17, decodage UID LSB first
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitshexa);
                QStringRef test(&badge,0,6);
                badge.clear();
                if (test.compare(QString("000000"))==0)
                {  /* 3 octets MSB de l'UID nuls = UID sur 4 octets */
                    badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68b_32lsbdec);  /* 32 bits => decimal */
                    Debug("badge32: " + badge);
                }else
                {  /* UID sur 7 octets */
                    badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68b_56lsbdec);  /* 56 bits => decimal */
                    Debug("badge64: " + badge);
                }
            }
            break;

        case OPT_68B_BBP_10DEC :  /* Lecteur Stid PHP 68 bits,  Detecte carte ISO14443B et B' et decode en consequence */
          badge = trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bits_bbp);
          Debug("OPT_68B_BBP_10DEC badge hex: " + badge);
          if (badge.compare("Badge inconnu")!=0)
          {  /* decodage OK => 16 hexa + 10 dec Pupi B' + 10 dec Pupi B */
            QStringRef test(&badge,0,8);
            if(test.compare(QString("45000000"))==0)
            {    /* prefixe carte ISO 14443 B */
              badge = badge.mid(26,10);
              Debug("ISO 14443 B: " + badge);
            }else
            {
                QStringRef test(&badge,0,6);
                QStringRef test2(&badge,14,2);
                if ((test.compare(QString("450007"))==0) && (test2.compare(QString("7F")) == 0))
              {  /* prefixe et suffixe ISO 14443 B Prim */
                badge = badge.mid(16,10);
                Debug("ISO 14443 B Prim: " + badge);
              }
            }
          }
          break;


        case OPT_68_ACCOR_UID : /* lecteur Stid LXS-RX1 bi freq, Wiegand 68, badge Mifare UID et HID 125kHz Accor = Rectorat Poitiers */
            badge = trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitshexa);
            Debug("OPT_68_ACCOR_UID badge hex: " + badge);
            if (badge.compare("Badge inconnu")!=0)
            {
                QStringRef test(&badge,0,2);
                if (test.compare(QString("42"))==0)
                {     /* puce 125 Khz */
                    Debug("puce 125 Khz");
                    test = QStringRef(&badge,2,6);
                    if ( (test.compare(QString("000010")) == 0) && (badge.at(8) != '0') )
                    {   /* tres probablement HID 125kHz format Accor */
                        badge = trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bitsAccor);
                        if (badge.compare("Badge inconnu")!=0)
                        {
                            badge.replace(0,1,"2"); /* force type puce pas decode par Wieg68bitsAccor (deja 3 champs de donnees) */
                            Debug("badge Accor: " + badge);
                        }
                    }else
                    {
                        Debug("badge Accor incorrect: " + badge);
                        badge = QString("Badge inconnu");   /* type de puce 125kHz incorrect */
                    }
                }else
                {   /* pas puce 125 kHz, on suppose Mifare Classic, on filtre pas, si évolution future */
                    badge = trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg68bstiddec);   /* Decodage 18 chiffre, UID decimal */
                    Debug("badge Mifare: " + badge);
                }
            }
            break;


        case OPT_84B_ASCII : /*  Decodage Lecteur Stid Wiegand 84 bits ascii => 10 ascii (DCNS Cherbourg) */
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 84 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg84bitsAscii);
            }
            break;

        case OPT_92B_ASCII : // Lecteur STID programmable en interface 92 bits ascii
            if (wieg_chklrc4b(bit,0)) {  // lrc wiegand 92 bit OK
                badge=trait_wiegand_struct(bit, (sWiegandFormat *)&Wieg92bitsAscii);
            }
            break;

        default :     //traitement standard ou option inconnue
            qDebug() << "traitement standard ou option inconnue" << bit;
            badge=trait_wiegand_std(bit, option);
            break;
        }
    }

    return(badge);

}
