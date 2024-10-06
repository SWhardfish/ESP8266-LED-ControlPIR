#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED_PIN D6  // The ESP8266 pin connected to LED
#define SWITCH_PIN D5  // The ESP8266 pin connected to the momentary switch
#define PIR_PIN D7  // The ESP8266 pin connected to PIR sensor

const char *ssid = "<ssid>";     // CHANGE IT
const char *password = "<password>";  // CHANGE IT

ESP8266WebServer server(80); // Web server on port 80

int LED_state = LOW;
int switchState = HIGH;
int lastSwitchState = HIGH;
int pirState = LOW; // Variable to hold the current PIR state
unsigned long lastMotionTime = 0;  // To track when motion was last detected
unsigned long motionTimeout = 5000; // Time in milliseconds to turn off the LED after no movement
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

String getHTML() {
  String html = "<!DOCTYPE HTML>";
  html += "<html>";
  html += "<head>";
  html += "<link rel='icon' href='data:,'>";
  html += "</head>";
  html += "<p>LED state: <span style='color: red;'>";

  if (LED_state == LOW)
    html += "OFF";
  else
    html += "ON";

  html += "</span></p>";
  html += "<a href='/led1/on'>Turn ON</a>";
  html += "<br><br>";
  html += "<a href='/led1/off'>Turn OFF</a>";
  html += "<br><br>";
  html += "<a href='/reboot'>Reboot</a>";
  html += "</html>";

  return html;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Set the switch pin as input with internal pullup resistor
  pinMode(PIR_PIN, INPUT); // Set the PIR pin as input

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  // Print the ESP8266's IP address
  Serial.print("ESP8266 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  // Home page
  server.on("/", HTTP_GET, []() {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /");
    server.send(200, "text/html", getHTML());
  });

  // Route to control the LED
  server.on("/led1/on", HTTP_GET, []() {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /led1/on");
    LED_state = HIGH;
    digitalWrite(LED_PIN, LED_state);
    Serial.println("LED turned ON");
    server.send(200, "text/html", getHTML());
  });
  server.on("/led1/off", HTTP_GET, []() {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /led1/off");
    LED_state = LOW;
    digitalWrite(LED_PIN, LED_state);
    Serial.println("LED turned OFF");
    server.send(200, "text/html", getHTML());
  });

  // Route to reboot the ESP8266
  server.on("/reboot", HTTP_GET, []() {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /reboot");
    server.send(200, "text/html", "<html><body><h1>Rebooting...</h1></body></html>");
    delay(1000);  // Give the web page time to send before rebooting
    ESP.restart();
  });

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read the state of the momentary switch
  int reading = digitalRead(SWITCH_PIN);

  // Check if the switch state has changed
  if (reading != lastSwitchState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the switch state has changed for longer than the debounce delay
    if (reading != switchState) {
      switchState = reading;

      // If the switch is pressed, toggle the LED state
      if (switchState == LOW) {
        LED_state = !LED_state;
        digitalWrite(LED_PIN, LED_state);
        server.send(200, "text/html", getHTML());
      }
    }
  }

  // Update the last switch state
  lastSwitchState = reading;

  // PIR motion detection logic
  int pirReading = digitalRead(PIR_PIN);

  if (pirReading == HIGH) {
    // Motion detected, turn on LED
    LED_state = HIGH;
    digitalWrite(LED_PIN, LED_state);
    lastMotionTime = millis();  // Reset the timer when motion is detected
  }

  // Turn off LED 5 seconds after no motion
  if (LED_state == HIGH && (millis() - lastMotionTime >= motionTimeout)) {
    LED_state = LOW;
    digitalWrite(LED_PIN, LED_state);
  }
}
