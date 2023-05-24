#ifndef BADGELIB_H
#define BADGELIB_H

#include "badgelib_global.h"

#include <QString>
#include <QBitArray>


#define WIEG_MAX_NBPARITY 2   /* max 2 bits de parite definis par format */
#define WIEG_MAX_NBDATA   3   /* max 3 champs de donnees definis par format */

typedef struct { /* structure bit parite */
    int pos;  /* position du bit de parite, 1er bit = 1, si 0 pas de bit */
    int deb;  /* position 1er bit pour calcul */
    int fin;  /* position dernier bit pour calcul */
    int odd;  /* si = 4 CRC sur 4 bits, si = 1 parite impaire, sinon paire */
}sWiegandParity ;

typedef struct { /* structure champ de donnee a convertir */
    int deb;  /* position 1er bit du champ, si 0 pas de champ */
    int fin;  /* position dernier bit du champ */
    int format;     /* format de conversion, cf cst ci-dessous */
    int len_out;    /* longueur sortie */
}sWiegandData ;


typedef struct { /* definition d'un format wiegand */
    int nbbits;                      /* nb de bits */
    sWiegandParity parity[WIEG_MAX_NBPARITY];  /* pour définition bits de parite */
    sWiegandData   data[WIEG_MAX_NBDATA];      /* pour definition champ de donnees a convertir */
}sWiegandFormat ;


