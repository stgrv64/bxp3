#include <morpho.h>
#include <badge.h>
#include <supervision.h>
#include <drv_rs232.h>

// Error table struct
typedef struct
{
    UC	m_uc_ErrorCode;
    UC  m_auc_ErrorString[256];
} T_ERROR_TABLE;

// Error Table with string explanation
T_ERROR_TABLE g_x_ErrorTable[] =
{
    { ILV_OK,						"OK "},								 		// 0x00
    { ILVERR_ERROR,					"Erreur" },										// 0xFF
    { ILVERR_BADPARAMETER,			"Parametres Incorrects" }, 						// 0xFE
    { ILVERR_INVALID_MINUTIAE,		"Empreinte invalide" },      				 		// 0xFD
    { ILVERR_INVALID_USER_ID,		"Identifiant inconnu" },	// 0xFC
    { ILVERR_INVALID_USER_DATA,		"Data invalides" },    			 			// 0xFB
    { ILVERR_TIMEOUT,				"Pas de réponse avant timeout" }, 						// 0xFA
    { ILVERR_INVALID_ID_PROTOCOL,	"Protocole invalide" },						// 0xF9
    { ILVERR_ALREADY_ENROLLED,		"Identifiant deja enregistré"},						// 0xF8
    { ILVERR_BASE_NOT_FOUND,		"Base inconnue"},   					// 0xF7
    { ILVERR_BASE_ALREADY_EXISTS,	"Base déja existante"},    					// 0xF6
    { ILVERR_BIO_IN_PROGRESS,		"Commande déja en cours .."},			// 0xF5
    { ILVERR_CMD_INPROGRESS,		"Autre commande en cours"},		// 0xF4
    { ILVERR_FLASH_INVALID,			"Flash invalide" },										// 0xF3
    { ILVERR_NO_SPACE_LEFT,			"Mémoire saturée" },		// 0xF2
    { ILVERR_FIELD_NOT_FOUND, 		"Champ inexistant"  },   								// 0xE9
    { ILVERR_FIELD_INVALID,  		"Taille champ invalide"  },					// 0xE8
    { ILVERR_SECURITY_MODE,  		"Pas compatible avec mode sécurisé" },		// 0xE7
    { ILVERR_USER_NOT_FOUND, 		"Empreinte non trouvée" },							// 0xE6
    { ILVERR_CMDE_ABORTED,   		"Commande avortée" },					// 0xE5
    { ILVERR_SAME_FINGER,   		"2 templates pour même doigt" },				// 0xE4
    { ILVERR_NO_HIT,				"Empreinte non identifiée" },						// 0xE3
    { ILVERR_FFD,					"Doigt faux detecté"},									// 0xDB
    { ILVERR_MOIST_FINGER,			"Doigt sale detecté" },								// 0xDA
};

int	g_ul_NbError = sizeof(g_x_ErrorTable)/sizeof(T_ERROR_TABLE);

/*****************************************************************************/
/*****************************************************************************/
I Morpho::MORPHO_ConvertError(UC i_uc_Status)
{
    int l_i_i;

    for(l_i_i=0; l_i_i<g_ul_NbError; l_i_i++)
    {
        if(g_x_ErrorTable[l_i_i].m_uc_ErrorCode == i_uc_Status)
        {
            if (i_uc_Status != (UC)ILVERR_TIMEOUT)
            {
                fprintf(stderr, "Error: %s\n", g_x_ErrorTable[l_i_i].m_auc_ErrorString);
                signal_morpho_message(QString((char*)g_x_ErrorTable[l_i_i].m_auc_ErrorString));
            }
            break;
        }
    }

    if( l_i_i == g_ul_NbError)
    {
        fprintf(stderr, "Error 0x%02X: Unknown error \n", i_uc_Status);
        return -1;
    }

    return 0;
}
/*****************************************************************************/
I Morpho::MORPHO_ConvertError_ihm(UC i_uc_Status)
{
    int l_i_i;

    for(l_i_i=0; l_i_i<g_ul_NbError; l_i_i++)
    {
        if(g_x_ErrorTable[l_i_i].m_uc_ErrorCode == i_uc_Status)
        {
            if (i_uc_Status != (UC)ILVERR_TIMEOUT)
                signal_morpho_message(QString((char*)g_x_ErrorTable[l_i_i].m_auc_ErrorString));
            break;
        }
    }

    if( l_i_i == g_ul_NbError)
    {
        signal_morpho_message((QString) "Erreur inconnue");
        return -1;
    }

    return 0;
}


