#include <msp430g2553.h>

#define STEP_PIN BIT0
#define DIR_PIN BIT1
#define ENABLE_PIN BIT2

void setupMotor() {
    P1DIR |= STEP_PIN | DIR_PIN | ENABLE_PIN; // �?t ch�n STEP, DIR v� ENABLE l� ch�n output
    P1OUT &= ~ENABLE_PIN; // T?t ch�n ENABLE (k�ch ho?t DRV8825)

}

void stepMotor(int direction) {
    if (direction == 1) {
        P1OUT |= DIR_PIN; // �?t ch�n DIR d? quay theo chi?u thu?n kim d?ng h?
    } else {
        P1OUT &= ~DIR_PIN; // �?t ch�n DIR d? quay ngu?c chi?u kim d?ng h?
    }

    // T?o xung b?ng c�ch d?t ch�n STEP l�n cao v� th?p
    P1OUT |= STEP_PIN;
    __delay_cycles(1000); // �?i m?t kho?ng th?i gian ng?n
    P1OUT &= ~STEP_PIN;
    __delay_cycles(1000); // �?i m?t kho?ng th?i gian ng?n
}

int main(void) {
     WDTCTL = WDTPW + WDTHOLD;

    setupMotor();

    while (1) {
        // �i?u khi?n d?ng co bu?c ? ch? d? half step
        // V� du: Quay theo chieu thuan kim dong ho trong 1 gi�y
        for (int i = 0; i < 200; i++) {
            stepMotor(1);
            __delay_cycles(1000); // �?i m?t kho?ng th?i gian ng?n gi?a c�c bu?c
        }
    }
}