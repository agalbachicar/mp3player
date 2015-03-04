/*
 * PinNames.h
 *
 *  Created on: 19/06/2011
 *      Author: Alejandro
 */

#ifndef LPC_1768_PINNAMES_H_
#define LPC_1768_PINNAMES_H_

#include "PIN.h"

#if 1 // Solo para colapsar
// Variantes del P0_0:
#define _GPIO__0_0 			( PN_PORT(0) | PN_PIN(0)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _RD1__0_0			( PN_PORT(0) | PN_PIN(0)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _TxD3__0_0			( PN_PORT(0) | PN_PIN(0)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _SDA1__0_0			( PN_PORT(0) | PN_PIN(0)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_1:
#define _GPIO__0_1			( PN_PORT(0) | PN_PIN(1)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _TD1__0_1			( PN_PORT(0) | PN_PIN(1)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _RxD3__0_1			( PN_PORT(0) | PN_PIN(1)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _SCL1__0_1			( PN_PORT(0) | PN_PIN(1)  | PN_FUNC(3) | PN_MODO_PULL_UP )


// Variantes del P0_2:
#define  _GPIO__0_2 		( PN_PORT(0) | PN_PIN(2)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _TxD0__0_2			( PN_PORT(0) | PN_PIN(2)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _AD0_7__0_2			( PN_PORT(0) | PN_PIN(2)  | PN_FUNC(2) | PN_MODO_PULL_NONE )
// RSV

// Variantes del P0_3:
#define  _GPIO__0_3 		( PN_PORT(0) | PN_PIN(3)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _RxD0__0_3			( PN_PORT(0) | PN_PIN(3)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _AD0_6__0_3			( PN_PORT(0) | PN_PIN(3)  | PN_FUNC(2) | PN_MODO_PULL_NONE )
// RSV

// Variantes del P0_4:
#define      _GPIO__0_4		( PN_PORT(0) | PN_PIN(4)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2SRX_CLK__0_4		( PN_PORT(0) | PN_PIN(4)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define       _RD2__0_4		( PN_PORT(0) | PN_PIN(4)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define    _CAP2_0__0_4		( PN_PORT(0) | PN_PIN(4)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_5:
#define     _GPIO__0_5		( PN_PORT(0) | PN_PIN(5)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2SRX_WS__0_5		( PN_PORT(0) | PN_PIN(5)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define      _TD2__0_5		( PN_PORT(0) | PN_PIN(5)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _CAP2_1__0_5		( PN_PORT(0) | PN_PIN(5)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_6:
#define      _GPIO__0_6 	( PN_PORT(0) | PN_PIN(6)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2SRX_SDA__0_6 	( PN_PORT(0) | PN_PIN(6)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _SSEL1__0_6		( PN_PORT(0) | PN_PIN(6)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MATCH2_0__0_6 	( PN_PORT(0) | PN_PIN(6)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_7:
#define      _GPIO__0_7 	( PN_PORT(0) | PN_PIN(7)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2STX_CLK__0_7 	( PN_PORT(0) | PN_PIN(7)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _SCLK1__0_7		( PN_PORT(0) | PN_PIN(7)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MATCH2_1__0_7 	( PN_PORT(0) | PN_PIN(7)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_8:
#define     _GPIO__0_8 		( PN_PORT(0) | PN_PIN(8)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2STX_WS__0_8 		( PN_PORT(0) | PN_PIN(8)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define    _MISO1__0_8		( PN_PORT(0) | PN_PIN(8)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH2_2__0_8 		( PN_PORT(0) | PN_PIN(8)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_9:
#define _GPIO__0_9 			( PN_PORT(0) | PN_PIN(9)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _I2STX_SDA__0_9 	( PN_PORT(0) | PN_PIN(9)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MOSI1__0_9	 		( PN_PORT(0) | PN_PIN(9)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH2_3__0_9 		( PN_PORT(0) | PN_PIN(9)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_10:
#define     _GPIO__0_10		( PN_PORT(0) | PN_PIN(10) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _TxD2__0_10		( PN_PORT(0) | PN_PIN(10) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _SDA2__0_10		( PN_PORT(0) | PN_PIN(10) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH3_0__0_10		( PN_PORT(0) | PN_PIN(10) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_11:
#define     _GPIO__0_11		( PN_PORT(0) | PN_PIN(11) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _RxD2__0_11		( PN_PORT(0) | PN_PIN(11) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _SCL2__0_11		( PN_PORT(0) | PN_PIN(11) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH3_1__0_11		( PN_PORT(0) | PN_PIN(11) | PN_FUNC(3) | PN_MODO_PULL_UP )

// NO HAY PIN 0_12
// NO HAY PIN 0_13
// NO HAY PIN 0_14

