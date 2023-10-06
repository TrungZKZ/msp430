#include <msp430.h>

void Configure_Clock(void){
  if(CALBC1_8MHZ == 0xFF){
    while(1);
  }
  DC0CTL = 0;
  BCSCTL1 = CALBC1_8MHZ;
  DC0CTL = CALBC1_8MHZ;
  
  BCSCTL2 |= SELM_0;
}

void Configure_IO(void){
  P1DIR |= BIT0 +BIT6;
  P1OUT &= ~BIT0;
  P1OUT &= ~BIT6;
}

void Configure_Uart(){
  P1SEL = BIT1 + BIT2;
  P1SEL2 = BIT1 + BIT2;
  UCA0CTL1 |= UCSWRST;
  UCA0CTL0 = 0x00;
  
  UCA0CTL1 = UCSSEL_2 | UCSWRST;
  UCA0MCTL = UCBRF_1 | UCBRS_0 | UCOS16;
  UCA0BR0 = 52;
  UCA0BR0 = 00;
  
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
  _BIS_RS( GIE);
}

void UartSendByte(unsigned char byte){
 while(!(IFG2 & UC0TXIFG));
 UCA0TXBUF = byte;
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    Configure_Clock();
    Configure_IO();
    Configure_Uart();
    
    while(1){
     UartSendByte("a");
     __delay_cycles(8000000);
    }
}

#prama vector=USCUAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  UCA0TXBUF = UCA0TXBUF;
}
