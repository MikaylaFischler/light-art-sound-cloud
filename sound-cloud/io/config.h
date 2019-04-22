#ifndef CLOUD_PIN_DEFS_H_
#define CLOUD_PIN_DEFS_H_

#define ROW_1_2_LED_STRIP_PIN 2
#define ROW_1_2_LED_LENGTH 46
#define ROW_3_4_LED_STRIP_PIN 3
#define ROW_3_4_LED_LENGTH 0
#define TOP_LED_STRIP_PIN 4
#define TOP_LED_LENGTH 0

#define LED_BRIGHTNESS_PIN A13
#define LED_MODE_PIN 31

#define S_LED_R	33
#define S_LED_G	34
#define S_LED_B	35

// segments

/*
level 0: segment 0: strip 0, offset 0, length 23
level 1: segment 1: strip 0, offset 23, length 23
level 2: segment 2: strip 1, offset 0, length 21
level 3: segment 3: strip 1, offset 21, length 19

top: segment 4: strip 2, offset 0, length 11
	segment 5: strip 1, offset 40, length 1
top: segment 6: strip 2, offset 11, length 12
	segment 7: strip 1, offset 41, length 1
top: segment 8: strip 2, offset 23, length 12
	segment 9: strip 1, offset 42, length 1
top: segment 10: strip 2, offset 35, length 13
top: segment 11: strip 2, offset 48, length 12

*/

#endif