// Variantes del P0_15:
#define  _GPIO__0_15 		( PN_PORT(0) | PN_PIN(15) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _TxD1__0_15	 	( PN_PORT(0) | PN_PIN(15) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _SCLK0__0_15 		( PN_PORT(0) | PN_PIN(15) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _SCLK__0_15	 	( PN_PORT(0) | PN_PIN(15) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_16:
#define  _GPIO__0_16 		( PN_PORT(0) | PN_PIN(16) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _RxD1__0_16	 	( PN_PORT(0) | PN_PIN(16) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _SSEL0__0_16 		( PN_PORT(0) | PN_PIN(16) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _SSEL__0_16	 	( PN_PORT(0) | PN_PIN(16) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_17:
#define  _GPIO__0_17 		( PN_PORT(0) | PN_PIN(17) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _CTS1__0_17 		( PN_PORT(0) | PN_PIN(17) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MISO0__0_17 		( PN_PORT(0) | PN_PIN(17) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MISO__0_17	 	( PN_PORT(0) | PN_PIN(17) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_18:
#define  _GPIO__0_18 		( PN_PORT(0) | PN_PIN(18) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _DCD1__0_18	 	( PN_PORT(0) | PN_PIN(18) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MOSI0__0_18 		( PN_PORT(0) | PN_PIN(18) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MOSI__0_18	 	( PN_PORT(0) | PN_PIN(18) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_19:
#define _GPIO__0_19			( PN_PORT(0) | PN_PIN(19) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _DSR1__0_19			( PN_PORT(0) | PN_PIN(19) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _SDA1__0_19			( PN_PORT(0) | PN_PIN(19) | PN_FUNC(3) | PN_MODO_PULL_UP )

//-----------------------------------------------------------------------------

// Variantes del P0_20:
#define _GPIO__0_20			( PN_PORT(0) | PN_PIN(20) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _DTR1__0_20			( PN_PORT(0) | PN_PIN(20) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _SCL1__0_20			( PN_PORT(0) | PN_PIN(20) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_21:
#define _GPIO__0_21			( PN_PORT(0) | PN_PIN(21) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _RI1__0_21			( PN_PORT(0) | PN_PIN(21) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _RD1__0_21			( PN_PORT(0) | PN_PIN(21) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_22:
#define _GPIO__0_22			( PN_PORT(0) | PN_PIN(22) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _RTS1__0_22			( PN_PORT(0) | PN_PIN(22) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _TD1__0_22			( PN_PORT(0) | PN_PIN(22) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_23:
#define      _GPIO__0_23	( PN_PORT(0) | PN_PIN(23) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _AD0_0__0_23	( PN_PORT(0) | PN_PIN(23) | PN_FUNC(1) | PN_MODO_PULL_NONE )
#define _I2SRX_CLK__0_23	( PN_PORT(0) | PN_PIN(23) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define    _CAP3_0__0_23	( PN_PORT(0) | PN_PIN(23) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_24:
#define     _GPIO__0_24		( PN_PORT(0) | PN_PIN(24) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _AD0_1__0_24		( PN_PORT(0) | PN_PIN(24) | PN_FUNC(1) | PN_MODO_PULL_NONE )
#define _I2SRX_WS__0_24		( PN_PORT(0) | PN_PIN(24) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _CAP3_1__0_24		( PN_PORT(0) | PN_PIN(24) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_25:
#define      _GPIO__0_25	( PN_PORT(0) | PN_PIN(25) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _AD0_2__0_25	( PN_PORT(0) | PN_PIN(25) | PN_FUNC(1) | PN_MODO_PULL_NONE )
#define _I2SRX_SDA__0_25	( PN_PORT(0) | PN_PIN(25) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define      _TxD3__0_25	( PN_PORT(0) | PN_PIN(25) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_26:
#define      _GPIO__0_26	( PN_PORT(0) | PN_PIN(26) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _AD0_3__0_26	( PN_PORT(0) | PN_PIN(26) | PN_FUNC(1) | PN_MODO_PULL_NONE )
#define      _AOUT__0_26	( PN_PORT(0) | PN_PIN(26) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define      _RxD3__0_26	( PN_PORT(0) | PN_PIN(26) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P0_27:
#define    _GPIO__0_27		( PN_PORT(0) | PN_PIN(27) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _SDA0__0_27		( PN_PORT(0) | PN_PIN(27) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _USB_SDA__0_27		( PN_PORT(0) | PN_PIN(27) | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P0_28:
#define    _GPIO__0_28		( PN_PORT(0) | PN_PIN(28) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _SCL0__0_28		( PN_PORT(0) | PN_PIN(28) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _USB_SCL__0_28		( PN_PORT(0) | PN_PIN(28) | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P0_29:
#define   _GPIO__0_29		( PN_PORT(0) | PN_PIN(29) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _USB_Dp__0_29		( PN_PORT(0) | PN_PIN(29) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P0_30:
#define   _GPIO__0_30		( PN_PORT(0) | PN_PIN(30) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _USB_Dn__0_30		( PN_PORT(0) | PN_PIN(30) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// NO HAY PIN 0_31

//-----------------------------------------------------------------------------

