#include <Arduino.h>
#include "driver/ledc.h"
#include <WiFi.h>

// Motor control pins
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22
#define ENA 5    // PWM for left motor
#define ENB 23   // PWM for right motor

// PWM channels for ENA/ENB
#define ENA_CH 0
#define ENB_CH 1

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // PWM setup: 1kHz, 8-bit
  ledcSetup(ENA_CH, 1000, 8);
  ledcAttachPin(ENA, ENA_CH);

  ledcSetup(ENB_CH, 1000, 8);
  ledcAttachPin(ENB, ENB_CH);

  // Reduced default speed
  ledcWrite(ENA_CH, 130); 
  ledcWrite(ENB_CH, 130); 

  WiFi.softAP("ESP_Car_AP", "12345678");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA_CH, 130);
  ledcWrite(ENB_CH, 130);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(ENA_CH, 130);
  ledcWrite(ENB_CH, 130);
}

// Slower turning (reduced PWM)
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA_CH, 70);
  ledcWrite(ENB_CH, 70);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(ENA_CH, 70);
  ledcWrite(ENB_CH, 70);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/forward") != -1) {
      forward();
    } else if (request.indexOf("/backward") != -1) {
      backward();
    } else if (request.indexOf("/left") != -1) {
      turnLeft();
    } else if (request.indexOf("/right") != -1) {
      turnRight();
    } else if (request.indexOf("/stop") != -1) {
      stopMotors();
    } else if (request.indexOf("/slow") != -1) {
      ledcWrite(ENA_CH, 80);
      ledcWrite(ENB_CH, 80);
    } else if (request.indexOf("/fast") != -1) {
      ledcWrite(ENA_CH, 255);
      ledcWrite(ENB_CH, 255);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<html><body><h1>Command Received</h1></body></html>");

    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}