void Morpho::MORPHO_Identify(void)
{
    I  		l_i_Ret;
    I  		l_i_i;
    UC		l_uc_Status;
    UL		l_ul_UserDBIndex;
    UC		l_uc_MatchingResult;
    UL		l_ul_AddFieldNb;
    UL		l_ul_Score = 0;
    PUL		l_pul_Score;
    T_BIO_PARAM_PC	l_x_BioParamPC;
    T_BUF       l_x_UserID;
    UC		l_auc_UserID[25];
    T_BUF       l_x_addField[2];
    UC		l_auc_AddField1[25];
    UC		l_auc_AddField2[25];

    //if (badge_thread.badge.badge_debug) fprintf(stdout,"---Identifies a live finger against the local database---\n");
    badgelog->debug("---Identifies a live finger against the local database---");

    l_pul_Score = NULL;

    memset(l_auc_UserID, 0, sizeof(l_auc_UserID));
    memset(l_auc_AddField1, 0, sizeof(l_auc_AddField1));
    memset(l_auc_AddField2, 0, sizeof(l_auc_AddField2));
    memset(&l_x_UserID, 0, sizeof(l_x_UserID));
    memset(&l_x_addField, 0, sizeof(l_x_addField));

    l_x_UserID.m_puc_Buf = l_auc_UserID;
    l_x_addField[0].m_puc_Buf = l_auc_AddField1;
    l_x_addField[1].m_puc_Buf = l_auc_AddField2;
    l_ul_AddFieldNb = 2;

    memset(&l_x_BioParamPC, 0, sizeof(T_BIO_PARAM_PC));

    l_x_BioParamPC.m_i_ffd_security_level	= 5; //i_i_SecurityLevel;
    l_x_BioParamPC.m_i_Algo					= MORPHO_DEFAULT_CODER;
    l_x_BioParamPC.m_ul_DetectModeChoice	= MORPHO_VERIF_DETECT_MODE;
    l_x_BioParamPC.m_ul_MatcherMode			= MORPHO_STANDARD_MATCHING_STRATEGY;
    l_x_BioParamPC.m_ul_asynchronous_evt	= 0;//i_ul_AsynchronousEvnt;
    l_x_BioParamPC.m_pul_score				= l_pul_Score;
    l_x_BioParamPC.m_px_Timers				= NULL;
    l_x_BioParamPC.m_uc_LatentDetection		= 0;

    l_i_Ret = MSO_Bio_Identify_FingerVein (
                i_h_Mso100Handle,		//MORPHO_HANDLE		i_h_Mso100Handle,
                0,						//UC 				i_uc_UidDB,            MSO_Cancel(morpho.i_h_Mso100Handle);

                1,						//US				i_us_Timeout,
                5,						//US				i_us_MatchingTreshold,
                &l_uc_MatchingResult,	//PUC				o_puc_MatchingResult,
                &l_ul_UserDBIndex,		//PUL				o_pul_UserDBIndex,
                (PT_BUF)&l_x_UserID,	//PT_BUF			o_px_UserID,
                &l_ul_AddFieldNb,		//PUL				io_pul_AddFieldNb,
                (PT_BUF)&l_x_addField,	//PT_BUF			o_px_AddFieldValue,
                &l_uc_Status,		//PUC				o_puc_ILV_Status,
                &l_x_BioParamPC		//PT_BIO_PARAM_PC	io_px_BioParamPC
                );

    if(l_i_Ret==0 && l_uc_Status==0)
    {
        badgelog->debug("Identify -----> OK");
        badgelog->debug("MatchingResult=%d score=%ld", l_uc_MatchingResult, l_ul_Score);
        if(l_uc_MatchingResult == ILVSTS_HIT)
        {
            signal_morpho_badge((QString) (char*)l_x_UserID.m_puc_Buf);
            if (badgelog->isDebugEnabled())
            {
                badgelog->debug("User Identified--> index: %ld", l_ul_UserDBIndex);
                badgelog->debug("\tUser ID: %s", l_x_UserID.m_puc_Buf);
                for(l_i_i = 0; (UL)l_i_i < l_ul_AddFieldNb; l_i_i++)
                    badgelog->debug("\tAdd Field %d: %s", l_i_i, l_x_addField[l_i_i].m_puc_Buf);
            }
        }
        else if(l_uc_MatchingResult == ILVSTS_DB_EMPTY)
        {
            signal_morpho_badge((QString) "Base Vide");
            badgelog->debug("Base Vide");
        }
        else
        {
            signal_morpho_badge((QString) "Badge inconnu");
            badgelog->debug("User Not Identified !! ");
        }
    }
    else
    {
        if(l_uc_Status!=0) signal_morpho_badge((QString)"No Badge");
        else signal_morpho_badge((QString) "ERREUR");
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_Bio_Identify ret=%d status=0x%02X", l_i_Ret, l_uc_Status);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        l_i_Ret = -1;
    }
}



/***************************************************************************/
/**
* Create database of index 0
* - 3 Additionnal fields
* - Max User 100
* - 2 fingers per user
* - Standard PK
*/

void Morpho::MORPHO_CreateDataBase(void)
{
    I  		l_i_Ret;
    UC		l_uc_Status;
    T_FIELD     l_x_BaseAddField[3];

    memset((PC)&l_x_BaseAddField, 0, sizeof(l_x_BaseAddField));
    // Add Field 1
    l_x_BaseAddField[0].m_us_FieldMaxSize = 25;
    sprintf((PC)l_x_BaseAddField[0].m_auc_FieldName, "%s", "");
    l_x_BaseAddField[0].m_uc_Right = PUBLIC;

    // Add Field 2
    l_x_BaseAddField[1].m_us_FieldMaxSize = 25;
    sprintf((PC)l_x_BaseAddField[1].m_auc_FieldName, "%s", "");
    l_x_BaseAddField[1].m_uc_Right = PUBLIC;
    // Add Field 3
    l_x_BaseAddField[2].m_us_FieldMaxSize = 25;
    sprintf((PC)l_x_BaseAddField[2].m_auc_FieldName, "%s", "");
    l_x_BaseAddField[2].m_uc_Right = PUBLIC;

    l_i_Ret = MSO_BioDB_CreateDb( i_h_Mso100Handle,
                                  0,                             //UC       i_uc_IndexDB,
                                  500,                           //US       i_us_UserMax,
                                  2,                             //UC       i_uc_NbFinger,
                                  ID_BIO_ALGO_PARAM_PKCOMP,                             //UC       i_uc_NormalizedPK_Type,
                                  2,                             //UC       i_uc_NbAddField,
                                  0,				 			 //UC	    i_uc_encryptedDB,
                                  (PT_FIELD)&l_x_BaseAddField,   //PT_FIELD i_px_AddField,
                                  &l_uc_Status                   //PUC      o_puc_ILV_Status
                                  );
    if(l_i_Ret == 0 && l_uc_Status==0)
    {
        badgelog->debug("Create Base index 0 -----> OK");
        signal_morpho_message("Base Créee");
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_CreateDb ret=%d status=0x%02X", l_i_Ret, l_uc_Status);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        l_i_Ret = -1;
    }
}

/***************************************************************************/