// Variantes del P1_0:
#define      _GPIO__1_0		( PN_PORT(1) | PN_PIN(0)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_TxD0__1_0		( PN_PORT(1) | PN_PIN(0)  | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_1:
#define      _GPIO__1_1		( PN_PORT(1) | PN_PIN(1)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_TxD1__1_1		( PN_PORT(1) | PN_PIN(1)  | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// NO HAY PIN 1_2
// NO HAY PIN 1_3

// Variantes del P1_4:
#define        _GPIO__1_4 	( PN_PORT(1) | PN_PIN(4)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_TxD_EN__1_4 	( PN_PORT(1) | PN_PIN(4)  | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_8:
#define     _GPIO__1_8		( PN_PORT(1) | PN_PIN(8)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_CRS__1_8		( PN_PORT(1) | PN_PIN(8)  | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_9:
#define      _GPIO__1_9		( PN_PORT(1) | PN_PIN(9)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_RxD0__1_9		( PN_PORT(1) | PN_PIN(9)  | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_10:
#define      _GPIO__1_10	( PN_PORT(1) | PN_PIN(10) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_RxD1__1_10 	( PN_PORT(1) | PN_PIN(10) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

//OK------------------------------------------------------------------------------

// Variantes del P1_14:
#define       _GPIO__1_14 	( PN_PORT(1) | PN_PIN(14) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_Rx_ER__1_14 	( PN_PORT(1) | PN_PIN(14) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_15:
#define         _GPIO__1_15 ( PN_PORT(1) | PN_PIN(15) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_REF_CLK__1_15 ( PN_PORT(1) | PN_PIN(15) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_16:
#define     _GPIO__1_16		( PN_PORT(1) | PN_PIN(16) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_MDC__1_16		( PN_PORT(1) | PN_PIN(16) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_17:
#define      _GPIO__1_17	( PN_PORT(1) | PN_PIN(17) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _ENET_MDIO__1_17	( PN_PORT(1) | PN_PIN(17) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
// RSV

// Variantes del P1_18:
#define       _GPIO__1_18 	( PN_PORT(1) | PN_PIN(18) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _USB_UP_LED__1_18 	( PN_PORT(1) | PN_PIN(18) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _PWM1_1__1_18 	( PN_PORT(1) | PN_PIN(18) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define     _CAP1_0__1_18	( PN_PORT(1) | PN_PIN(18) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_19:
#define     _GPIO__1_19		( PN_PORT(1) | PN_PIN(19) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _MCOA0__1_19		( PN_PORT(1) | PN_PIN(19) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _USB_PPWR__1_19		( PN_PORT(1) | PN_PIN(19) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _CAP1_1__1_19		( PN_PORT(1) | PN_PIN(19) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_20:
#define   _GPIO__1_20		( PN_PORT(1) | PN_PIN(20) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define   _MCI0__1_20		( PN_PORT(1) | PN_PIN(20) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _PWM1_2__1_20		( PN_PORT(1) | PN_PIN(20) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _SCLK0__1_20		( PN_PORT(1) | PN_PIN(20) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_21:
#define    _GPIO__1_21		( PN_PORT(1) | PN_PIN(21) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _MCABORT__1_21		( PN_PORT(1) | PN_PIN(21) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define  _PWM1_3__1_21		( PN_PORT(1) | PN_PIN(21) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _SSEL0__1_21		( PN_PORT(1) | PN_PIN(21) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_22:
#define     _GPIO__1_22		( PN_PORT(1) | PN_PIN(22) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _MCOB0__1_22		( PN_PORT(1) | PN_PIN(22) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _USB_PWRD__1_22		( PN_PORT(1) | PN_PIN(22) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH1_0__1_22		( PN_PORT(1) | PN_PIN(22) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_23:
#define   _GPIO__1_23		( PN_PORT(1) | PN_PIN(23) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define   _MCI1__1_23		( PN_PORT(1) | PN_PIN(23) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _PWM1_4__1_23		( PN_PORT(1) | PN_PIN(23) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MISO0__1_23		( PN_PORT(1) | PN_PIN(23) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_24:
#define   _GPIO__1_24		( PN_PORT(1) | PN_PIN(24) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define   _MCI2__1_24		( PN_PORT(1) | PN_PIN(24) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _PWM1_5__1_24		( PN_PORT(1) | PN_PIN(24) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define  _MOSI0__1_24		( PN_PORT(1) | PN_PIN(24) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_25:
#define     _GPIO__1_25		( PN_PORT(1) | PN_PIN(25) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _MCOA1__1_25		( PN_PORT(1) | PN_PIN(25) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _MATCH1_1__1_25		( PN_PORT(1) | PN_PIN(25) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_26:
#define   _GPIO__1_26		( PN_PORT(1) | PN_PIN(26) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _MCOB1__1_26		( PN_PORT(1) | PN_PIN(26) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _PWM1_6__1_26		( PN_PORT(1) | PN_PIN(26) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _CAP0_0__1_26		( PN_PORT(1) | PN_PIN(26) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_27:
#define      _GPIO__1_27	( PN_PORT(1) | PN_PIN(27) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _CLKOUT__1_27	( PN_PORT(1) | PN_PIN(27) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _USB_OVRCR__1_27	( PN_PORT(1) | PN_PIN(27) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define    _CAP0_1__1_27	( PN_PORT(1) | PN_PIN(27) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_28:
#define     _GPIO__1_28		( PN_PORT(1) | PN_PIN(28) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _MCOA2__1_28		( PN_PORT(1) | PN_PIN(28) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define  _PCAP1_0__1_28		( PN_PORT(1) | PN_PIN(28) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH0_0__1_28		( PN_PORT(1) | PN_PIN(28) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_29:
#define     _GPIO__1_29		( PN_PORT(1) | PN_PIN(29) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _MCOB2__1_29		( PN_PORT(1) | PN_PIN(29) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define  _PCAP1_1__1_29		( PN_PORT(1) | PN_PIN(29) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _MATCH0_1__1_29		( PN_PORT(1) | PN_PIN(29) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P1_30:
#define  _GPIO__1_30		( PN_PORT(1) | PN_PIN(30) | PN_FUNC(0) | PN_MODO_PULL_UP )
// RSV
#define  _VBUS__1_30		( PN_PORT(1) | PN_PIN(30) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _AD0_4__1_30		( PN_PORT(1) | PN_PIN(30) | PN_FUNC(3) | PN_MODO_PULL_NONE )

