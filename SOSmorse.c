//word.......word
//w...o...r...d.......w...o...r...d
//w -> _.. with brakes -> _....
//o -> _ _ _ with brakes -> _._._
// _ -> ...
// . -> time 1/10 s -> waitDelay(10)
// *********************************************************************
// *                            SOS program                            *
// *                           IQRF PLATFORM                           *
// *********************************************************************
//
// This is intended for seamless link-up of user application and IQRF
// operating system.
//
// It should be used for any user IQRF application with:
//     HW: IQRF transceiver modules
//     OS: 4.04
//
// ---------------------------------------------------------------------
// Online IQRF OS Reference Guide: http://www.iqrf.org/IQRF-OS-Reference-guide/
//
// Copyright (c) IQRF Tech s.r.o.
//
// File:    Template.c
// Version: v2.00                                   Revision: 03/03/2017
//
// Revision history:
//   v1.00: 18/05/2021  Modified for OS 4.04D.
//
// *********************************************************************
#include "C:/Users/krzys/PK/IoT/IQRF_OS404_7xD/Development/include/IQRF_OS/IQRF.h"	// System header files
// *********************************************************************
void dot();
void undersc();
void signS();
void signO();
void APPLICATION(){
    pulseLEDG();
    while(1){
        waitDelay(70);
        
        signS();
        waitDelay(30);
        signO();
        waitDelay(30);
        signS();
    }
}
// *********************************************************************
void dot(){
    //turn on LEDR for 10ms
    setLEDR();
    waitDelay(10);
    stopLEDR();
}
void undersc(){
    setLEDR();
    waitDelay(30);
    stopLEDR();
}
void signS(){
    dot();
    waitDelay(10);
    dot();
    waitDelay(10);
    dot();
}
void signO(){
    undersc();
    waitDelay(10);
    undersc();
    waitDelay(10);
    undersc();
}