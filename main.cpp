#include <Arduino.h>
#include <ArduinoBLE.h>

BLEService ServoService("0"); // Servo arm service

// Custom 128-bit UUIDs so App Inventor recognizes the characteristics
BLEByteCharacteristic switchCharacteristic("1", BLERead | BLEWrite); // DEL RGB
BLEByteCharacteristic servo1Characteristic("2", BLERead | BLEWrite); // servos 1-4
BLEByteCharacteristic servo2Characteristic("3", BLERead | BLEWrite);
BLEByteCharacteristic servo3Characteristic("4", BLERead | BLEWrite);
BLEByteCharacteristic servo4Characteristic("5", BLERead | BLEWrite);

BLEDescriptor desc1("2901", "servo 1");
BLEDescriptor desc2("2901", "servo 2");
BLEDescriptor desc3("2901", "servo 3");
BLEDescriptor desc4("2901", "servo 4");

int servoinputs[4] = {0, 0, 0, 0};

void majinput();
void majpos();

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  // set LED's pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, false); // when the central disconnects, turn off the LED
  digitalWrite(LEDR, true);         // will turn the LED off
  digitalWrite(LEDG, true);         // will turn the LED off
  digitalWrite(LEDB, true);         // will turn the LED off

  // begin initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® false Energy failed!");

    while (1)
      ;
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(ServoService);

  servo1Characteristic.addDescriptor(desc1);
  servo2Characteristic.addDescriptor(desc2);
  servo3Characteristic.addDescriptor(desc3);
  servo4Characteristic.addDescriptor(desc4);

  // add the characteristic to the service
  ServoService.addCharacteristic(switchCharacteristic);
  ServoService.addCharacteristic(servo1Characteristic);
  ServoService.addCharacteristic(servo2Characteristic);
  ServoService.addCharacteristic(servo3Characteristic);
  ServoService.addCharacteristic(servo4Characteristic);

  // add service
  BLE.addService(ServoService);

  // set the initial value for the characteristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
  byte a = 03;
  int b = a;
  Serial.println(b);
}

void loop()
{
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central)
  {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, true); // turn on the LED to indicate the connection

    // while the central is still connected to peripheral:
    while (central.connected())
    {
      majinput();
      majpos();
      delay(5);
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, false); // when the central disconnects, turn off the LED
    digitalWrite(LEDR, true);         // will turn the LED off
    digitalWrite(LEDG, true);         // will turn the LED off
    digitalWrite(LEDB, true);         // will turn the LED off
  }
}
// mise à jour des inputs
void majinput()
{
  // if the remote device wrote to the characteristic,
  // use the value to control the LED:
  if (switchCharacteristic.written())
  {
    switch (switchCharacteristic.value())
    { // any value other than 0
    case 01:
      Serial.println("Red LED on");
      digitalWrite(LEDR, false); // will turn the LED on
      digitalWrite(LEDG, true);  // will turn the LED off
      digitalWrite(LEDB, true);  // will turn the LED off
      break;
    case 02:
      Serial.println("Green LED on");
      digitalWrite(LEDR, true);  // will turn the LED off
      digitalWrite(LEDG, false); // will turn the LED on
      digitalWrite(LEDB, true);  // will turn the LED off
      break;
    case 03:
      Serial.println("Blue LED on");
      digitalWrite(LEDR, true);  // will turn the LED off
      digitalWrite(LEDG, true);  // will turn the LED off
      digitalWrite(LEDB, false); // will turn the LED on
      break;
    default:
      Serial.println(F("LEDs off"));
      digitalWrite(LEDR, true); // will turn the LED off
      digitalWrite(LEDG, true); // will turn the LED off
      digitalWrite(LEDB, true); // will turn the LED off
      break;
    }
  }
  if (servo1Characteristic.written())
  {
    servoinputs[0] = (int)servo1Characteristic.value();
  }
  if (servo2Characteristic.written())
  {
    servoinputs[1] = (int)servo2Characteristic.value();
  }
  if (servo3Characteristic.written())
  {
    servoinputs[2] = (int)servo3Characteristic.value();
  }
  if (servo4Characteristic.written())
  {
    servoinputs[3] = (int)servo4Characteristic.value();
  }
}

// when the central disconnects, print it out:

// mise à jour des positions des servos
void majpos()
{
  analogWrite(LEDR, servoinputs[0] * 255 / 180);
  // Serial.println(servoinputs[0]);

  /*for (int i = 0; i < 4; i++)
  {
    Serial.print(servoinputs[i]);
    Serial.print("; ");
  }
  Serial.println();*/
}
