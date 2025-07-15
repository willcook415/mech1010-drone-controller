/* 
--------------------------------------------  
-------- MECH1010 Coursework 2024 --------
-------- Name:  William Cook
-------- Username:  mlsv9213
--------------------------------------------
*/

// Pin Definitions
const int pot_pin = A0;
const int enable_pin = 10;
const int direction_pin = 9;
const int green_led = 2;
const int yellow_led = 3;
const int red_led = 4;

// Constants
const float target_height = 0.491; // metres (horizontal)
const float beam_length = 0.491;   // pivot to propeller
const float angle_m = 0.346;
const float angle_c = -141.85;

const float Kp = 1400.0;
const int control_frequency = 25; // Hz
const unsigned long loop_interval = 1000 / control_frequency;
const float acceptable_error = 0.03;

// Variables
unsigned long start_time = 0;
unsigned long last_loop_time = 0;

float angle = 0.0;
float height = 0.0;
float error = 0.0;

bool reached_target = false;
float first_reach_time = 0.0;
float max_error = 0.0;
float error_sum = 0.0;
int sample_count = 0;

int stable_counter = 0;
const int stable_threshold_frames = 12;

// Functions
  float readAngle() {
    int pot_value = analogRead(pot_pin);
    return angle_m * pot_value + angle_c;
  }

  float calculateHeight(float angle_deg) {
    return beam_length * cos(radians(angle_deg));
  }

  float computeControlSignal(float error) {
    return constrain(Kp * error, 0, 255);
  }

  void setLEDs(float error) {
    if (abs(error) <= acceptable_error) {
      digitalWrite(green_led, HIGH);
      digitalWrite(yellow_led, LOW);
      digitalWrite(red_led, LOW);
    } else if (error < -acceptable_error) {
      digitalWrite(green_led, LOW);
      digitalWrite(yellow_led, HIGH);
      digitalWrite(red_led, LOW);
    } else {
      digitalWrite(green_led, LOW);
      digitalWrite(yellow_led, LOW);
      digitalWrite(red_led, HIGH);
    }
  }

  void shutdownSequence() {
    digitalWrite(green_led, HIGH);
    digitalWrite(yellow_led, HIGH);
    digitalWrite(red_led, HIGH);

    analogWrite(enable_pin, (int)(0.3 * 255));
    delay(1000);
    analogWrite(enable_pin, 0);

    Serial.println("4. Shutdown");
    Serial.print("Time to reach target height: ");
    Serial.println(first_reach_time, 2);
    Serial.print("Maximum Error: ");
    Serial.println(max_error, 2);
    Serial.print("Average Error: ");
    Serial.println((sample_count > 0) ? error_sum / sample_count : 0.0, 2);
  }

void setup() {
  pinMode(pot_pin, INPUT);
  pinMode(enable_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led, OUTPUT);

  Serial.begin(9600);
  Serial.println("0. System Started");

  digitalWrite(green_led, HIGH);
  delay(500);
  digitalWrite(green_led, LOW);

  Serial.println("1. System Initiated");
  Serial.println("2. Controller Starting");
  Serial.println("Time,Angle,Height,Error,PWM,StableCount,Reached");

  digitalWrite(direction_pin, LOW); // push up
  start_time = millis();
  last_loop_time = millis();
}

void loop() {
  unsigned long current_time = millis();

  if ((current_time - start_time) < 5000) {
    if ((current_time - last_loop_time) >= loop_interval) {
      last_loop_time = current_time;

      float t = (current_time - start_time) / 1000.0;

      // Sensor Reading
      angle = readAngle();
      height = calculateHeight(angle);
      error = target_height - height;

      // Control Signal
      float control_signal = computeControlSignal(error);
      analogWrite(enable_pin, (int)control_signal);

      // LED Status
      setLEDs(error);

      // Debug Output
      Serial.print(t, 2);
      Serial.print(",");
      Serial.print(angle, 2);
      Serial.print(",");
      Serial.print(height, 2);
      Serial.print(",");
      Serial.print(error, 2);
      Serial.print(",");
      Serial.print((int)control_signal);
      Serial.print(",");
      Serial.print(stable_counter);
      Serial.print(",");
      Serial.println(reached_target);

      // Target Reached Check
      if (abs(error) <= acceptable_error) {
        stable_counter++;
      } else {
        stable_counter = 0;
      }

      if (!reached_target && stable_counter >= stable_threshold_frames) {
        reached_target = true;
        first_reach_time = t;
      }

      // Track Performance Stats
      if (reached_target) {
        max_error = max(max_error, abs(error));
        error_sum += abs(error);
        sample_count++;
      }
    }
  } else {
    shutdownSequence();
    while (true); // shutdown
  }

}