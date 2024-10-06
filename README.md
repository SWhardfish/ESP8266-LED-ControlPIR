# ESP8266-LED-ControlPIR
Using ESP8266 to control low power LED lights using a PIR
The code include communicating with a PIR HC-SR501 sensor to detect movement to trigger the LED lights. The lights will turn of 5 seconds after no movement is detected.

1. PIR Sensor Pin: Added PIR_PIN connected to D7, and pinMode(PIR_PIN, INPUT) in the setup.
2. Motion Detection Logic: In the loop(), the pirReading checks for motion. If detected, it turns on the LED and resets the lastMotionTime.
3. Turn Off After No Motion: If no motion is detected for more than 5 seconds (motionTimeout), the LED turns off automatically.

The PIR sensor (HC-SR501) has three pins:

VCC (Power)
OUT (Signal output)
GND (Ground)
To connect the PIR sensor to the ESP8266, follow these steps:

1. VCC (Power Pin)
The VCC pin on the PIR sensor needs to be connected to a 5V power supply. If you're using an ESP8266 module like NodeMCU, it typically provides both 3.3V and 5V outputs.
Connect the VCC pin of the PIR sensor to the VIN pin (which provides 5V) on the ESP8266.
Note: If your ESP8266 module only provides a 3.3V pin, you can try powering the PIR sensor with 3.3V. While the HC-SR501 is generally designed to work with 5V, some modules may work with 3.3V, though this is less reliable.

2. OUT (Signal Pin)
The OUT pin on the PIR sensor sends a HIGH signal when motion is detected. This pin is connected to one of the digital input pins of the ESP8266.
In your case, you are using D7 for the PIR sensor signal. So, connect the OUT pin of the PIR sensor to the D7 pin on the ESP8266.
3. GND (Ground Pin)
The GND pin of the PIR sensor must be connected to the GND pin on the ESP8266 to complete the circuit.
Wiring Summary:
VCC (PIR) → VIN (ESP8266 for 5V, or 3.3V pin if necessary).
OUT (PIR) → D7 (ESP8266 digital input).
GND (PIR) → GND (ESP8266).

![image](https://github.com/user-attachments/assets/60380316-2a07-435b-b0d8-5fdb7eeaee13)

Power Considerations:
The PIR sensor typically operates at 5V, but many ESP8266 boards (like NodeMCU) can handle 5V on the VIN pin. If you use 3.3V for the PIR, you might experience some instability, so testing at 5V is recommended.
Final Notes:
If you're using an ESP8266 that only provides 3.3V, you might need to step up the voltage to 5V using a regulator, or use a PIR sensor that works reliably at 3.3V.
The signal pin (OUT) will provide a HIGH signal (3.3V or 5V) when motion is detected, and the ESP8266 can read this to control the LED in your code.