/* Valeurs de option sur lecteur Wiegand  */
#define OPT_EUROCLEAR      1    /* lecteur Stid Marin 125 kHz wiegand 44 bits */
static const sWiegandFormat WiegEuroclear    = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_MIFARE_SAFRAN  3    /* Badge Mifare groupe Safran, lecteur Deister wiegand 37 bits */
static const sWiegandFormat WiegSafran       = { 37,  1, 2,36,0,  37, 2,36,1,  2,36,0,11,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_HID37CODESITE  4    /* Badge HID 125khz wiegand 37 bits avec code site */
static const sWiegandFormat Wieg37bitssite   = { 37,  1, 2,19,0,  37,19,36,1,  2,17,0, 5,  18,36,0, 6,  0, 0,0, 0};
#define OPT_HID37HISEC     5   /* Badge HID 125khz wiegand 37 bits HISEC */
static const sWiegandFormat Wieg37bitsHisec  = { 37,  0, 0, 0,0,   0, 0, 0,0,  8,14,0, 3,  15,21,0, 3, 22,37,0, 5};
#define OPT_DEISTER48HISEC 6   /* Badge Deister 125khz wiegand 48 bits HISEC */
static const sWiegandFormat Wieg48bitsHisec  = { 48,  0, 0, 0,0,   0, 0, 0,0, 33,48,0, 5,   0, 0,0, 0,  0, 0,0, 0};
static const sWiegandFormat Wieg58bitsHisec  = { 58,  0, 0, 0,0,   0, 0, 0,0, 41,56,0, 5,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_44B_26BDEC     7    /* lecteur Stid Marin 125 kHz wiegand 44 bits, decode 24 bits LSB en decimal */
static const sWiegandFormat Wieg44b_24dec    = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,16,0, 5,  17,40,0, 8,  0, 0,0, 0};
#define OPT_FINGERKEY44    8   /* Badge Deister 125khz wiegand 44 bits Traitement FingerKey */
static const sWiegandFormat Wieg44bitsFinger = { 44,  0, 0, 0,0,   0, 0, 0,0, 12,43,3,10,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_ANATOR36       9   /* Badge HID 125khz wiegand 36 bits Traitement Anator */
static const sWiegandFormat Wieg36bitsAnator = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,18,0, 6,  19,36,0, 6,  0, 0,0, 0};
#define OPT_CORNING32     10   /* Badge Indala 125khz wiegand 32 bits Format specif Corning */
static const sWiegandFormat Wieg32bitsCorning = { 32,  1, 2,13,0,  32,14,31,1,  2,13,0, 4,  14,31,0, 6,  0, 0,0, 0};
#define OPT_TECHTALK8     12   /* Badge Indala/HID 125khz wiegand 36 bits Format specif TechTalk decode 8 chiffres */
static const sWiegandFormat Wieg36btechtalk8  = { 36, 13,19,36,1,   0, 0, 0,0, 14,18,0, 2, 19,20,0, 1,  21,36,0, 5};
#define OPT_STID68DEC     13   /* Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor */
static const sWiegandFormat Wieg68bstiddec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1,  9,64,0,17,  0, 0,0, 0};
#define OPT_STID44AIA     14   /* Lecteur STID Marin 125kHz en interface 44 bits, decodage bdge DGA AIA Clermont Ferrand */
static const sWiegandFormat Wieg44bstidaia   = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_COTAG34       15   /* Lecteur COTAG xxxkHz en interface 34 bits, decodage badge MERIAL */
static const sWiegandFormat Wieg34bcotag     = { 34,  0, 0, 0,0,   0, 0, 0,0, 21,33,1, 4,  18,20,1, 1,   2,17,1, 5};   /* Sans ctrl parites + 3 champs */
#define OPT_TETRAPAK      16   /* Bagde Marin 125 kHz Lecteur Wiegand 44 bits = inversion binaire avant decodage */
static const sWiegandFormat Wieg44btetrapak  = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,14,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_HID37BCD      17   /* Bagde iClass 37 bits 8 quartets BCD => 8 decimal ascii */
static const sWiegandFormat Wieg37bitsbcd    = { 37,  1, 2,19,0,  37,19,36,1,  5,36,2, 8,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_44B_44BDEC    18   /* Decodage Wiegand 44 bits => 13 decimale + */
/* Decodage lecteur Deister Mullion interface Clock Data 44 bits => 13 decimale */
static const sWiegandFormat Wieg44bitsdec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,0,13,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_44B_MSPASS    19   /* Decodage Lecteur Stid "Ritzenthaler" Wiegand 44 bits => 13 decimal format MSPASS ISO2 */
static const sWiegandFormat Wieg44bitsmspass  = { 44,  0, 0, 0,0,   0, 0, 0,0,  9,20,0,4,   21,23,0, 3,  24,40,0,6};
#define OPT_68B_ASCII     20   /* Decodage Lecteur Stid Wiegand 64 bits ascii + 4 lrc => 8 ascii */
static const sWiegandFormat Wieg68bitsascii   = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,5,8,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_44B_2S_9D    21   /* Decodage Lecteur Stid Wiegand 44 bits en format 2S  => 9 decimal */
#define OPT_34B_URMET_8H  22   /* Decodage Lecteur Urmet Wiegand 34 bits => 8 hexadecimal majuscule */
static const sWiegandFormat Wieg34bitsUrmet_8H  = { 34,  1, 2,17,0,  34,18,33,1,  2,33,3,8,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_34B_URMET_10D 23   /* Decodage Lecteur Urmet Wiegand 34 bits => 10 decimal */
static const sWiegandFormat Wieg34bitsUrmet_10D = { 34,  1, 2,17,0,  34,18,33,1,  2,33,0,10,   0, 0,0, 0,  0, 0,0, 0};
#define OPT_92B_ASCII 24  /*  Decodage Lecteur Stid Wiegand 92 bits ascii => 11 ascii  */
static const sWiegandFormat Wieg92bitsAscii   = { 92,  0, 0, 0,0,   0, 0, 0,0,  1,88,5,11,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_CORNING32_35  25   /* Badge Indala et iClass sur lecteur MultiClass = wiegand 32 et 35 bits Format specif Corning */
static const sWiegandFormat Wieg32bitsiCorning = { 32,  1, 2,13,0,  32,14,31,1,  2,13,0, 4,  14,31,0, 7,  0, 0,0, 0};
static const sWiegandFormat Wieg35bitsiCorning = { 35,  1, 2,35,1,   0, 0, 0,0,  3,14,0, 4,  15,34,0, 7,  0, 0,0, 0};
#define OPT_68B_CG95      26   /* Decodage Lecteur Stid Wiegand 68 bits, 6 ascii decimal => 8 ascii decimal */
static const sWiegandFormat Wieg68bitscg95     = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,48,5,8,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_36B_CALNICE   27   /* Centre Antoine Lacassagne, Decodage Lecteur Stid Wiegand 36 bits, 8 BCD + LRC => 8 ascii decimal */
static const sWiegandFormat Wieg36bitscalnice  = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,32,2,8,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_64B_20DEC     28   /* S3V, Wiegand Stid 3T 68 bits, numero de serie ISO15693 64 bits converti en decimal sur 20 ascii */
static const sWiegandFormat Wieg68bit20dec     = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,0,20,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_64B_LSB_18DEC 29   /* Biomerieux, Wiegand Stid 3T 68 bits, decimal sur 1+17, decodage UID LSB first */
static const sWiegandFormat Wieg68b_56lsbdec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1,  9,64,6,17,  0, 0,0, 0};
static const sWiegandFormat Wieg68b_32lsbdec   = { 68,  0, 0, 0,0,   0, 0, 0,0,   5, 8,0, 1, 33,64,6,17,  0, 0,0, 0};
#define OPT_84B_ASCII     30   /*  Decodage Lecteur Stid Wiegand 84 bits ascii => 10 ascii (DCNS Cherbourg) */
static const sWiegandFormat Wieg84bitsAscii   = { 84,  0, 0, 0,0,   0, 0, 0,0,  1,80,5,10,    0, 0,0, 0,  0, 0,0, 0};
#define OPT_44B_28BDEC    31   /* wiegand 44 bits, 1 champ 12 bits en dec sur 4 + 1 champ 28 bits en decimal sur 9 */
const sWiegandFormat Wieg44b_28dec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,12,0, 4,  13,40,0, 9,  0, 0,0, 0};
#define OPT_44B_32BDEC    32   /* wiegand 44 bits, 1 champ 8 bits en dec sur 3 + 1 champ 32 bits en decimal sur 10 */
static const sWiegandFormat Wieg44b_32dec     = { 44,  0, 0, 0,0,   0, 0, 0,0,  1, 8,0, 3,   9,40,0,10,  0, 0,0, 0};
#define OPT_68B_BBP_10DEC 39   /* wiegand 68 bits, Lecteur MXS PHP, PUPI B et BPrim convertis en decimal sur 10 */
const sWiegandFormat Wieg68bits_bbp   = { 68,  1, 0, 0,4,   0, 0, 0,0,  1,64,3,16,  25,56,0,10, 33,64,6,10};
#define OPT_68_ACCOR_UID  47   /* lecteur Stid LXS-RX1 bi freq, Wiegand 68, badge Mifare UID et HID 125kHz Accor = Rectorat Poitiers */
/* utilise le décodage Wieg68bitshexa pour determiner type de puce, si HID OK applique ci-dessous */
static const sWiegandFormat Wieg68bitsAccor  = { 68,  1, 0, 0,4,   0, 0, 0,0,  37,44,0,11, 45,48,0, 2, 49,64,0, 5};

// --- formats "standard", differencies par nombre de bits
//                                                  bits|  parite 1 |  parite 2 |  data 1   |  Data 2    |  Data 3
static const sWiegandFormat Wieg25bitsNCS       = { 25, 25,17,24,0,   0, 0, 0,0, 17,24,0, 3,   2,16,0, 5,  0, 0,0, 0};
static const sWiegandFormat Wieg26bits          = { 26,  1, 2,13,0,  26,14,25,1,  2, 9,0, 3,  10,25,0, 5,  0, 0,0, 0};
static const sWiegandFormat Wieg30bitsAccor     = { 30,  1, 2,16,0,  30,17,29,1,  2, 9,0, 3,  10,13,0, 2, 14,29,0, 5};
static const sWiegandFormat Wieg34bits          = { 34, 34, 1,33,0,   0, 0, 0,0,  2,17,0, 5,  18,33,0, 5,  0, 0,0, 0};
static const sWiegandFormat Wieg35bHIDCorp      = { 35,  1, 2,35,1,   0, 0, 0,0,  3,14,0, 4,  15,34,0, 7,  0, 0,0, 0};
static const sWiegandFormat Wieg36bitsclock     = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,32,2, 9,   0, 0,0, 0,  0, 0,0, 0};
static const sWiegandFormat Wieg36btechtalk6    = { 36, 13,19,36,1,   0, 0, 0,0, 19,20,0, 1,  21,36,0, 5,  0, 0,0, 0};
static const sWiegandFormat Wieg37bitsnosite    = { 37,  1, 2,19,0,  37,19,36,1,  2,36,0,11,   0, 0,0, 0,  0, 0,0, 0};
static const sWiegandFormat Wieg44bitshexa      = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,3,10,   0, 0,0, 0,  0, 0,0, 0};
static const sWiegandFormat Wieg68bitshexa      = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,3,16,   0, 0,0, 0,  0, 0,0, 0};
static const sWiegandFormat Wieg112bitshexa     = {112,  0, 0, 0,0,   0, 0, 0,0, 1,108,2,27,   0, 0,0, 0,  0, 0,0, 0};


