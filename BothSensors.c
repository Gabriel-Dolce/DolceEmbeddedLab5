#include <msp430.h>

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C //6682

#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-High Temperature (85 for Industrial, 105 for Extended)

int num = 0;

volatile float IntDegC = 0;
volatile float temp = 0;
volatile float IntLight = 0;


void ConfigureAdc_temp();
void ConfigureAdc_light();
int readData();

void main(void)

{

    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

 while(1){



        if(num == 0){
            ConfigureAdc_temp();
            __delay_cycles(400);
            temp = readData();
            IntDegC = (temp-CALADC_15V_30C)*(85-30)/(CALADC_15V_85C-CALADC_15V_30C)+30;

           num = 1;

        }

        else{ ConfigureAdc_light();
            //__delay_cycles(400);
            IntLight = readData();

            num = 0;



    }
 }

}

//configure from example code
void ConfigureAdc_temp(){
    ADCCTL0 |= ADCSHT_8 | ADCON;                                  // ADC ON,temperature sample period>30us
       ADCCTL1 |= ADCSHP;                                            // s/w trig, single ch/conv, MODOSC
       ADCCTL2 &= ~ADCRES;                                           // clear ADCRES in ADCCTL
       ADCCTL2 |= ADCRES_2;                                          // 12-bit conversion results
       ADCMCTL0 |= ADCSREF_1 | ADCINCH_12;                           // ADC input ch A12 => temp sense
       ADCIE |=ADCIE0;                                               // Enable the Interrupt request for a completed ADC_B conversion

       PMMCTL0_H = PMMPW_H;                                          // Unlock the PMM registers
          PMMCTL2 |= INTREFEN | TSENSOREN;                              // Enable internal reference and temperature sensor
}


void ConfigureAdc_light(){

        P1SEL1 |= BIT1;
        P1SEL0 |= BIT1;


    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks 16 clock cycles per sample adcon turns adc on.
              ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer sampling signal from internal way
              ADCCTL2 &= ~ADCRES;                                      // clear ADCRES in ADCCTL
              ADCCTL2 |= ADCRES_2;                                     // 12-bit conversion results
              ADCMCTL0 |= ADCINCH_1 | ADCSREF_1;                                   // A1 ADC input select; Vref=AVCC
              ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt
}

int readData(){

    ADCCTL0 |= ADCENC | ADCSC;     // Sampling and conversion start
       return ADCMEM0;

}
