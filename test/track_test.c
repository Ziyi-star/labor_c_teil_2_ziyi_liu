#include <avr/io.h>
#include "iesusart.h"
#include "iesmotors.h"
#include "iesadc.h"

// Konstanten für die PB- und PD-Pins
#define PIN_IN1_FORWARD_LEFT PD7
#define PIN_IN2_BACKWARD_LEFT PB0
#define PIN_IN3_BACKWARD_RIGHT PB1
#define PIN_IN4_FORWARD_RIGHT PB3


// Leichte Abbiegen nach rechts
void small_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 255);
    //rechts
    setDutyCycle(PD6, 180);
    // Set IN1 to HIGH and don't set IN2 to HIGH (leave LOW) -> Left motors FORWARD
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT); 
    // Set IN2 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // Set IN3 to HIGH and don't set anything else to HIGH -> Right motors BACKWARD
    PORTB |= (1 << PIN_IN3_BACKWARD_RIGHT);
    // Set IN4 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);

}

// small Abbiegen nach links
void small_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 180);
    //rechts
    setDutyCycle(PD6, 255);
    // links  FORWARD, 0
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB |= (1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void big_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 155);
    //rechts
    setDutyCycle(PD6, 255);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void big_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 255);
    //rechts
    setDutyCycle(PD6, 155);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

void gerade(){
    // Set the duty cycles for PD5/PD6, speed
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
    // Both sides stop, backward
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    // Both sides forward
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
}

void stop(){
	 // Both sides stop, backward & forward
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
	}


void init_run(){
    // Set Data Direction Register C [0|1|2] as input.
    DDRC = ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    // Initialize U(S)ART!
    USART_init(UBRR_SETTING);
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);
    // Set PD7 as output (IN1)
    DDRD |= (1 << PIN_IN1_FORWARD_LEFT);
    // Make PWM work on PD[5|6], immer fur motor
    setupTimer0();
    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);
    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
}

#include "track.h"

// Schnelle Drehung im Uhrzeigersinn
void rotate_clockwise() {
    // Set the duty cycles for maximum differential speed
    //links
    setDutyCycle(PD5, 255);  // Maximum speed forward for left motors
    //rechts
    setDutyCycle(PD6, 0);    // Stop right motors or set to 0 for minimum speed if reverse not desired
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB |= (1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

