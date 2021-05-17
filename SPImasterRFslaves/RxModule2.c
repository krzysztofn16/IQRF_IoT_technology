// *********************************************************************
#include "C:/Users/krzys/PK/IoT/IQRF_OS404_7xD/Development/include/IQRF_OS/IQRF.h"	// System header files
// *********************************************************************
void APPLICATION()                  // Obligatory assigning
{
    uns8 i,tenths,level;
    uns16 temperature,tmp,voltage;
    toutRF=10;
    pulseLEDG();
    setOnPulsingLED(10);            // LED flash time setting
    while (1)                       // Main cycle (perpetually repeated)
    {
        if (RFRXpacket())           // If anything was received
        {
            copyBufferRF2COM();
            switch (bufferCOM[0])
            {
                case '1':
                    for(i=0;i<10;i++){
                        pulseLEDG();
                        waitDelay(100); //1 sek pause between LED flash
                    }
                    PIN=0;
                    bufferRF[0]='M';
                    bufferRF[1]='2';
                    bufferRF[2]=' ';
                    bufferRF[3]='T';
                    bufferRF[4]='a';
                    bufferRF[5]='s';
                    bufferRF[6]='k';
                    bufferRF[7]='1';
                    DLEN=8;
                    waitDelay(100);
                    RFTXpacket();
                    pulseLEDR();
                    break;
                case '2':
                    for(i=0;i<10;i++){
                        pulseLEDR();
                        waitDelay(100);
                    }
                    PIN=0;
                    bufferRF[0]='M';
                    bufferRF[1]='2';
                    bufferRF[2]=' ';
                    bufferRF[3]='T';
                    bufferRF[4]='a';
                    bufferRF[5]='s';
                    bufferRF[6]='k';
                    bufferRF[7]='2';
                    DLEN=8;
                    waitDelay(100);
                    RFTXpacket();
                    pulseLEDG();
                    break;
                case '3':                     	// replacing lighting of the LEDs
                    for (i = 0; i < 10; i++){
                        if(i%2==0){
                            pulseLEDG();
                            waitDelay(100);
                        }else{
                            pulseLEDR();
                            waitDelay(100);
                        }
                    }
                    PIN=0;
                    bufferRF[0]='M';
                    bufferRF[1]='2';
                    bufferRF[2]=' ';
                    bufferRF[3]='T';
                    bufferRF[4]='a';
                    bufferRF[5]='s';
                    bufferRF[6]='k';
                    bufferRF[7]='3';
                    DLEN=8;
                    waitDelay(100);
                    RFTXpacket();
                    pulseLEDR();
                    break;			
                case 't':
                    eEEPROM_TempSensorOn();
                    waitDelay(30);
                    getTemperature();	
                    eEEPROM_TempSensorOff();
                    temperature = param3;
                    if(temperature!=128){
                        PIN=0;
                        bufferRF[0]='M';
                        bufferRF[1]='2';
                        bufferRF[2]=' ';
                        bufferRF[3]='T';
                        bufferRF[4]='=';
                        bufferRF[5]=' ';
                        bufferRF[8]='.';
                        bufferRF[10]=0xB0;
                        bufferRF[11]='C';
                        bufferRF[12]='.';
                        if(temperature.high8 & 0x08){
                            temperature =~temperature;
                            temperature++;
                            bufferRF[5] = '-';
                        }
                    }
                    tenths=temperature.high8 & 0x0F;
                    temperature>>=4;

                    i=temperature.low8/10;
                    bufferRF[6] = i+'0';
                    i=temperature.low8%10;
                    bufferRF[7] = i+'0';

                    tmp=tenths*625;
                    i=tmp/1000;
                    tmp%=1000;
                    if((tmp>=500)&&(i<9))
                        i++;
                    bufferRF[9] = i+'0';
                    DLEN=13;
                    waitDelay(100);
                    RFTXpacket();
                    pulseLEDR();
                    break;		
                case 'v':
                    PIN=0;
                    bufferRF[0]='M';
                    bufferRF[1]='2';
                    bufferRF[2]=' ';
                    bufferRF[3] = 'V';
                    bufferRF[4] = '=';
                    bufferRF[6] = '.';
                    bufferRF[9] = 'V';
                    bufferRF[10] = '.';
                    level = getSupplyVoltage();
                    tmp = 127-level;
                    voltage = 261/(uns8)tmp;
                    i = (uns8)voltage%10;
                    bufferRF[5] = i+'0';
                    i = (uns8)voltage*10;
                    i = i%10;
                    bufferRF[7] = i+'0';
                    i=(uns8)voltage*100;
                    i = i%10;
                    bufferRF[8] = i+'0';
                    DLEN=11;
                    waitDelay(100);
                    RFTXpacket();
                    pulseLEDR();
                    break;
                default:
                    stopLEDR();       	// Long flash LED (1 s)
                    _LEDR = 1;
                    waitDelay(100);
                    _LEDR = 0;
                    break;
            }
        }
    }
}

// *********************************************************************