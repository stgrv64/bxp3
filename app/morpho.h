#ifndef MORPHO_H
#define MORPHO_H

#include"MSO100.h"
#include"Ilv_definitions.h"
#include <QMutex>
#include <QObject>

class Morpho : public QObject
{
    Q_OBJECT
    // DATA MORPHO
public:
    MORPHO_HANDLE i_h_Mso100Handle;
    //   UC	o_puc_ILV_Status;
    //    QString id;
    UC  identifiant[25];
    UL  index;
    unsigned char* blob;
    QMutex mutex;

signals:
    void signal_morpho_badge(QString);
    void signal_morpho_message(QString);

public slots:

    I MORPHO_ConvertError(UC i_uc_Status);
    I MORPHO_ConvertError_ihm(UC i_uc_Status);

    void MORPHO_Identify( void);

    void MORPHO_FillDataBase(void);
    I MORPHO_AddBaseRecord(  PT_BUF  l_x_UserID,
                             PT_BUF_PK i_px_Buf_Pk,
                             UC		   i_uc_NbPk);


    void MORPHO_EnrollUser(void);

    void MORPHO_DeleteUserIndex(void);
    void MORPHO_DeleteAllUser(void);

    void MORPHO_DestroyDataBase(void);
    void MORPHO_CreateDataBase(void);

    void MORPHO_GetBaseConfig(void);
    void MORPHO_ListDataBaseUser(void);
    void MORPHO_GetDescriptor(void);

    I SaveMinutiae (PT_BUF,PT_BUF,PT_BUF);


};
#endif // MORPHO_H