/* valeurs de sWiegandData.format */
#define WIEG_BINMSB_DEC  0  /* donnees recues en binaire MSB first converties en ascii decimal */
#define WIEG_BINLSB_DEC  1  /* donnees recues en binaire LSB first con(T_pFuncILV_Buffer) CommandCallback())verties en ascii decimal */
#define WIEG_BCD_DEC     2  /* donnees recues en BCD par quartet, converties en ascii decimal */
/* si long champ n'est pas un multiple de 4, on decoupe en quartet */
/* en partant de la fin le 1er caractere (MSB) sera traite avec le */
/* nbr de bits restant */
#define WIEG_HEXMSB_HEX  3  /* donnees recues en hexa MSB first converties en ascii hexa majuscule */
#define WIEG_HEXLSB_DEC  4  /* donnees recues en hexa LSB first (par quartet) converties en ascii hexa majuscule */
#define WIEG_ASCII       5  /* donnees recues en ascii, copiees directement dans lect->result, 8 bits wiegand par octet */
#define WIEG_BYTELSB_DEC 6  /* donnees recues en binaire, octet LSB first, bits MSB first converties en ascii decimal */


class BADGELIBSHARED_EXPORT BadgeLib
{

public:
    BadgeLib();
    void Debug(QString a);
    int wieg_chklrc4b(QBitArray bit, char lrc);
    QString trait_wiegand_struct(QBitArray bit, sWiegandFormat *wiegform);
    QString trait_hid_mifare_32bits(QBitArray bit, int option);
    QString trait_tech_talk(QBitArray bit, int option);
    QString trait_hid_56bits(QBitArray bit, int option);
    QString trait_wiegand44b_2s(QBitArray bit);
    QString trait_wiegand_std(QBitArray bit, int option);
    QString trait_motorola_hid(QBitArray bit, QString stOption);

private:
    bool badge_debug;
};

#endif // BADGELIB_H
