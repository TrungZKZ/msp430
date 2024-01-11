
#include "msp430g2553.h"
#include "i2c_lcd.h"

volatile int pulseCount = 0; // Bi?n d?m s? xung t? encoder
volatile unsigned long startTime = 0; // Th?i gian b?t d?u vòng d?ng co
volatile unsigned long endTime = 0; // Th?i gian k?t thúc vòng d?ng co
volatile unsigned long elapsedTime = 0; // Th?i gian trôi qua gi?a các vòng d?ng co
volatile float motorSpeed = 0.0; // T?c d? d?ng co
//khai bao dau "="
unsigned char customChar[8] = { 
  0x00,
  0x00,
  0x00,
  0x1F,
  0x00,
  0x1F,
  0x00,
  0x00
};

void setupEncoder() {
    P2DIR &= ~(BIT0 | BIT1); // Ð?t chân P2.0 và P2.1 là chân input
    P2REN |= BIT0 | BIT1; // Kích ho?t resistor pull-up/down cho chân P2.0 và P2.1
    P2OUT |= BIT0 | BIT1; // Ð?t resistor pull-up cho chân P2.0 và P2.1

    P2IES |= BIT0; // Ng?t x?y ra khi chân P2.0 chuy?n t? cao xu?ng th?p
    P2IE |= BIT0; // Kích ho?t ng?t cho chân P2.0
    __enable_interrupt(); // Kích ho?t ng?t toàn c?c
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void) {
    if (P2IFG & BIT0) { // Ki?m tra ng?t t? chân P2.0
        pulseCount++; // Tang bi?n d?m s? xung
        P2IFG &= ~BIT0; // Xóa c? ng?t c?a chân P2.0
    }
}

//Khai bao ngat cho nut nhan
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
  if (P1IFG & BIT3) // Check if P1.3 interrupt flag is set
  {
    TA0CCR1 = 191;
    P1IFG &= ~BIT3; // Clear P1.3 interrupt flag
  }
  
  if (P1IFG & BIT4) // Check if P1.4 interrupt flag is set
  {
    TA0CCR1 = 127;
    P1IFG &= ~BIT4; // Clear P1.4 interrupt flag
  }
  
  if (P1IFG & BIT5) // Check if P1.5 interrupt flag is set
  {
    TA0CCR1 = 63;
    P1IFG &= ~BIT5; // Clear P1.5 interrupt flag
  }
}

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  //int a;
  BCSCTL1 = CALBC1_1MHZ;      
  DCOCTL = CALDCO_1MHZ;
  setupEncoder();
  P1DIR &= ~(BIT3 + BIT4 + BIT5); // Set P1.3, P1.4, P1.5 as input
  P1REN |= BIT3 + BIT4 + BIT5; // Enable pull-up/pull-down resistors for P1.3, P1.4, P1.5
  P1OUT |= BIT3 + BIT4 + BIT5; // Set pull-up resistors for P1.3, P1.4, P1.5
  P1IE |= BIT3 + BIT4 + BIT5; // Enable interrupt for P1.3, P1.4, P1.5
  P1IES |= BIT3 + BIT4 + BIT5; // Set interrupt edge select to falling edge for P1.3, P1.4, P1.5
  P1IFG &= ~(BIT3 + BIT4 + BIT5); // Clear interrupt flags for P1.3, P1.4, P1.5
  __bis_SR_register(GIE); // Enable global interrupts
  P1DIR |= BIT0; // Set P1.0 as output
  P1OUT |= BIT0; // Set P1.0 to HIGH
  
  P1DIR |= BIT2; // Set P1.2 as output
  P1SEL |= BIT2;
  
  TA0CCR0 = 255; // Set period (PWM resolution)
  TA0CCTL1 = OUTMOD_7; // Set output mode to reset/set
  TA0CCR1 = 200; // thay doi toc do dong co o day
  TA0CTL = TASSEL_2 + MC_1; // Use SMCLK in up mode
  
  LCD_Init(0X27, 2, 16);
  LCD_Clear(); //xoa cac ky tu vo dinh
  
  LCD_createChar(0, customChar);
  
  while(1)
  {
    __delay_cycles(1000000); // Ð?i m?t giây

    motorSpeed = pulseCount / 96.0; // Tính t?c d? d?ng co (vòng m?i giây)

    pulseCount = 0; // Ð?t l?i bi?n d?m s? xung
    LCD_SetCursor(0,0);//set vi tri lcd(cot, hang)
    LCD_Print("Toc Do");
    LCD_SetCursor(6,0);//set vi tri lcd(cot, hang)
    LCD_writeChar(0);//in char custom ra mang hinh
    lcd_put_num(motorSpeed,0,0);

    
    LCD_SetCursor(0,1);
    LCD_Print("PWM=");
    lcd_put_num((255-TA0CCR1)*100/255,0,0);
    LCD_SetCursor(7,1);
    LCD_Print("xung=");
    lcd_put_num(pulseCount,0,0);
  }
}