// Variantes del P1_31:
#define  _GPIO__1_31		( PN_PORT(1) | PN_PIN(31) | PN_FUNC(0) | PN_MODO_PULL_UP )
// RSV
#define _SCLK1__1_31		( PN_PORT(1) | PN_PIN(31) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _AD0_5__1_31		( PN_PORT(1) | PN_PIN(31) | PN_FUNC(3) | PN_MODO_PULL_NONE )

//-----------------------------------------------------------------------------

// Variantes del P2_0:
#define   _GPIO__2_0		( PN_PORT(2) | PN_PIN(0)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_1__2_0		( PN_PORT(2) | PN_PIN(0)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _TxD1__2_0		( PN_PORT(2) | PN_PIN(0)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_1:
#define   _GPIO__2_1		( PN_PORT(2) | PN_PIN(1)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_2__2_1		( PN_PORT(2) | PN_PIN(1)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _RxD1__2_1		( PN_PORT(2) | PN_PIN(1)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_2:
#define   _GPIO__2_2	 	( PN_PORT(2) | PN_PIN(2)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_3__2_2 		( PN_PORT(2) | PN_PIN(2)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _CTS1__2_2	 	( PN_PORT(2) | PN_PIN(2)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_3:
#define   _GPIO__2_3		( PN_PORT(2) | PN_PIN(3)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_4__2_3		( PN_PORT(2) | PN_PIN(3)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _DCD1__2_3		( PN_PORT(2) | PN_PIN(3)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_4:
#define   _GPIO__2_4		( PN_PORT(2) | PN_PIN(4)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_5__2_4		( PN_PORT(2) | PN_PIN(4)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _DSR1__2_4		( PN_PORT(2) | PN_PIN(4)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_5:
#define   _GPIO__2_5		( PN_PORT(2) | PN_PIN(5)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _PWM1_6__2_5		( PN_PORT(2) | PN_PIN(5)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _DTR1__2_5		( PN_PORT(2) | PN_PIN(5)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_6:
#define     _GPIO__2_6		( PN_PORT(2) | PN_PIN(6)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _PCAP1_0__2_6		( PN_PORT(2) | PN_PIN(6)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define      _RI1__2_6		( PN_PORT(2) | PN_PIN(6)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_7:
#define _GPIO__2_7			( PN_PORT(2) | PN_PIN(7)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _RD2__2_7			( PN_PORT(2) | PN_PIN(7)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _RTS1__2_7			( PN_PORT(2) | PN_PIN(7)  | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_8:
#define     _GPIO__2_8		( PN_PORT(2) | PN_PIN(8)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define      _TD2__2_8		( PN_PORT(2) | PN_PIN(8)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define     _TxD2__2_8		( PN_PORT(2) | PN_PIN(8)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define _ENET_MDC__2_8		( PN_PORT(2) | PN_PIN(8)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P2_9:
#define        _GPIO__2_9 	( PN_PORT(2) | PN_PIN(9)  | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _USB_CONNECT__2_9 	( PN_PORT(2) | PN_PIN(9)  | PN_FUNC(1) | PN_MODO_PULL_UP )
#define        _RxD2__2_9 	( PN_PORT(2) | PN_PIN(9)  | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _ENET_MDIO__2_9 	( PN_PORT(2) | PN_PIN(9)  | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P2_10:
#define  _GPIO__2_10		( PN_PORT(2) | PN_PIN(10) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define _EINT0__2_10		( PN_PORT(2) | PN_PIN(10) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define   _NMI__2_10		( PN_PORT(2) | PN_PIN(10) | PN_FUNC(2) | PN_MODO_PULL_UP )
// RSV

