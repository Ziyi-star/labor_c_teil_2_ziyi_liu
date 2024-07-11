#include "track.h"


// Leichte Abbiegen nach rechts
void turn_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 200);
    //rechts
    setDutyCycle(PD6, 200);
    // Set IN1 to HIGH and don't set IN2 to HIGH (leave LOW) -> Left motors FORWARD
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT); 
    // Set IN2 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // Set IN3 to HIGH and don't set anything else to HIGH -> Right motors BACKWARD
    PORTB |= (1 << PIN_IN3_BACKWARD_RIGHT);
    // Set IN4 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);

}

// grosse Abbiegen nach links
void turn_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 200);
    //rechts
    setDutyCycle(PD6, 200);
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
void drive_right(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 200);
    //rechts
    setDutyCycle(PD6, 0);
    // links  FORWARD, 1
    PORTD |= (1 << PIN_IN1_FORWARD_LEFT);
    // links BACKWARD 1
    PORTB &= ~(1 << PIN_IN2_BACKWARD_LEFT);
    // RIGHT FORWARD 1
    PORTB &= ~(1 << PIN_IN4_FORWARD_RIGHT);
    // Right BACKWARD 0
    PORTB &= ~(1 << PIN_IN3_BACKWARD_RIGHT);
}

// Leichte Abbiegen nach links
void drive_left(){
    // Set the duty cycles for PD5/PD6
    //links
    setDutyCycle(PD5, 0);
    //rechts
    setDutyCycle(PD6, 200);
    // links  FORWARD, 1
    PORTD &= ~(1 << PIN_IN1_FORWARD_LEFT);
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

void handleDrivingLogic(int *left, int *middle, int *right, int *last_right, int *start, char * field_count) {
	if (currentLap > 3){
		return;
		}
	
    if (!(*left) && (*middle) && !(*right)) {
        gerade();
        *start = 0;
    } else if (!*left && !*middle && !*right) {
		*start = 0;
        if (*last_right) {
            turn_right();
        } else {
            turn_left();
        }
        
        
    } else if (!*left && !*middle && *right) {
        drive_right();
        //turn_right();
        *start = 0;
    } else if (!*left && *middle && *right) {
        gerade();
        *start = 0;
    } else if (*left && !*middle && !*right) {
        drive_left();
        //turn_left();
        *start = 0;
    } else if (*left && !*middle && *right) {
        gerade();
        *start = 0;
    } else if (*left && *middle && !*right) {
        gerade();
        *start = 0;
    }
    // Start field
    else if (*left && *middle && *right) {
        gerade();
        // If threshold then increment
        if (!*start) {
            *start = m_second;
        }
        if (m_second - *start > 10 && *field_count == 0) {
			isCompleted = 1;
			*field_count = 1;
			// When the robot is on the start field and the race has not started
			if (isCompleted && currentLap == 0 && second == 1) {
            USART_print("Same story, different student ... boring, IES needs to refactor this course.\n");
        }
            currentLap++;
            if (currentLap > 3){
				stop();
				}
        }
    }
    
    if( !(*right) || !(*left) || !(*middle) ){
		field_count = 0;
		}
    
    if (*left || *right) {
        
        if (*right) {
            *last_right = 1;
        } else {
            *last_right = 0;
        }
        

        
        
    }
}
