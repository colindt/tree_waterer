const int TREE_N = 4;
const int PIN_TREE_SENSOR[] = {A0, A1, A2, A3}; //used as digital
const int PIN_TREE_LED[] = {3, 4, 5, 6};
int tree_sensor[4];

const int TANK_N = 2;
const int PIN_TANK_SENSOR[] = {A4, A5}; //used as digital
const int PIN_TANK_LED[] = {7, 8};
const int PIN_TANK_LED_EMPTY = 9;
int tank_sensor[2];

const int PIN_MOTOR = 2;
const int PIN_MOTOR_LED = 10;
bool motor_status = 0;


void setup() {
  for (int i = 0; i < TREE_N; i++) {
    pinMode(PIN_TREE_LED[i], OUTPUT);
    pinMode(PIN_TREE_SENSOR[i], INPUT_PULLUP);
  }

  for (int i = 0; i < TANK_N; i++) {
    pinMode(PIN_TANK_LED[i], OUTPUT);
    pinMode(PIN_TANK_SENSOR[i], INPUT_PULLUP);
  }
  pinMode(PIN_TANK_LED_EMPTY, OUTPUT);

  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_MOTOR_LED, OUTPUT);

  Serial.begin(9600);
}


void loop() {
  tree_waterer();
}


void tree_waterer() {
  sensor_read_write();

  while (!any_tree_sensors_on()) {
    motor(1);
    sensor_read_write();
    delay(250);
  }
  motor(0);
  
  delay(250);
}


void sensor_read_write() {
  read_sensors();
  led_write();
  serial_write_sensors();
}


void read_sensors() {
  for (int i = 0; i < TREE_N; i++) {
    tree_sensor[i] = digitalRead(PIN_TREE_SENSOR[i]);
  }

  for (int i = 0; i < TANK_N; i++) {
    tank_sensor[i] = digitalRead(PIN_TANK_SENSOR[i]);
  }
}


void led_write() {
  for (int i = 0; i < TREE_N; i++) {
    digitalWrite(PIN_TREE_LED[i], tree_sensor[i]);
  }

  bool any_tank_sensors_on = false;
  for (int i = 0; i < TANK_N; i++) {
    if (tank_sensor[i]) {
      any_tank_sensors_on = true;
    }
    digitalWrite(PIN_TANK_LED[i], tank_sensor[i]);
  }

  digitalWrite(PIN_TANK_LED_EMPTY, !any_tank_sensors_on);
}


void serial_write_sensors() {
  String sep = F(", ");
  
  Serial.print(F("tree: "));
  for (int i = 0; i < TREE_N; i++) {
    Serial.print(tree_sensor[i]);
    Serial.print(sep);
  }

  Serial.print(F("| tank: "));
  for (int i = 0; i < TANK_N; i++) {
    Serial.print(tank_sensor[i]);
    Serial.print(sep);
  }

  Serial.print(F("| motor: "));
  Serial.print(motor_status);
  
  Serial.println();
}


bool any_tree_sensors_on() {
  bool result = false;
  for (int i = 0; i < TREE_N; i++) {
    if (tree_sensor[i]) {
      result = true;
      break;
    }
  }
  return result;
}


void motor(bool status_) {
  motor_status = status_;
  digitalWrite(PIN_MOTOR,     status_);
  digitalWrite(PIN_MOTOR_LED, status_);
}


void led_test() {
  for (int i = 0; i < TREE_N; i++) {
    blink_led(PIN_TREE_LED[i]);
  }

  blink_led(PIN_TANK_LED_EMPTY);
  
  for (int i = 0; i < TANK_N; i++) {
    blink_led(PIN_TANK_LED[i]);
  }
  
  blink_led(PIN_MOTOR_LED);
}


void blink_led(int pin) {
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
}


void motor_test() {
  motor(1);
  delay(2000);
  motor(0);
  delay(2000);
}
