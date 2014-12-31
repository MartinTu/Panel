/*
 * tpm2_spcmd.h
 *
 *  Created on: 04.12.2014
 *      Author: keKs
 */

#ifndef TPM2_SPCMD_H_
#define TPM2_SPCMD_H_



#define SPCMD_DRAW          0x01
#define SPCMD__COLOR        0x01
#define SPCMD__SET_PIXEL    0x02
#define SPCMD__DRAW_LINE    0x03
#define SPCMD__DRAW_RECT    0x04
#define SPCMD__DRAW_CIRC    0x05

#define SPCMD_ANI_SET       0x03
//      SPCMD__ANI_SET_ANIMATION
#define SPCMD_ANI_MIXER     0x04
#define SPCMD_ANI_DELAY     0x05
#define SPCMD_SYSTEM_ADMIN  0xFE


#endif /*TPM2_SPCMD_H_*/