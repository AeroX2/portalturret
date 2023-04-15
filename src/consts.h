//Tweak these according to servo speed
#define OPEN_DURATION 1000
#define CLOSE_STOP_DELAY 100
#define MAX_ROTATION 50

#define CENTER_ANGLE 90
#define STATIONARY_ANGLE 110
#define NUM_LEDS 8

#define ACCEL_MEASUREMENTS 5

#define GFORCE_PICKED_UP_MIN 8
#define GFORCE_PICKED_UP_MAX 12
#define GFORCE_STEADY_MIN 9.5
#define GFORCE_STEADY_MAX 10.5
#define TIPPED_OVER_Z_TRESHOLD 5

#define USE_AUDIO 1

#define FREQ 50           // one clock is 20 ms
#define FREQ_MINIMUM 205  // 1ms is 1/20, of 4096
#define FREQ_MAXIMUM 410  // 2ms is 2/20, of 4096

#define ACCELEROMETER_PIN_X 34
#define ACCELEROMETER_PIN_Y 39
#define ACCELEROMETER_PIN_Z 36

#define MUSIC_RX_PIN 25
#define MUSIC_TX_PIN 26

#define MOTION_SENSOR_PIN 33
#define CENTER_LED_PIN 2
#define RING_LED_PIN 14
#define GUNS_LED_PIN 4
#define ROTATE_SERVO_PIN 22
#define WING_SERVO_PIN 19
#define WING_SWITCH_PIN 24