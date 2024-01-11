#include <msp430g2553.h>

#define STEP_PIN BIT0
#define DIR_PIN BIT1
#define ENABLE_PIN BIT2

void setupMotor() {
    P1DIR |= STEP_PIN | DIR_PIN | ENABLE_PIN; // Ð?t chân STEP, DIR và ENABLE là chân output
    P1OUT &= ~ENABLE_PIN; // T?t chân ENABLE (kích ho?t DRV8825)

}

void stepMotor(int direction) {
    if (direction == 1) {
        P1OUT |= DIR_PIN; // Ð?t chân DIR d? quay theo chi?u thu?n kim d?ng h?
    } else {
        P1OUT &= ~DIR_PIN; // Ð?t chân DIR d? quay ngu?c chi?u kim d?ng h?
    }

    // T?o xung b?ng cách d?t chân STEP lên cao và th?p
    P1OUT |= STEP_PIN;
    __delay_cycles(1000); // Ð?i m?t kho?ng th?i gian ng?n
    P1OUT &= ~STEP_PIN;
    __delay_cycles(1000); // Ð?i m?t kho?ng th?i gian ng?n
}

int main(void) {
     WDTCTL = WDTPW + WDTHOLD;

    setupMotor();

    while (1) {
        // Ði?u khi?n d?ng co bu?c ? ch? d? half step
        // Ví du: Quay theo chieu thuan kim dong ho trong 1 giây
        for (int i = 0; i < 200; i++) {
            stepMotor(1);
            __delay_cycles(1000); // Ð?i m?t kho?ng th?i gian ng?n gi?a các bu?c
        }
    }
}