// Variantes del P2_11:
#define      _GPIO__2_11	( PN_PORT(2) | PN_PIN(11) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _EINT1__2_11	( PN_PORT(2) | PN_PIN(11) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _I2STX_CLK__2_11	( PN_PORT(2) | PN_PIN(11) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P2_12:
#define     _GPIO__2_12		( PN_PORT(2) | PN_PIN(12) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _EINT2__2_12		( PN_PORT(2) | PN_PIN(12) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _I2STX_WS__2_12		( PN_PORT(2) | PN_PIN(12) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P2_13:
#define      _GPIO__2_13	( PN_PORT(2) | PN_PIN(13) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define     _EINT3__2_13	( PN_PORT(2) | PN_PIN(13) | PN_FUNC(1) | PN_MODO_PULL_UP )
// RSV
#define _I2STX_SDA__2_13	( PN_PORT(2) | PN_PIN(13) | PN_FUNC(3) | PN_MODO_PULL_UP )


//-----------------------------------------------------------------------------

// Variantes del P3_25:
#define     _GPIO__3_25		( PN_PORT(3) | PN_PIN(25) | PN_FUNC(0) | PN_MODO_PULL_UP )
//RESERVED
#define _MATCH0_0__3_25		( PN_PORT(3) | PN_PIN(25) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _PWM1_2__3_25		( PN_PORT(3) | PN_PIN(25) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P3_26:
#define     _GPIO__3_26		( PN_PORT(3) | PN_PIN(26) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define    _STCLK__3_26		( PN_PORT(3) | PN_PIN(26) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MATCH0_1__3_26		( PN_PORT(3) | PN_PIN(26) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define   _PWM1_3__3_26		( PN_PORT(3) | PN_PIN(26) | PN_FUNC(3) | PN_MODO_PULL_UP )

//-----------------------------------------------------------------------------

// Variantes del P4_28:
#define     _GPIO__4_28		( PN_PORT(4) | PN_PIN(28) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _RX_MCLK__4_28		( PN_PORT(4) | PN_PIN(28) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MATCH2_4__4_28		( PN_PORT(4) | PN_PIN(28) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define     _TxD3__4_28		( PN_PORT(4) | PN_PIN(28) | PN_FUNC(3) | PN_MODO_PULL_UP )

// Variantes del P4_29:
#define     _GPIO__4_29		( PN_PORT(4) | PN_PIN(29) | PN_FUNC(0) | PN_MODO_PULL_UP )
#define  _TX_MCLK__4_29		( PN_PORT(4) | PN_PIN(29) | PN_FUNC(1) | PN_MODO_PULL_UP )
#define _MATCH2_1__4_29		( PN_PORT(4) | PN_PIN(29) | PN_FUNC(2) | PN_MODO_PULL_UP )
#define     _RxD3__4_29		( PN_PORT(4) | PN_PIN(29) | PN_FUNC(3) | PN_MODO_PULL_UP )

#endif

//-----------------------------------------------------------------------------