void Morpho::MORPHO_FillDataBase(void)
{
    badgelog->debug("MORPHO_FillDataBase");
    if(!mutex.tryLock())
    {
        badgelog->debug("MORPHO_FillDataBase mutex Lock");
        return;
    }
    UL ID_pos;
    UL IDNEXT_pos;
    UC ID_len;
    UL TP1_pos=0;
    UL TP1_len=0;
    UL TP2_pos=0;
    UL TP2_len=0;
    UC          l_auc_UserID[25];
    UC          l_auc_tp1[200];
    UC          l_auc_tp2[200];
    T_BUF_PK       i_px_Buf_Pk[3];
    T_BUF l_x_UserID;
    int erreur=0;

    QFile file ("/var/x7/contents/biometrie/minutiae_save.bio");

    // NE PAS SUPPRIMER CETTE PARTIE SOFT CELA NE MARCHE PAS SANS ELLE !!!!!!!
    // ***********************************************************************
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            blob= NULL;
            blob= (UC*)file.readAll().constData();
            int nb=(int) (((char) *(blob+1)) *256 + ((char) *(blob+2)));
            if (nb >0) MORPHO_DeleteAllUser();
            sleep(1);
            file.close();
        }
    }
    // ************************************************************************


    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            blob= NULL;
            blob= (UC*)file.readAll().constData();
            file.close();
            int nb=(int) (((char) *(blob+1)) *256 + ((char) *(blob+2)));
            badgelog->debug("MORPHO_FillDataBase nb=%d",nb);
            ID_len=(UC) blob[0];
            ID_pos=3;
            IDNEXT_pos=3;
            int i;
            if (nb==0) signal_morpho_message("Fichier Sauvegarde Empreintes vide");
            else
            {
                for ( i=0;i<nb;i++)
                {
                    ID_pos=IDNEXT_pos;
                    TP1_pos= ID_pos+ID_len;
                    TP1_len=(UL) *(blob+TP1_pos);
                    TP2_pos=(TP1_pos+TP1_len+1);
                    TP2_len=(UL) *(blob+TP2_pos);
                    IDNEXT_pos=TP2_pos+TP2_len+1;

                    memcpy(l_auc_UserID,blob+ID_pos,ID_len );
                    memcpy(l_auc_tp1,blob+TP1_pos+1,TP1_len);
                    memcpy(l_auc_tp2,blob+TP2_pos+1,TP2_len);

                    l_x_UserID.m_ul_Size=ID_len;
                    l_x_UserID.m_puc_Buf=l_auc_UserID;

                    i_px_Buf_Pk[0].m_uc_IdPk=(UC) ID_PKCOMP;
                    i_px_Buf_Pk[0].m_x_Buf.m_puc_Buf=l_auc_tp1;
                    i_px_Buf_Pk[0].m_x_Buf.m_ul_Size=TP1_len;
                    i_px_Buf_Pk[1].m_uc_IdPk=(UC) ID_PKCOMP;
                    i_px_Buf_Pk[1].m_x_Buf.m_puc_Buf=l_auc_tp2;
                    i_px_Buf_Pk[1].m_x_Buf.m_ul_Size=TP2_len;
                    badgelog->debug("ID_p fin:%ld ID_l:%ld TP1_p:%ld TP1_l:%ld TP2_p:%ld TP2_l:%ld ID_NEXT:%ld", ID_pos, ID_len, TP1_pos, TP1_len, TP2_pos, TP2_len, IDNEXT_pos);
                    if (MORPHO_AddBaseRecord(&l_x_UserID,&i_px_Buf_Pk[0],2) ==-1)
                    {
                        erreur=-1;
                        badgelog->debug("Erreur Fichier Corrompu !!");
                        break;
                    }
                }
                //badgelog->debug("Segmentation sync 1");
                if (erreur==-1) signal_morpho_message("Installation Incomplète !! Veuillez Relancer");
                else signal_morpho_message("Base Installée");
                //badgelog->debug("Segmentation sync 2");
            }
        }
        else signal_morpho_message("Erreur Fichier Sauvegarde Empreintes");
    }
    else signal_morpho_message("Fichier Sauvegarde Empreintes Inexistant");
    //badgelog->debug("Segmentation sync 3");
    mutex.unlock();
    return;
}

// Bio Function addbaserecord
/***************************************************************************/

