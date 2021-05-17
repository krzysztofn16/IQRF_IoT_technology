// *********************************************************************
#include "C:/Users/krzys/PK/IoT/IQRF_OS404_7xD/Development/include/IQRF_OS/IQRF.h"	// System header files

// *********************************************************************
void APPLICATION()
{
uns8 i,tenths,level;
uns16 temperature,tmp,voltage;
uns8 num_mods = 2;
toutRF = 10;

    enableSPI();                    // Enable SPI Rx/Tx on background

    appInfo();                      // Copy menu from EEPROM
    copyBufferINFO2COM();           // (from Appl. info) to bufferCOM
    startSPI(21);                   // and then try to send it via SPI
    setOnPulsingLED(10);            // LED flash time setting
    while (1)
    {
        if (getStatusSPI())         // Update SPIstatus, check SPI busy
                                    //   SPIpacketLength: data length
                                    //   param2: SPI status
            continue;               // Wait until message is picked up

        if (_SPIRX)                 // Anything received?
        {                           // Yes:
            if (!_SPICRCok)         //   CRCM matched?
            {                       //     No:
                startSPI(0);        //       Restart SPI communication
                continue;           //       and continue
            }

            waitDelay(35);          // Wait 350 ms before indication
            clrwdt();

            switch (bufferCOM[0])   // Check what is in bufferCOM
            {								
                case '1':                     	// 10x LED Green flashes
                            PIN=0;
                            copyBufferCOM2RF();
                            DLEN=1;
                            RFTXpacket();
                            for(i=0;i<10;i++){
                                pulseLEDG();
                                waitDelay(100); //1 sek pause between LED flash
                            }  
                            bufferCOM[0] = 'D';
                            bufferCOM[1] = 'o';
                            bufferCOM[2] = 'n';
                            bufferCOM[3] = 'e';
                            bufferCOM[4] = '.';
                            startSPI(5);      	// Sent data through SPI
                            break;
				case '2':                     	// 10x LED Red flashes
                            PIN=0;
                            copyBufferCOM2RF();
                            DLEN=1;
                            RFTXpacket();
                            for(i=0;i<10;i++){
                                pulseLEDR();
                                waitDelay(100);
                            }  
                            bufferCOM[0] = 'D';
                            bufferCOM[1] = 'o';
                            bufferCOM[2] = 'n';
                            bufferCOM[3] = 'e';
                            bufferCOM[4] = '.';
                            startSPI(5);      	// Sent data through SPI
                            break;
                case '3':                     	// replacing lighting of the LEDs
                            PIN=0;
                            copyBufferCOM2RF();
                            DLEN=1;
                            RFTXpacket();
                            for (i = 0; i < 10; i++){
                                if(i%2==0){
                                    pulseLEDG();
                                    waitDelay(100);
                                }else{
                                    pulseLEDR();
                                    waitDelay(100);
                                }
                            }
                            bufferCOM[0] = 'D';
                            bufferCOM[1] = 'o';
                            bufferCOM[2] = 'n';
                            bufferCOM[3] = 'e';
                            bufferCOM[4] = '.';
                            startSPI(5);        // Sent data through SPI
                            break;			
                case 't':
                            PIN=0;
                            copyBufferCOM2RF();
                            DLEN=1;
                            RFTXpacket();
                            eEEPROM_TempSensorOn();
                            waitDelay(30);
                            getTemperature();	
                            eEEPROM_TempSensorOff();
                            temperature = param3;
                            if(temperature!=128){
                                bufferCOM[0] = 'T';
                                bufferCOM[1] = '=';
                                bufferCOM[2] = ' ';
                                bufferCOM[5] = '.';
                                bufferCOM[7] = 0xB0;
                                bufferCOM[8] = 'C';
                                bufferCOM[9] = '.';
                                if(temperature.high8 & 0x08){
                                    temperature =~temperature;
                                    temperature++;
                                    bufferCOM[2] = '-';
                                }
                            }
                            tenths=temperature.high8 & 0x0F;
                            temperature>>=4;

                            i=temperature.low8/10;
                            bufferCOM[3] = i+'0';
                            i=temperature.low8%10;
                            bufferCOM[4] = i+'0';

                            tmp=tenths*625;
                            i=tmp/1000;
                            tmp%=1000;
                            if((tmp>=500)&&(i<9))
                                i++;
                            bufferCOM[6] = i+'0';
                            startSPI(10);
                            break;
							
                case 'v':
                            PIN=0;
                            copyBufferCOM2RF();
                            DLEN=1;
                            RFTXpacket();
                            bufferCOM[0] = 'V';
                            bufferCOM[1] = '=';
                            bufferCOM[3] = '.';
                            bufferCOM[6] = 'V';
                            bufferCOM[7] = '.';
                            level = getSupplyVoltage();
                            tmp = 127-level;
                            voltage = 261/(uns8)tmp;
                            i = (uns8)voltage%10;
                            bufferCOM[2] = i+'0';
                            i = (uns8)voltage*10;
                            i = i%10;
                            bufferCOM[4] = i+'0';
                            i=(uns8)voltage*100;
                            i = i%10;
                            bufferCOM[5] = i+'0';
                            startSPI(8);
                            break;
                case 'r':
                            bufferCOM[0] = 'R';
                            bufferCOM[1] = 'e';
                            bufferCOM[2] = 's';
                            bufferCOM[3] = 't';
                            bufferCOM[4] = 'a';
                            bufferCOM[5] = 'r';
                            bufferCOM[6] = 't';
                            bufferCOM[7] = ' ';
                            bufferCOM[8] = 'S';
                            bufferCOM[9] = 'P';
                            bufferCOM[10]= 'I';
                            bufferCOM[11]= '.';
                            startSPI(12);        // Sent data through SPI
                            restartSPI();
                default:                    	// Unknown command:
                            stopLEDR();       	// Long flash LED (1 s)
                            _LEDR = 1;
                            waitDelay(100);
                            _LEDR = 0;
                            startSPI(0);      	// and restart SPI
                            break;
            }
        }       
        if(RFRXpacket()){
            copyBufferRF2COM();
            pulseLEDG();
            startSPI(DLEN);         //   and send it via SPI
        }
    }
}
// *********************************************************************
#pragma packedCdataStrings 0
//                             00000000001111111111222222222233
#pragma cdata[__EEAPPINFO] =  "Commands: 1 2 3 t v r           "
// *********************************************************************