// Nombres de los Pines, con sus atributos codificados
enum _pin_names {
	GPIO__0_0  	= _GPIO__0_0,	RD1__0_0 			= _RD1__0_0,			TxD3__0_0		= _TxD3__0_0,			SDA1__0_0 			= _SDA1__0_0,
	GPIO__0_1  	= _GPIO__0_1,	TD1__0_1 			= _TD1__0_1,			RxD3__0_1		= _RxD3__0_1,			SCL1__0_1 			= _SCL1__0_1,
	GPIO__0_2  	= _GPIO__0_2,	TxD0__0_2 			= _TxD0__0_2,			AD0_7__0_2		= _AD0_7__0_2,
	GPIO__0_3  	= _GPIO__0_3,	RxD0__0_3 			= _RxD0__0_3,			AD0_6__0_3		= _AD0_6__0_3,
	GPIO__0_4  	= _GPIO__0_4,	I2SRX_CLK__0_4 		= _I2SRX_CLK__0_4,		RD2__0_4		= _RD2__0_4,			CAP2_0__0_4			= _CAP2_0__0_4,
	GPIO__0_5  	= _GPIO__0_5,	I2SRX_WS__0_5 		= _I2SRX_WS__0_5,		TD2__0_5		= _TD2__0_5,			CAP2_1__0_5			= _CAP2_1__0_5,
	GPIO__0_6  	= _GPIO__0_6,	I2SRX_SDA__0_6 		= _I2SRX_SDA__0_6,		SSEL1__0_6		= _SSEL1__0_6, 			MATCH2_0__0_6		= _MATCH2_0__0_6,
	GPIO__0_7  	= _GPIO__0_7, 	I2STX_CLK__0_7 		= _I2STX_CLK__0_7,		SCLK1__0_7		= _SCLK1__0_7, 			MATCH2_1__0_7		= _MATCH2_1__0_7,
	GPIO__0_8  	= _GPIO__0_8, 	I2STX_WS__0_8  		= _I2STX_WS__0_8,		MISO1__0_8		= _MISO1__0_8, 			MATCH2_2__0_8		= _MATCH2_2__0_8,
	GPIO__0_9  	= _GPIO__0_9, 	I2STX_SDA__0_9 		= _I2STX_SDA__0_9,		MOSI1__0_9		= _MOSI1__0_9,			MATCH2_3__0_9		= _MATCH2_3__0_9,
	GPIO__0_10 	= _GPIO__0_10, 	TxD2__0_10 			= _TxD2__0_10,			SDA2__0_10 		= _SDA2__0_10,			MATCH3_0__0_10		= _MATCH3_0__0_10,
	GPIO__0_11 	= _GPIO__0_11, 	RxD2__0_11 			= _RxD2__0_11,			SCL2__0_11 		= _SCL2__0_11,			MATCH3_1__0_11		= _MATCH3_1__0_11,
	// NO HAY PIN 0_12
	// NO HAY PIN 0_13
	// NO HAY PIN 0_14
	GPIO__0_15 	= _GPIO__0_15, 	TxD1__0_15 			= _TxD1__0_15,			SCLK0__0_15 	= _SCLK0__0_15,			SCLK__0_15			= _SCLK__0_15,
	GPIO__0_16 	= _GPIO__0_16, 	RxD1__0_16			= _RxD1__0_16, 			SSEL0__0_16		= _SSEL0__0_16, 		SSEL__0_16			= _SSEL__0_16,
	GPIO__0_17 	= _GPIO__0_17, 	CTS1__0_17			= _CTS1__0_17,			MISO0__0_17		= _MISO0__0_17, 		MISO__0_17			= _MISO__0_17,
	GPIO__0_18 	= _GPIO__0_18, 	DCD1__0_18			= _DCD1__0_18, 			MOSI0__0_18		= _MOSI0__0_18, 		MOSI__0_18			= _MOSI__0_18,
	GPIO__0_19 	= _GPIO__0_19, 	DSR1__0_19			= _DSR1__0_19, 													SDA1__0_19			= _SDA1__0_19,
	GPIO__0_20 	= _GPIO__0_20, 	DTR1__0_20			= _DTR1__0_20, 													SCL1__0_20			= _SCL1__0_20,
	GPIO__0_21 	= _GPIO__0_21, 	RI1__0_21			= _RI1__0_21, 													RD1__0_21			= _RD1__0_21,
	GPIO__0_22 	= _GPIO__0_22, 	RTS1__0_22			= _RTS1__0_22, 													TD1__0_22			= _TD1__0_22,
	GPIO__0_23 	= _GPIO__0_23, 	AD0_0__0_23			= _AD0_0__0_23, 		I2SRX_CLK__0_23 = _I2SRX_CLK__0_23,		CAP3_0__0_23		= _CAP3_0__0_23,
	GPIO__0_24 	= _GPIO__0_24, 	AD0_1__0_24			= _AD0_1__0_24, 		I2SRX_WS__0_24 	= _I2SRX_WS__0_24,		CAP3_1__0_24		= _CAP3_1__0_24,
	GPIO__0_25 	= _GPIO__0_25, 	AD0_2__0_25			= _AD0_2__0_25, 		I2SRX_SDA__0_25 = _I2SRX_SDA__0_25,		TxD3__0_25			= _TxD3__0_25,
	GPIO__0_26 	= _GPIO__0_26, 	AD0_3__0_26			= _AD0_3__0_26, 		AOUT__0_26		= _AOUT__0_26,			RxD3__0_26			= _RxD3__0_26,
	GPIO__0_27 	= _GPIO__0_27, 	SDA0__0_27			= _SDA0__0_27, 			USB_SDA__0_27 	= _USB_SDA__0_27,
	GPIO__0_28 	= _GPIO__0_28, 	SCL0__0_28			= _SCL0__0_28, 			USB_SCL__0_28 	= _USB_SCL__0_28,
	GPIO__0_29 	= _GPIO__0_29, 	USB_Dp__0_29		= _USB_Dp__0_29,
	GPIO__0_30 	= _GPIO__0_30, 	USB_Dn__0_30		= _USB_Dn__0_30,
	// NO HAY PIN 0_31