I Morpho::MORPHO_AddBaseRecord(  PT_BUF    l_x_UserID,
                                 PT_BUF_PK i_px_Buf_Pk,
                                 UC		   i_uc_NbPk
                                 )
{

    I	l_i_Ret;
    UC	l_uc_Status;
    UC	l_uc_BaseStatus;
    UL  l_ul_IndexUser;
    UC          l_auc_AddField1[25];
    UC          l_auc_AddField2[25];
    T_BUF       l_x_addField[2];

    badgelog->debug("---Add Base Record:---");
    *l_auc_AddField1=0;
    *l_auc_AddField2=0;
    l_x_addField[0].m_ul_Size = strlen((PC)l_auc_AddField1);
    l_x_addField[0].m_puc_Buf = l_auc_AddField1;

    l_x_addField[1].m_ul_Size = strlen((PC)l_auc_AddField2);
    l_x_addField[1].m_puc_Buf = l_auc_AddField2;


    l_i_Ret = MSO_BioDbAddBaseRecord_FingerVein(i_h_Mso100Handle,// MORPHO_HANDLE i_h_Mso100Handle,
                                                (UC) 0,		//UC                     i_uc_IndexDB,
                                                i_uc_NbPk,	//UC                     i_uc_NbPk,
                                                i_px_Buf_Pk,	//PT_BUF_PK              i_px_Pk,
                                                l_x_UserID,	//PT_BUF                 i_px_UserId,
                                                (UC) 2,		//UC                     i_uc_NbAddField,
                                                (PT_BUF)&l_x_addField,	//PT_BUF                 i_px_AddField,
                                                (UL) 1,				//UL					i_ul_pkUpdateMask,
                                                &l_ul_IndexUser,  //PUL                    o_pul_IndexUser,
                                                &l_uc_Status,	//PUC                    o_puc_ILV_Status,
                                                &l_uc_BaseStatus, //PUC                    o_puc_Base_Status,
                                                (BOOL) 1			//BOOL                   i_b_NoCheckOnTemplate
                                                );
    if(l_i_Ret==0 && l_uc_Status==0)
    {
        badgelog->debug("Add Base Record -----> OK");
        badgelog->debug("User Index: %ld", l_ul_IndexUser);
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDBAddBaseRecord ret=%d status=0x%02X BaseStatus=0x%02X", l_i_Ret, l_uc_Status, l_uc_BaseStatus);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }
    return(l_i_Ret);
}





I BiometricCallback(PVOID i_pv_context, T_MORPHO_CALLBACK_COMMAND i_i_command, PVOID i_pv_param)
{
    Q_UNUSED(i_pv_context);
    PUC l_puc_EnrollmentCmd;
    QString a,b,c;

    //    T_MORPHO_CALLBACK_IMAGE_STATUS l_x_ImageStructure;
    //    C l_c_String[256];
    //    UC l_uc_quality;

    //    badgelog->debug("T_MORPHO_CALLBACK_COMMAND=%s", i_i_command);
    if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
    {
        switch (*((PI) i_pv_param))
        {
        case MORPHO_REMOVE_FINGER:
            b=QString("ENLEVER LE DOIGT !!\n");
            break;
        case MORPHO_MOVE_NO_FINGER:
            b=QString("ATTENTE DOIGT\n");
            break;
        case MORPHO_LATENT:
            b=QString("LATENT\n");
            break;
        case MORPHO_MOVE_FINGER_UP:
            b=QString("DEPLACER LE DOIGT VERS LE BAS\n");
            break;
        case MORPHO_MOVE_FINGER_DOWN:
            b=QString("DEPLACER LE DOIGT VERS LE HAUT\n");
            break;
        case MORPHO_MOVE_FINGER_LEFT:
            b=QString("DEPLACER LE DOIGT VERS LA DROITE\n");
            break;
        case MORPHO_MOVE_FINGER_RIGHT:
            b=QString("DEPLACER LE DOIGT VERS LA GAUCHE\n");
            break;
        case MORPHO_PRESS_FINGER_HARDER:
            b=QString("APPUYER PLUS FORT !!!\n");
            break;
        case MORPHO_FINGER_OK:
            b=QString("Acquisition ok\n");
            break;
        default:
            break;
        }
        badge_thread.badge.message2=b;
        c.clear();
        c.append(badge_thread.badge.message1);
        c.append(b);
        badge_thread.badge.morpho_message(c);
    }

    if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
    {
        badge_thread.badge.message2.clear();
        l_puc_EnrollmentCmd = (PUC) i_pv_param;
        switch (l_puc_EnrollmentCmd[0])
        {
        case 1:
            switch (l_puc_EnrollmentCmd[2])
            {
            case 1:
                a=QString("Présenter 1er Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;

            case 2:
                a=QString("Re-Présenter 1er Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;

            case 3:
                a=QString("Re-Présenter 1er Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;
            }
            break;

        case 2:
            switch (l_puc_EnrollmentCmd[2])
            {
            case 1:
                a=QString("ATTENTION CHANGEMENT DE DOIGT !! \n Présenter 2ème Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;

            case 2:
                a=QString("Re-Présenter 2ème Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;

            case 3:
                a=QString("Re-Présenter 2ème Doigt -> Capture %1/%2\n").arg(l_puc_EnrollmentCmd[2]).arg(l_puc_EnrollmentCmd[3]);
                break;
            }
            break;
        }
        badge_thread.badge.message1=a;
        badge_thread.badge.morpho_message(badge_thread.badge.message1.append(badge_thread.badge.message2));
    }

    if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD)
    {
        /* badgelog->debug("IMAGE");
       memcpy( &l_x_ImageStructure.m_x_ImageHeader,
                (T_MORPHO_IMAGE_HEADER *) i_pv_param,
                sizeof (T_MORPHO_IMAGE_HEADER));
        l_x_ImageStructure.m_puc_Image = (PUC) i_pv_param + sizeof (T_MORPHO_IMAGE_HEADER);
        printf("Async Image : %dx%d\n",
                    l_x_ImageStructure.m_x_ImageHeader.m_us_NbRow,
                    l_x_ImageStructure.m_x_ImageHeader.m_us_NbCol);
*/
    }

    if (i_i_command == MORPHO_CALLBACK_CODEQUALITY)
    {
        /*    l_uc_quality = *(PUC) i_pv_param;
        sprintf(l_c_String, "Code Quality : %d\n", l_uc_quality);
        printf ("%s", l_c_String);
    */
    }

    if (i_i_command == MORPHO_CALLBACK_DETECTQUALITY)
    {
        /*  l_uc_quality = *(PUC) i_pv_param;
        sprintf (l_c_String, "Detect Quality : %d\n", l_uc_quality);
        printf ("%s", l_c_String);
    */
    }

    return 0;
}

// Bio Function
/***************************************************************************/
/**
* Enroll User.
* Captures and enrolls live fingers
* - 1 User ID and 2 additionnal fields
* - 2 fingers per user
* - 3 acquisitions per finger
* - Standard PK
*/
void Morpho::MORPHO_EnrollUser(void)
{
    I  		l_i_Ret;
    UC		l_uc_Status;
    T_BIO_PARAM_PC l_x_BioParamPC;
    T_BUF	l_x_addField[3];
    UC		l_uc_EnrollStatus;
    UL		l_ul_UserDBIndex;
    UC		l_auc_AddField1[25];
    UC		l_auc_AddField2[25];
    T_BUF	minutiae_t_buf;
    T_BUF	minutiae1_t_buf;
    UC minutiae[260];
    UC minutiae1[260];
    QString a,b,id;

    //    UC		l_auc_UserID[25];

    /*        typedef struct
            {
                typedef struct
                {
                    UL			m_ul_Size;
                    PUC			m_puc_Buf;
                }T_BUF, *PT_BUF;
                PT_BUF		io_px_BiometricData;
                UC			i_uc_ExportMinutiae;
                UC			pad[3];
            }T_EXPORT_PK, *PT_EXPORT_PK;
    */




    PT_EXPORT_IMAGE l_px_ExportImage;
    l_px_ExportImage = NULL;

    T_EXPORT_PK        l_x_ExportPK;
    PT_EXPORT_PK        l_px_ExportPK;
    l_px_ExportPK = NULL;
    l_px_ExportPK = &l_x_ExportPK;

    if (l_px_ExportPK != NULL)
    {
        l_x_ExportPK.i_uc_ExportMinutiae=1;
        l_x_ExportPK.io_px_BiometricData=&minutiae_t_buf;

        l_x_ExportPK.io_px_BiometricData->m_ul_Size = 256;
        l_x_ExportPK.io_px_BiometricData->m_puc_Buf =minutiae;
        memset ( l_x_ExportPK.io_px_BiometricData->m_puc_Buf, 0, 256);
    }

    T_EXPORT_PK        l_x_ExportPK1;
    PT_EXPORT_PK        l_px_ExportPK1;
    l_px_ExportPK1 = NULL;
    l_px_ExportPK1 = &l_x_ExportPK1;

    if (l_px_ExportPK1 != NULL)
    {
        l_x_ExportPK1.i_uc_ExportMinutiae=1;
        l_x_ExportPK1.io_px_BiometricData=&minutiae1_t_buf;

        l_x_ExportPK1.io_px_BiometricData->m_ul_Size = 256;
        l_x_ExportPK1.io_px_BiometricData->m_puc_Buf = minutiae1;
        memset ( l_x_ExportPK1.io_px_BiometricData->m_puc_Buf, 0, 256);
    }


    badgelog->debug("---Captures and enrolls live fingers---");
    badgelog->debug("identifiant %s",identifiant);

    l_auc_AddField1[0] = 0;	// Suppress '\n'
    l_auc_AddField2[0] = 0;	// Suppress '\n'

    l_x_addField[0].m_ul_Size =  strlen((PC)identifiant);
    l_x_addField[0].m_puc_Buf = identifiant;

    l_x_addField[1].m_ul_Size = strlen((PC)l_auc_AddField1)+1;
    l_x_addField[1].m_puc_Buf = l_auc_AddField1;

    l_x_addField[2].m_ul_Size = strlen((PC)l_auc_AddField2)+1;
    l_x_addField[2].m_puc_Buf = l_auc_AddField2;

    memset(&l_x_BioParamPC, 0, sizeof(T_BIO_PARAM_PC));

    l_x_BioParamPC.m_i_ffd_security_level	= 5;   // i_i_SecurityLevel;
    l_x_BioParamPC.m_i_Algo					= MORPHO_DEFAULT_CODER;
    l_x_BioParamPC.m_ul_DetectModeChoice	= MORPHO_ENROLL_DETECT_MODE;
    l_x_BioParamPC.m_ul_MatcherMode			= MORPHO_STANDARD_MATCHING_STRATEGY;

    l_x_BioParamPC.m_ul_asynchronous_evt	=  MORPHO_CALLBACK_DETECTQUALITY | MORPHO_CALLBACK_CODEQUALITY | MORPHO_CALLBACK_IMAGE_CMD |
            MORPHO_CALLBACK_COMMAND_CMD | MORPHO_CALLBACK_ENROLLMENT_CMD; // i_ul_AsynchronousEvnt;
    l_x_BioParamPC.m_pul_score				= NULL;
    l_x_BioParamPC.m_px_Timers				= NULL;
    l_x_BioParamPC.m_uc_LatentDetection		= 0;

    //  typedef I (*T_pFuncILV_Buffer)(PVOID,I,PVOID)=CommandCallback();
    badge_thread.badge.message1="";
    badge_thread.badge.message2="";

    MSO_RegisterAsyncILV(i_h_Mso100Handle,
                         (I)l_x_BioParamPC.m_ul_asynchronous_evt,
                         (T_pFuncILV_Buffer) &BiometricCallback,
                         (PVOID) NULL);
    //    MSO_RegisterAsyncILV(i_h_Mso100Handle,l_x_BioParamPC.m_ul_asynchronous_evt,(T_MORPHO_CALLBACK_FUNCTION) &badge_thread.badge.morpho.BiometricCallback,NULL);
    l_i_Ret = MSO_Bio_Enroll_FingerVein (
                i_h_Mso100Handle,	//MORPHO_HANDLE				i_h_Mso100Handle,
                (UC) 0,					//UC 						i_uc_IndexDB,
                (US) 0,          		//US						i_us_Timeout,
                (US) 0,					//US						i_us_Acquisition_Threshold,
                (UC) 0,					//UC						i_uc_EnrollmentType,
                (UC) 2,					//UC						i_uc_NbFinger,
                (UC) 1,					//UC						i_uc_SaveRecord,
                (UC) MORPHO_PK_COMP,	//UC				i_uc_NormalizedPK_Type,
                (UC) ID_PK_FVP,	//UC						        i_uc_NormalizedFVP_Type,
                (UC) 3,					//UC						i_uc_NbAddField,
                (PT_BUF) &l_x_addField,	//PT_BUF				i_px_AddField,
                (UL) 0,					//UL						i_ul_pkUpdateMask,
                (PT_EXPORT_PK) l_px_ExportPK,   //    PT_EXPORT_PK          io_px_ExportPk;
                (PT_EXPORT_PK) l_px_ExportPK1,       		//PT_EXPORT_PK              io_px_ExportFVP,
                (PT_EXPORT_IMAGE) l_px_ExportImage,           	//PT_EXPORT_IMAGE		    io_px_ExportImage,
                (PUC) &l_uc_EnrollStatus,	//PUC					    o_puc_EnrollStatus,
                (PUL) &l_ul_UserDBIndex,	//PUL					    o_pul_UserDBIndex,
                (PUC) &l_uc_Status,		//PUC					    o_puc_ILV_Status,
                (T_MORPHO_TEMPLATE_ENVELOP) MORPHO_RAW_TEMPLATE, //T_MORPHO_TEMPLATE_ENVELOP i_x_typEnvelop,
                (I) 0,					//I							i_i_ApplicationDataLen,
                (PUC) NULL,				//PUC						i_puc_ApplicationData,
                (UC) 0,					//UC						i_uc_ExportCoderResult,
                (PT_BUF) NULL,				//PT_BUF					io_px_ExportResultCoder,
                (PT_BUF)NULL,				//PT_BUF					io_px_ExportMatrixDirections,
                (PT_BUF)NULL,				//PT_BUF					o_px_ExportMatrixQualities,
                (PT_BIO_PARAM_PC) &l_x_BioParamPC,	//PT_BIO_PARAM_PC			io_px_BioParamPC,
                (UC) 0,					//UC						i_uc_SaveImageIndex,
                (PT_MORPHO_MOC_PARAMETERS) NULL,				//PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
                (PUC) NULL,				//PUC						io_puc_PkFpQuality,
                (PUC) NULL,				//PUC						io_puc_AdvancedSecurityLevelCompatibility
                (PT_BIO_DATA_ENCRYPTION) NULL,
                (PT_EXPORT_IMAGE_FFD) NULL
                );

    if(l_i_Ret==0 && l_uc_Status==0)
    {
        badgelog->debug("Enroll -----> OK");
        badgelog->debug("Enroll Status: %d", l_uc_EnrollStatus);
        badgelog->debug("User Database Index: %ld", l_ul_UserDBIndex);
        badgelog->debug("longueur minutiae: %ld",l_x_ExportPK.io_px_BiometricData->m_ul_Size);
        badgelog->debug("longueur minutiae1: %ld",l_x_ExportPK1.io_px_BiometricData->m_ul_Size);
        id=(QString) (C*)identifiant;
        SaveMinutiae(&l_x_addField[0],l_x_ExportPK.io_px_BiometricData,l_x_ExportPK1.io_px_BiometricData);
        b.setNum(l_ul_UserDBIndex,10);
        a=QString("%1\nIndex: %2\nEnregistrement terminé").arg(id).arg(b);
        signal_morpho_message(a);
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_Bio_Enroll ret=%d status=0x%02X EnrollStatus=%d",l_i_Ret, l_uc_Status, l_uc_EnrollStatus);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }

}



I
Morpho::SaveMinutiae (PT_BUF identifiant,PT_BUF minutiae,PT_BUF minutiae1)
{
    I l_i_Ret;int nb;
    l_i_Ret=0;
    FILE * fp;
    long lSize;
    char buffer[5];
    nb=1;

    fp = fopen ( "/var/x7/contents/biometrie/minutiae.bio" , "r" );
    if (fp ==NULL)
    {
        fp = fopen ( "/var/x7/contents/biometrie/minutiae.bio" , "w" );
        buffer[0]=identifiant->m_ul_Size;
        buffer[1]=0;
        buffer[2]=1;
        fwrite (&buffer[0], sizeof(char), 3, fp);
        l_i_Ret += fwrite (identifiant->m_puc_Buf, sizeof(char), identifiant->m_ul_Size, fp);
        l_i_Ret += fwrite (& minutiae->m_ul_Size, sizeof(char), 1, fp);
        l_i_Ret += fwrite (minutiae->m_puc_Buf, sizeof(char), minutiae->m_ul_Size, fp);
        l_i_Ret += fwrite (&minutiae1->m_ul_Size, sizeof(char), 1, fp);
        l_i_Ret += fwrite (minutiae1->m_puc_Buf, sizeof(char), minutiae1->m_ul_Size, fp);
        fflush(fp);
        fclose(fp);
    }
    else
    {
        fp = fopen ( "/var/x7/contents/biometrie/minutiae.bio" , "rw+" );
        // obtain file size:
        fseek (fp , 0 , SEEK_END);
        lSize = ftell (fp);
        rewind (fp);
        // copy the file into the buffer:
        if (lSize >3)
        {
            fread(&buffer[0],sizeof(char),3,fp);
            nb=buffer[1]*256+buffer[2];
            nb++;
        }
        buffer[0]=identifiant->m_ul_Size;
        buffer[1]=(char)(nb/256);
        buffer[2]=(char)(nb & 0xFF);
        rewind (fp);
        fwrite (&buffer[0], sizeof(char), 3, fp);
        fflush(fp);


        fseek(fp,0,SEEK_END);
        l_i_Ret += fwrite (identifiant->m_puc_Buf, sizeof(char), identifiant->m_ul_Size, fp);
        l_i_Ret += fwrite (& minutiae->m_ul_Size, sizeof(char), 1, fp);
        l_i_Ret += fwrite (minutiae->m_puc_Buf, sizeof(char), minutiae->m_ul_Size, fp);
        l_i_Ret += fwrite (&minutiae1->m_ul_Size, sizeof(char), 1, fp);
        l_i_Ret += fwrite (minutiae1->m_puc_Buf, sizeof(char), minutiae1->m_ul_Size, fp);
        fflush(fp);
        fclose (fp);

        badgelog->debug("--> Save %d bytes", l_i_Ret);

    }
    sleep(1);
    QFile file ("/var/x7/contents/biometrie/minutiae.bio");
    if (file.exists()) {

        QFile file1 ("/var/x7/contents/biometrie/minutiae_save.bio");
        file1.remove();
        file.copy("/var/x7/contents/biometrie/minutiae_save.bio");
        file1.flush();
    }

    return l_i_Ret;
}






/***************************************************************************/

// Database function
/***************************************************************************/
/**
* Delete user of index i_ul_IndexUser in database of index 0
*
*/
void Morpho::MORPHO_DeleteUserIndex(void)
{
    I   l_i_Ret;
    UC	l_uc_Status;
    UL  l_ul_EmbeddedError;
    UL	l_ul_IndexUser;

    //    C	l_ac_String[32];

    //   fprintf(stdout, "Enter Index of User to delete:\n->");
    //   fgets(l_ac_String, 32, stdin);
    //    sscanf(l_ac_String, "%ld", &l_ul_IndexUser);

    l_ul_IndexUser=index;
    l_i_Ret = MSO_BioDB_DeleteUser( i_h_Mso100Handle,
                                    0,                   //UC              i_uc_IndexDB,
                                    l_ul_IndexUser,      //UL              i_ul_IndexUser,
                                    &l_uc_Status,        //PUC             o_puc_ILV_Status,
                                    &l_ul_EmbeddedError  //PUL             o_pul_EmbeddedError
                                    );
    if(l_i_Ret==0 && l_uc_Status==0 && l_ul_EmbeddedError==0)
    {
        badgelog->debug("Delete user with index %ld in Base index 0 -----> OK", l_ul_IndexUser);
        l_i_Ret = 0;
        QString b;
        b.setNum(l_ul_IndexUser,10);
        QString a=QString("Index: %1 Effacé").arg(b);
        signal_morpho_message(a);
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_DeleteUser ret=%d status=0x%02X EmbeddedError=%ld",l_i_Ret, l_uc_Status, l_ul_EmbeddedError);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }
}

/*
* Delete all user in database of index 0
*
*/

void Morpho::MORPHO_DeleteAllUser(void)
{
    I   l_i_Ret;
    UC	l_uc_Status;
    UL  l_ul_EmbeddedError;

    l_i_Ret = MSO_BioDB_EraseDb( i_h_Mso100Handle,
                                 0,                         //UC   i_uc_IndexDB,
                                 &l_uc_Status,              //PUC  o_puc_ILV_Status,
                                 &l_ul_EmbeddedError        //PUL  o_pul_EmbeddedError
                                 );
    if(l_i_Ret==0 && l_uc_Status==0 && l_ul_EmbeddedError==0)
    {
        badgelog->debug("Erase all user in Base index 0 -----> OK");
        l_i_Ret = 0;
        signal_morpho_message("Base effacée");
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_EraseDb ret=%d status=0x%02X EmbeddedError=%ld",l_i_Ret, l_uc_Status, l_ul_EmbeddedError);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }


}
/***************************************************************************/
/**
* Destroy database of index 0
*
*/

void Morpho::MORPHO_DestroyDataBase(void)
{
    I   l_i_Ret;
    UC	l_uc_Status;
    UL  l_ul_EmbeddedError;

    l_i_Ret = MSO_BioDB_DestroyDb( i_h_Mso100Handle,
                                   0,
                                   &l_uc_Status,              //PUC  o_puc_ILV_Status,
                                   &l_ul_EmbeddedError        //PUL  o_pul_EmbeddedError
                                   );
    if(l_i_Ret == 0 && l_uc_Status==0 && l_ul_EmbeddedError ==0)
    {
        badgelog->debug("Destroy Base index 0 -----> OK");
        l_i_Ret = 0;
        signal_morpho_message("Base Détruite");
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_DestroyDb ret=%d status=0x%02X EmbeddedError=%ld",l_i_Ret, l_uc_Status, l_ul_EmbeddedError);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }
}

/**
* Get Base Configuration
*
*/

void Morpho::MORPHO_GetBaseConfig(void)
{
    I  		l_i_Ret;
    UC		l_uc_Status;
    UL  	l_ul_EmbeddedError;
    I		l_i_i;
    T_FIELD     l_x_BaseAddField[2];
    UL 		l_ul_AddFieldNb;
    UC		l_uc_FingerNb;
    UL		l_ul_CurrentRecord;
    UC		l_uc_NormalizedPK_Type;
    UL		l_ul_MaxRecord;
    UC		l_uc_EncrytedDB;

    memset((PC)&l_x_BaseAddField, 0, sizeof(l_x_BaseAddField));
    l_ul_AddFieldNb = 2;
    l_uc_FingerNb = 0;
    l_ul_CurrentRecord = 0;
    l_i_Ret = MSO_BioDB_GetBaseConfig( i_h_Mso100Handle,
                                       0,                           //UC              i_uc_IndexDB,
                                       &l_uc_FingerNb,              //PUC             o_puc_FingerNb,
                                       &l_ul_MaxRecord,             //PUL             o_pul_MaxRecord,
                                       &l_ul_CurrentRecord,         //PUL             o_pul_CurrentRecord,
                                       &l_uc_EncrytedDB,            //PUC             o_puc_EncrytedDB,
                                       &l_ul_AddFieldNb,    	    //PUL             io_pul_AddFieldNb,
                                       (PT_FIELD)&l_x_BaseAddField, //PT_FIELD        o_px_AddField,
                                       &l_uc_NormalizedPK_Type,     //PUC             o_puc_NormalizedPK_Type,
                                       &l_uc_Status,                //PUC             o_puc_ILV_Status,
                                       &l_ul_EmbeddedError          //PUL             o_pul_EmbeddedError
                                       );
    if(l_i_Ret==0 && l_uc_Status==0 && l_ul_EmbeddedError==0)
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("Get Base Config -----> OK");
            badgelog->debug("\tNormalized PK:         %d", l_uc_NormalizedPK_Type);
            badgelog->debug("\tFinger Number:         %d", l_uc_FingerNb);
            badgelog->debug("\tMax record number:     %ld", l_ul_MaxRecord);
            badgelog->debug("\tCurrent Record number: %ld", l_ul_CurrentRecord);
            badgelog->debug("\tEncrypted database ?   %s", ((l_uc_EncrytedDB == 0) ? "No" : "Yes"));
            badgelog->debug("\tAdd field Number:      %ld", l_ul_AddFieldNb);
            for(l_i_i=0; (UL)l_i_i<l_ul_AddFieldNb; l_i_i++)
                badgelog->debug("\t-->%s", l_x_BaseAddField[l_i_i].m_auc_FieldName);
        }
        QString a=QString("Nb Empreintes/Nb Max: %1/%2").arg(l_ul_CurrentRecord).arg(l_ul_MaxRecord);
        signal_morpho_message(a);
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_GetBaseConfig ret=%d status=0x%02X EmbeddedError=%ld",l_i_Ret, l_uc_Status, l_ul_EmbeddedError);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        if(l_uc_Status != 0) MORPHO_ConvertError_ihm(l_uc_Status);
        l_i_Ret = -1;
    }
}



/***************************************************************************/
/*
*/
void Morpho::MORPHO_ListDataBaseUser(void)
{
    I				   l_i_Ret;
    I				   l_i_i;
    UC				   l_uc_Status;
    UL				   ul_NbTransport;
    PT_TRANSPORT_PUBLIC_LIST_FIELD l_px_TransportPublicField;

    UC 				   l_uc_FingerNb;
    UC 				   l_uc_NormalizedPK_Type;
    UL 				   l_ul_MaxRecord;
    UL 				   l_ul_CurrentRecord;
    UL 				   l_ul_AddFieldNb;
    T_FIELD        		   l_x_BaseAddField[2];
    UL				   l_ul_EmbeddedError;

    // GetBaseConfig to get current number user in database
    memset((PC)&l_x_BaseAddField, 0, sizeof(l_x_BaseAddField));
    l_ul_AddFieldNb = 2;
    l_i_Ret = MSO_BioDB_GetBaseConfig( i_h_Mso100Handle,
                                       0,                           //UC              i_uc_IndexDB,
                                       &l_uc_FingerNb,              //PUC             o_puc_FingerNb,
                                       &l_ul_MaxRecord,             //PUL             o_pul_MaxRecord,
                                       &l_ul_CurrentRecord,         //PUL             o_pul_CurrentRecord,
                                       0, //&l_uc_EncrytedDB,
                                       &l_ul_AddFieldNb,            //PUL             io_pul_AddFieldNb,
                                       (PT_FIELD)&l_x_BaseAddField, //PT_FIELD        o_px_AddField,
                                       &l_uc_NormalizedPK_Type,     //PUC             o_puc_NormalizedPK_Type,
                                       &l_uc_Status,                //PUC             o_puc_ILV_Status,
                                       &l_ul_EmbeddedError          //PUL             o_pul_EmbeddedError
                                       );
    if(l_i_Ret==0 && l_uc_Status==0)
    {
        badgelog->debug("Current User Number in Database:%ld", l_ul_CurrentRecord);
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("MSO_BioDB_GetBaseConfig ret=%d status=0x%02X", l_i_Ret, l_uc_Status);
            if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
        }
        return;
    }


    ul_NbTransport = l_ul_CurrentRecord;	// Must be >= to number of current record in database.
    // Retrieve Current number with GetBaseConfig
    l_px_TransportPublicField = (PT_TRANSPORT_PUBLIC_LIST_FIELD) malloc(sizeof(T_TRANSPORT_PUBLIC_LIST_FIELD)*ul_NbTransport);
    if(l_px_TransportPublicField == NULL)
    {
        perror("error malloc");
        return ;
    }
    memset(l_px_TransportPublicField, 0, sizeof(T_TRANSPORT_PUBLIC_LIST_FIELD)*ul_NbTransport);

    for(l_i_i=0; (UL)l_i_i<ul_NbTransport; l_i_i++)
    {
        l_px_TransportPublicField[l_i_i].m_puc_Data = (PUC)malloc(128);
        if(l_px_TransportPublicField[l_i_i].m_puc_Data == NULL)
        {
            perror("error malloc");
            l_i_Ret = -1;
            break;
        }
        memset(l_px_TransportPublicField[l_i_i].m_puc_Data, 0, 128);
        l_px_TransportPublicField[l_i_i].m_ul_DataLenght = 128;
    }

    if(l_i_Ret == 0)
    {
        l_i_Ret = MSO_BioDB_GetPublicListData( i_h_Mso100Handle,
                                               0,			     //UC  i_uc_IndexDB,
                                               0,			     //UL  i_ul_UidData,
                                               &ul_NbTransport,	             //PUL io_pul_NbTranport,
                                               l_px_TransportPublicField,    //PT_TRANSPORT_PUBLIC_LIST_FIELD  io_ax_Strct,
                                               &l_uc_Status		     //PUC o_puc_ILV_Status
                                               );



        if(l_i_Ret==0 && l_uc_Status==0)
        {
            if (badgelog->isDebugEnabled())
            {
                badgelog->debug("List User -----> OK");
                badgelog->debug("User Number: %ld", ul_NbTransport);
                for(l_i_i=0; (UL)l_i_i<ul_NbTransport; l_i_i++)
                    badgelog->debug("\tIndex %ld: %s", l_px_TransportPublicField[l_i_i].m_ul_UserIndex,
                            l_px_TransportPublicField[l_i_i].m_puc_Data);
            }
        }
        else
        {
            if (badgelog->isDebugEnabled())
            {
                badgelog->debug("MSO_BioDB_GetPublicListData ret=%d status=0x%02X", l_i_Ret, l_uc_Status);
                if(l_uc_Status != 0) MORPHO_ConvertError(l_uc_Status);
            }
            l_i_Ret = -1;
        }
    }

    for(l_i_i=0; (UL)l_i_i<ul_NbTransport; l_i_i++)
    {
        if(l_px_TransportPublicField[l_i_i].m_puc_Data != NULL)
            free(l_px_TransportPublicField[l_i_i].m_puc_Data);
    }
    if(l_px_TransportPublicField != NULL)
        free(l_px_TransportPublicField);

}


void Morpho::MORPHO_GetDescriptor(void)
{
    I               l_i_Ret;
    UC              l_uc_Status;
    UC              l_auc_DescProduct[1024];
    UC              l_auc_DescSensor[1024];
    UC              l_auc_DescSoftware[1024];
    /*I				l_i_speed;
C               l_ac_Device[32];
UC				l_uc_ConnectionType;
I               l_i_IdProduct;
UC				l_auc_SerialNumber[MSO_SERIAL_NUMBER_LEN+1];
UC				l_uc_SecuConfig;
US				l_us_SecuMaxFAR;
I               option;
C               *liste_options = "c:s:d:h";
*/

    // Send GetDescriptor ILV
    l_i_Ret = MSO_GetDescriptor( i_h_Mso100Handle,
                                 ID_FORMAT_TEXT,                  //UC      i_uc_DescFormat,
                                 &l_uc_Status,                      //PUC     o_puc_ILV_Status,
                                 l_auc_DescProduct,               //VOID*   i_pv_DescProduct,
                                 1024,                            //UL      i_ul_SizeOfDescProduct,
                                 l_auc_DescSensor,                //VOID*   i_pv_DescSensor,
                                 1024,                            //UL      i_ul_SizeOfDescSensor,
                                 l_auc_DescSoftware,              //VOID*   i_pv_DescSoftware,
                                 1024                             //UL      i_ul_SizeOfDescSoftware
                                 );
    if( (l_i_Ret == 0) && (l_uc_Status == ILV_OK))
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("----------------------------");
            badgelog->debug("MSO_GetDescriptor:");
            badgelog->debug("-->Product:\t%s",l_auc_DescProduct);
            badgelog->debug("-->Sensor:\t%s",l_auc_DescSensor);
            badgelog->debug("-->Software:\t%s",l_auc_DescSoftware);
        }
    }
    else
    {
        if (badgelog->isDebugEnabled())
        {
            badgelog->debug("Error MSO_GetDescriptor");
            MORPHO_ConvertError(l_uc_Status);
        }
    }

}


