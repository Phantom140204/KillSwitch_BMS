const float voltage_divider_constant = 3.215;
const int voltage_pin = A2;
const int current_pin = A5;
const int kill_switch_pin = A0;
const int gate_pin = 6;
const int red_led_pin = 2;
const int green_led_pin = 3;

bool current_kill_switch_state;
bool gate_pin_state = 0;
bool red_led_state = 0;
bool danger_status = 0;

long prev_blink_time = 0;
long prev_log_time = 0;

void setup() {
  pinMode(voltage_pin, INPUT);
  pinMode(current_pin, INPUT);
  pinMode(kill_switch_pin, INPUT_PULLUP);

  pinMode(gate_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  double raw_voltage = ((1.0*analogRead(voltage_pin))*5)/1023;
  double battery_voltage = raw_voltage*voltage_divider_constant;

  int kill_switch_reading = analogRead(kill_switch_pin);
  if(kill_switch_reading > 100){
    current_kill_switch_state = 0;
  }
  else{
    current_kill_switch_state = 1;
  }
  if((current_kill_switch_state != gate_pin_state) && (danger_status != 1)){
    delay(100); // Debouncing
    gate_pin_state = current_kill_switch_state;
    digitalWrite(gate_pin, gate_pin_state);
  }

  if(battery_voltage < 10){
    digitalWrite(red_led_pin, LOW);
    digitalWrite(green_led_pin, HIGH);
    danger_status = 1;
    digitalWrite(gate_pin, HIGH);
  }
  else if(battery_voltage > 11.5){
    digitalWrite(red_led_pin, HIGH);
    digitalWrite(green_led_pin, LOW);
    danger_status = 0;
  }
  else{
    if((millis() - prev_blink_time) > 1000){
      digitalWrite(red_led_pin, red_led_state);
      red_led_state = 1 - red_led_state;
      digitalWrite(green_led_pin, red_led_state);
      prev_blink_time = millis();
    }
    danger_status = 0;
  }

  if((millis() - prev_log_time) > 1000){
    Serial.print("Raw Voltage: "); Serial.println(raw_voltage);
    Serial.print("Battery Voltage: "); Serial.println(battery_voltage);
    Serial.print("Kill Switch State: "); Serial.println(current_kill_switch_state);
    Serial.print("Gate Pin State: "); Serial.println(gate_pin_state);
    Serial.println("\n");
    prev_log_time = millis();
  }
}