#include <msp430.h>


void configureLight();
int readSensor();

volatile float brightness = 0;

/**
 * main.c
 */
int main(void)
{
 WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	while(1)
	{
	configureLight();
	brightness = readSensor();
	}
}

void configureLight(){

    P1SEL1 |= BIT1;
    P1SEL0 |= BIT1;

    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks 16 clock cycles per sample adcon turns adc on.
          ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer sampling signal from internal way
          ADCCTL2 &= ~ADCRES;                                      // clear ADCRES in ADCCTL
          ADCCTL2 |= ADCRES_2;                                     // 12-bit conversion results
          ADCMCTL0 |= ADCINCH_1;                                   // A1 ADC input select; Vref=AVCC
          ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

}

int readSensor(){

    ADCCTL0 |= ADCENC | ADCSC;     // Sampling and conversion start
       while (!(ADCIFG & ADCIFG0));   // Wait for sample to be sampled and converted
       return ADCMEM0;

}
