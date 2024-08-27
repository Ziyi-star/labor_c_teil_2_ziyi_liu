#include "main.h"


// Define a structure to hold the ADC values
typedef struct {
    uint16_t adc0;
    uint16_t adc1;
    uint16_t adc2;
} ADCValues;


ADCValues initialAdcValues;

void check_adc_rotate() {
    uint16_t currentAdc0, currentAdc1, currentAdc2;
    // Loop until the break condition is met
    while (1) {
        // Read current ADC values
        currentAdc0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        currentAdc1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        currentAdc2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

        // Check if current ADC values are close enough to initial values
        if (abs(currentAdc0 - initialAdcValues.adc0) < 30 &&
            abs(currentAdc1 - initialAdcValues.adc1) < 30 &&
            abs(currentAdc2 - initialAdcValues.adc2) < 30) {
            stop();  // Stop the robot
            break;
        } else {
            rotate_clockwise();  // Continue rotating
            _delay_ms(100);  // Delay to allow for ADC reading stabilization
        }
    } 
}

int main(void) {
	
    setup_heartbeat_timer();
    int start = 0;
    
    init_run();
   
    USART_init(UBRR_SETTING);
    
	// ADC zu lesen 
    DR_ADC0 &= ~(1 << DP_ADC0);
    DR_ADC1 &= ~(1 << DP_ADC1);
    DR_ADC2 &= ~(1 << DP_ADC2);

    ADC_init();

    unsigned char strbuff[sizeof(ADCMSG) + 15];

    uint16_t adcval0 = 0;
    uint16_t adcval1 = 0;
    uint16_t adcval2 = 0;
    
    // Setup everything
    setup_ddr_all();

    // Allocate 1 byte in memory/on heap for a representation (model) of the register and clear the contents directly, 
    // and update everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;
    
    
    int left;
    int right;
    int middle;
    
    int last_middle = 0;
    int last_right = 0;
    char message = 0;
    char str_buffer [50];
    char state = 0;

    while (1) {
		
		message = USART_receiveByte();
		
		
		 
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
        
        left = (adcval2 > 350);
        right = (adcval0 > 300);
        middle = (adcval1 > 200);
        
        update_model(regmdl, left, middle, right);
		
			//STARTFIELD
		if(message == 'S' && left && middle && right && state != 'S'){
			state = 'S';
			USART_print("Here I am once more, going down the only round I've ever known...\n"); 
		}
		
					//NOT STARTFIELD
		if(state == 0 && !(middle && right && left) ){
				if (centi_second) {
					USART_print("Hey you, you know what to do. :-)\n");
					centi_second = 0;
				}
			}
		
		//LED goes with the adc
        if (*regmdl != last_model_state) {
            update_hardware(regmdl);
            last_model_state = *regmdl;
        }
        if (state == 'S'){
				//run logic
				if (!left && middle && !right) {
				   gerade();
				   start = 0;
				}
				else if (!left && !middle && !right) {
					
					if(!last_middle) {
				   
				   if (last_right){
					   big_right();
					   }
					else{
						big_left();
						}
					} else{
						gerade();
					}
						
					start = 0;
				}
				else if (!left && !middle && right) {
				   small_right();
					start = 0;

				}
				else if (!left && middle && right) {
				   small_right();
				   start = 0;

				}
				else if(left && !middle && !right){
					small_left();
					start = 0;

				}
				else if (left && !middle && right){
					gerade();
					start = 0;
				}
				else if( left && middle && !right){
					small_left();
					start = 0;
				}
				//startfield
				else if (left && middle && right){
					gerade();
					//if schwellwert dann hochzählen
					if (!start){
						start = m_second;
						}
					if(m_second-start > 10){
						currentLap++;
						if (currentLap == 2){
						USART_print("YEAH, done first lap, feelig well, going for lap 2/3\n");
						}
						if (currentLap == 3){
						USART_print("YEAH YEAH, done 2nd lap, feeling proud, going for lap 3/3\n"); 
						}
						if (currentLap == 4) {
							int totalSeconds = (int)(time(NULL) - raceStartTime);
							//todo: reset
							sprintf(str_buffer, " Finally finished , It's over and done now, after #%d seconds. Thanks for working with me! :-) I will reset myself in 5 seconds. Take care!\n", totalSeconds);
							USART_print(str_buffer);
							stop();
							_delay_ms(5000);
							wdt_enable(WDTO_4S);
							// Loop forever, watchdog will reset the microcontroller
							while(1);  
						}
					}
				}	
				if (left || right){
					if (right){
						last_right = 1;
						}
					else{
						last_right = 0;
						}
						
						last_middle = 0;
					} else{
						last_middle = middle;
					}
					
			//print 1hz things
			if (second) {
				sprintf(str_buffer, "Currently I go round #%d\n", currentLap);
				USART_print(str_buffer);
				second = 0;
			}
				
		}
		if (state == 'P'){
				isPaused = !isPaused;
					if (isPaused) {
						stop();  
						run_led_sequence(regmdl);
						if (second){
							USART_print("Pause\n");
							second = 0;
						}
					}else {
						break;
					}
			}
			
			if (state == 'T'){
				// Store current ADC values for 'H'
				initialAdcValues.adc0 = adcval0;
				initialAdcValues.adc1 = adcval1;
				initialAdcValues.adc2 = adcval2;
				rotate_clockwise();
				if (half_second){
					USART_print("lalala.\n");
					half_second = 0;
				}
				if (state == 'H'){
					stop();  
					// Function to handle the rotation until ADC values match
					check_adc_rotate();
				}
			}
	}
	return 0;      
}
    