	GPIO__1_0 	= _GPIO__1_0, 	ENET_TxD0__1_0		= _ENET_TxD0__1_0,
	GPIO__1_1 	= _GPIO__1_1, 	ENET_TxD1__1_1		= _ENET_TxD1__1_1,
	// NO HAY PIN 1_2
	// NO HAY PIN 1_3
	GPIO__1_4 	= _GPIO__1_4, 	ENET_TxD_EN__1_4	= _ENET_TxD_EN__1_4,
	// NO HAY PIN 1_5
	// NO HAY PIN 1_6
	// NO HAY PIN 1_7
	GPIO__1_8 	= _GPIO__1_8, 	ENET_CRS__1_8		= _ENET_CRS__1_8,
	GPIO__1_9 	= _GPIO__1_9, 	ENET_RxD0__1_9		= _ENET_RxD0__1_9,
	GPIO__1_10 	= _GPIO__1_10, 	ENET_RxD1__1_10		= _ENET_RxD1__1_10,
	// NO HAY PIN 1_11
	// NO HAY PIN 1_12
	// NO HAY PIN 1_13
	GPIO__1_14 	= _GPIO__1_14, 	ENET_Rx_ER__1_14	= _ENET_Rx_ER__1_14,
	GPIO__1_15 	= _GPIO__1_15, 	ENET_REF_CLK__1_15	= _ENET_REF_CLK__1_15,
	GPIO__1_16 	= _GPIO__1_16, 	ENET_MDC__1_16		= _ENET_MDC__1_16,
	GPIO__1_17 	= _GPIO__1_17, 	ENET_MDIO__1_17		= _ENET_MDIO__1_17,
	GPIO__1_18 	= _GPIO__1_18, 	USB_UP_LED__1_18 	= _USB_UP_LED__1_18,	PWM1_1__1_18 	= _PWM1_1__1_18, 		CAP1_0__1_18 	= _CAP1_0__1_18,
	GPIO__1_19 	= _GPIO__1_19, 	MCOA0__1_19 		= _MCOA0__1_19,			USB_PPWR__1_19 	= _USB_PPWR__1_19, 		CAP1_1__1_19 	= _CAP1_1__1_19,
	GPIO__1_20 	= _GPIO__1_20, 	MCI0__1_20 			= _MCI0__1_20, 			PWM1_2__1_20 	= _PWM1_2__1_20,		SCLK0__1_20 	= _SCLK0__1_20,
	GPIO__1_21 	= _GPIO__1_21, 	MCABORT__1_21 		= _MCABORT__1_21,		PWM1_3__1_21 	= _PWM1_3__1_21,		SSEL_0__1_21 	= _SSEL0__1_21,
	GPIO__1_22 	= _GPIO__1_22, 	MCOB0__1_22 		= _MCOB0__1_22,			USB_PWRD__1_22 	= _USB_PWRD__1_22,		MATCH1_0__1_22 	= _MATCH1_0__1_22,
	GPIO__1_23 	= _GPIO__1_23,  MCI1__1_23 			= _MCI1__1_23, 			PWM1_4__1_23 	= _PWM1_4__1_23,		MISO0__1_23 	= _MISO0__1_23,
	GPIO__1_24 	= _GPIO__1_24,  MCI2__1_24 			= _MCI2__1_24, 			PWM1_5__1_24 	= _PWM1_5__1_24,		MOSI0__1_24 	= _MOSI0__1_24,
	GPIO__1_25 	= _GPIO__1_25,  MCOA1__1_25 		= _MCOA1__1_25, 												MATCH1_1__1_25 	= _MATCH1_1__1_25,
	GPIO__1_26 	= _GPIO__1_26,  MCOB1__1_26 		= _MCOB1__1_26, 		PWM1_6__1_26 	= _PWM1_6__1_26,		CAP0_0__1_26 	= _CAP0_0__1_26,
	GPIO__1_27 	= _GPIO__1_27,  CLKOUT__1_27 		= _CLKOUT__1_27, 		USB_OVRCR__1_27 = _USB_OVRCR__1_27,		CAP0_1__1_27 	= _CAP0_1__1_27,
	GPIO__1_28 	= _GPIO__1_28,  MCOA2__1_28 		= _MCOA2__1_28, 		PCAP1_0__1_28 	= _PCAP1_0__1_28,		MATCH0_0__1_28 	= _MATCH0_0__1_28,
	GPIO__1_29 	= _GPIO__1_29,  MCOB2__1_29 		= _MCOB2__1_29, 		PCAP1_1__1_29 	= _PCAP1_1__1_29,		MATCH0_1__1_29 	= _MATCH0_1__1_29,
	GPIO__1_30 	= _GPIO__1_30,  											VBUS__1_30 		= _VBUS__1_30,			AD0_4__1_30 	= _AD0_4__1_30,
	GPIO__1_31 	= _GPIO__1_31,  											SCLK1__1_31 	= _SCLK1__1_31,			AD0_5__1_31 	= _AD0_5__1_31,

