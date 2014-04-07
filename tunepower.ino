int16_t LED_BUILTIN = 1;  //LED on Model A  

int16_t DIGITAL_OUT_PIN = 0;

int16_t AUDIO_IN_PIN = 2;
int16_t AUDIO_IN_ANALOG = 1;

int16_t AMP_IN_PIN = 5;
int16_t AMP_IN_ANALOG = 0;

int16_t MAX_READ = 1024;
int16_t MAX_WRITE = 255;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(DIGITAL_OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(AUDIO_IN_PIN, INPUT);
  pinMode(AMP_IN_PIN, INPUT);
}

float signal = 0.0;
float reference = 0.5;

float k_ref = 0.999;
float k = 0.999;

// End of the current time cycle (52 days)
uint32_t END_OF_TIME = (uint32_t)(-1);
uint32_t SECOND = 1000;
uint32_t DEBUG_SHUTOFF_START = 1 * SECOND;
//uint32_t SHUTOFF_INTERVAL = 60*60*1000;
uint32_t SHUTOFF_INTERVAL = 10*SECOND;

bool isOn = false;
bool shutting_off = false;
uint32_t time_prev = 0;
uint32_t turnoff_time = END_OF_TIME;

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

STATIC_ASSERT(sizeof(char) == 1, size_of_char);
STATIC_ASSERT(sizeof(int) == 2, size_of_int);
STATIC_ASSERT(sizeof(unsigned long) == 4, size_of_unsigned_long);
STATIC_ASSERT(sizeof(long) == 4, size_of_long);
STATIC_ASSERT(sizeof(long long) == 8, size_of_long_long);
STATIC_ASSERT(sizeof(float) == 4, size_of_float);

// Returns 1 if the output should be on
bool doTimer(bool signal) {
  bool nextOn = isOn;
  // Process timer
  uint32_t time = millis();
  // Handle time overflow
  if (time < time_prev) {
    turnoff_time = time + SHUTOFF_INTERVAL;
    time_prev = 0;
  }

  if (signal) {
    turnoff_time = time + SHUTOFF_INTERVAL;
    shutting_off = false;
    nextOn = true;
  } else if (isOn && time > turnoff_time)  {
    shutting_off = false;
    nextOn = false;
    turnoff_time = 0;
  } else if (isOn && time > turnoff_time - (SHUTOFF_INTERVAL - DEBUG_SHUTOFF_START) ) {
    shutting_off = true;
    nextOn = true;
  } else {
    shutting_off = false;
    nextOn = isOn;
  }

  time_prev = time;
  isOn = nextOn;
  return nextOn;
}

// the loop routine runs over and over again forever:
void loop() {

  // Potentiometer to set gain
  float gainKnob = analogRead(AMP_IN_ANALOG) / (float)(MAX_READ);
  float gain = exp(10.0 * gainKnob); // map the volume knob exponentially from 1 to 22026

  float rawAudio = analogRead(AUDIO_IN_ANALOG) / (float)(MAX_READ);

  // Audio is the difference between raw input and the reference voltage
  float audio = rawAudio - reference;

  // Auto adjust the reference for any bias in center voltage
  reference = k_ref * reference + (1.0 - k_ref) * rawAudio;

  // How much power there was at this instant
  float instPower = sqrt(audio * audio);
  signal = k * signal + (1.0 - k) * instPower;

  digitalWrite(LED_BUILTIN, shutting_off ? HIGH : LOW);

  bool detected = signal * gain > 1.0;
  bool on = doTimer(detected);

  digitalWrite(DIGITAL_OUT_PIN, on ? HIGH : LOW);
}
