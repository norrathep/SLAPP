/*
 * DHT.h
 *
 * Created: 2/15/2024 3:39:27 PM
 *  Author: Oak
 */ 


#ifndef DHT_H_
#define DHT_H_

extern uint8_t* pox_output;

inline void DHT_read_pox() {
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	
	HAL_GPIO_LED_clr();

	// pull the pin high and wait 250 milliseconds
	HAL_GPIO_DHT_write(HIGH);
	delayMilliseconds(250);

	uint8_t data[5] = {0};

	// now pull it low for ~20 milliseconds
	HAL_GPIO_DHT_out();
	HAL_GPIO_DHT_write(LOW);
	delayMilliseconds(20);
	HAL_GPIO_DHT_write(HIGH);
	delayMicroseconds(40);
	HAL_GPIO_DHT_in();
	
	int max_counter = 0;
	int k = 0;
	int avg = 0;

	// read in timings
	for ( i=0; i< 85; i++) {
		counter = 0;
		while (HAL_GPIO_DHT_read() == laststate) {
			counter++;
			//delayMicroseconds(2);
			if (counter == 255) {
				break;
			}
		}
		laststate = HAL_GPIO_DHT_read();

		if (counter == 255) {
			//break;
			continue;
		}

		if(counter > max_counter && i < 85-1) max_counter = counter;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			data[j/8] <<= 1;
			if (counter > 16) { // TODO: it was 6 before...
				data[j/8] |= 1;
				avg += counter;
				k++;
			}
			j++;
		}

	}

	// check we read 40 bits and that the checksum matches
	int c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
	memcpy(pox_output, &c, 4);
}





#endif /* DHT_H_ */