	GPIO__2_0	= _GPIO__2_0,	PWM1_1__2_0			= _PWM1_1__2_0,			TxD1__2_0		= _TxD1__2_0,
	GPIO__2_1	= _GPIO__2_1,	PWM1_2__2_1			= _PWM1_2__2_1,			RxD1__2_1		= _RxD1__2_1,
	GPIO__2_2	= _GPIO__2_2,	PWM1_3__2_2			= _PWM1_3__2_2,			CTS1__2_2		= _CTS1__2_2,
	GPIO__2_3	= _GPIO__2_3,	PWM1_4__2_3			= _PWM1_4__2_3,			DCD1__2_3		= _DCD1__2_3,
	GPIO__2_4	= _GPIO__2_4,	PWM1_5__2_4			= _PWM1_5__2_4,			DSR1__2_4		= _DSR1__2_4,
	GPIO__2_5	= _GPIO__2_5,	PWM1_6__2_5			= _PWM1_6__2_5,			DTR1__2_5		= _DTR1__2_5,
	GPIO__2_6	= _GPIO__2_6,	PCAP1_0__2_6		= _PCAP1_0__2_6,		RI1__2_6		= _RI1__2_6,
	GPIO__2_7	= _GPIO__2_7,	RD2__2_7			= _RD2__2_7,			RTS1__2_7		= _RTS1__2_7,
	GPIO__2_8	= _GPIO__2_8,	TD2__2_8			= _TD2__2_8,			TxD2__2_8		= _TxD2__2_8,			ENET_MDC__2_8	= _ENET_MDC__2_8,
	GPIO__2_9	= _GPIO__2_9,	USB_CONNECT__2_9 	= _USB_CONNECT__2_9,	RxD2__2_9		= _RxD2__2_9,			ENET_MDIO__2_9	= _ENET_MDIO__2_9,
	GPIO__2_10	= _GPIO__2_10,	EINT0__2_10			= _EINT0__2_10,			NMI__2_10		= _NMI__2_10,
	GPIO__2_11	= _GPIO__2_11,	EINT1__2_11			= _EINT1__2_11,													I2STX_CLK__2_11	= _I2STX_CLK__2_11,
	GPIO__2_12	= _GPIO__2_12,	EINT2__2_12			= _EINT2__2_12,													I2STX_WS__2_12	= _I2STX_WS__2_12,
	GPIO__2_13	= _GPIO__2_13,	EINT3__2_13			= _EINT3__2_13,													I2STX_SDA__2_13	= _I2STX_SDA__2_13,

	GPIO__3_25	= _GPIO__3_25,												MATCH0_0__3_25	= _MATCH0_0__3_25,		PWM1_2__3_25	= _PWM1_2__3_25,
	GPIO__3_26	= _GPIO__3_26,	STCLK__3_26			= _STCLK__3_26,			MATCH0_1__3_26	= _MATCH0_1__3_26,		PWM1_3__3_26	= _PWM1_3__3_26,

	GPIO__4_28	= _GPIO__4_28,	RX_MCLK__4_28		= _RX_MCLK__4_28,		MATCH2_4__4_28	= _MATCH2_4__4_28,		TxD3__4_28		= _TxD3__4_28,
	GPIO__4_29	= _GPIO__4_29,	TX_MCLK__4_29		= _TX_MCLK__4_29,		MATCH2_1__4_29	= _MATCH2_1__4_29,		RxD3__4_29		= _RxD3__4_29,

//Pines del MBED

	MBED_LED1 = GPIO__1_18,
	MBED_LED2 = GPIO__1_20,
	MBED_LED3 = GPIO__1_21,
	MBED_LED4 = GPIO__1_23,

	MBED_PWM_LED1 = PWM1_1__1_18,
	MBED_PWM_LED2 = PWM1_2__1_20,
	MBED_PWM_LED3 = PWM1_3__1_21,
	MBED_PWM_LED4 = PWM1_4__1_23,

//Pines de la EA_BASEBOARD

	EABASE_LED_ROJO		= GPIO__2_0,
	EABASE_LED_VERDE	= GPIO__2_1,
	EABASE_LED_AZUL		= GPIO__0_26,

	EABASE_SW4			= GPIO__1_31,
	EABASE_SW3_REV_A	= GPIO__0_4,
	EABASE_SW3_REV_B	= GPIO__2_10,

//Pines de la RubenBoard

	RUBENBOARD_SW2		= GPIO__0_18,

	RUBENBOARD_POTE		= AD0_5__1_31,

	RUBENBOARD_D4		= GPIO__2_12,	// LED7
	RUBENBOARD_D5		= GPIO__2_11,	// LED6
	RUBENBOARD_D6		= GPIO__0_4,	// LED5
	RUBENBOARD_D7		= GPIO__2_4,	// LED4
	RUBENBOARD_D8		= GPIO__2_3,	// LED3
	RUBENBOARD_D9		= GPIO__2_2,	// LED2
	RUBENBOARD_D10		= GPIO__0_22,	// LED1
	RUBENBOARD_D11		= GPIO__0_17,	// LED0

	RUBENBOARD_RGB_R	= RUBENBOARD_D9,
	RUBENBOARD_RGB_G	= RUBENBOARD_D8,
	RUBENBOARD_RGB_B	= RUBENBOARD_D7,

	RUBENBOARD_LCD_D4	= GPIO__2_5,
	RUBENBOARD_LCD_D5	= GPIO__2_6,
	RUBENBOARD_LCD_D6	= GPIO__2_7,
	RUBENBOARD_LCD_D7	= GPIO__2_8,
	RUBENBOARD_LCD_RS	= GPIO__2_0,
	RUBENBOARD_LCD_EN	= GPIO__2_1,

//Led del Stick LPCXpresso

	STICK_LED_ROJO 		= GPIO__0_22

};

typedef enum _pin_names PinName;

#endif /* LPC_1768_PINNAMES_H_ */
