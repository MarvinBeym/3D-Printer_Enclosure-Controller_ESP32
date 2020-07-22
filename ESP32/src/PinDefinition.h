const int dht22_1_pin = 19;
const int dht22_2_pin = 18;
const int fan1_tacho_pin = 13;
const int fan1_pwm_pin = 14;
const int fan2_tacho_pin = 27;
const int fan2_pwm_pin = 26;
const int buzzer_pin = 4;
const int relay_in_pin = 21;
const int led2_data_pin = 22;

//EEPROM TABLE
/* ADR | Value saved in ADR
0 | fan1_dutyCycle 
1 | fan2_dutyCycle
2 | led1_enabled
3 | led2_color(byte converted)
4 | led2_mode(byte converted)
5 | display brightness
6 | display sleep
*/