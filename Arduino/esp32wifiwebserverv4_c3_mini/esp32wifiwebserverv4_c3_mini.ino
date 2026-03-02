#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  // Mi pagina web
#include <Arduino.h>

Servo myservo;  // create servo object to control a servo

// GPIO the servo is attached to
// static const int servoPin = 15;
const int ledposx = 0;
const int ledposy = 1;
const int lednegx = 3;
const int lednegy = 2;

// flashlight
const int Flashlight = 8;
bool on_off = false;

int pos = 90; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment

// wired connections
// #define MOTOR_PWM 33 //  Speed
// #define MOTOR_DIR 32 //  Direction

// // the actual values for "fast" and "slow" depend on the motor
// #define dir_Forward 0
// #define dir_Back 255

// #define PWM_STOP 0
// #define PWM_SLOW 220 // arbitrary slow speed PWM duty cycle
// #define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
// #define DIR_DELAY 1000 // brief delay for abrupt motor changes

// const int freq = 5000;
// const int ledChannel_PWM = 5;
// const int ledChannel_DIR = 6;
// const int resolution = 8;

int x = 0;
int y = 0;

// SSID y contraseña (longitud minimo 8 caracteres)
const char* ssid = "el_Gorki_carro_xd_c3";
const char* password = "12345678";

WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup...");

  // El servoPin como OUTPUT
  // pinMode(servoPin, OUTPUT);
  pinMode(ledposx, OUTPUT);
  pinMode(ledposy, OUTPUT);
  pinMode(lednegx, OUTPUT);
  pinMode(lednegy, OUTPUT);
  analogWrite(ledposx, 0);
  analogWrite(ledposy, 0);
  analogWrite(lednegx, 0);
  analogWrite(lednegy, 0);

  // myservo.attach(servoPin);
  // myservo.write(pos);

  // Los pines como OUTPUT
  pinMode(Flashlight, OUTPUT);
  // pinMode(MOTOR_DIR, OUTPUT);
  // pinMode(MOTOR_PWM, OUTPUT);
  
  // configurar funcionalidades LED PWM y asociar el canal al GPIO a controlar
  // ledcAttachChannel(MOTOR_PWM, freq, resolution, ledChannel_PWM);
  // ledcAttachChannel(MOTOR_DIR, freq, resolution, ledChannel_DIR);

  // ledcWriteChannel(ledChannel_DIR, dir_Forward);
  // ledcWriteChannel(ledChannel_PWM, PWM_STOP);

  // //Apaga el flashlight
  digitalWrite(Flashlight, on_off); 

  //ESP32 IP: 192.168.4.1
  WiFi.mode(WIFI_AP); // modo Access Point (para conectarse)
  WiFi.softAP(ssid, password);   

  server.on("/", handle_OnConnect);
  server.on("/off", handle_Stop);
  server.on("/FlashLight", handle_Flashlight_On);
  server.on("/SentDrivingData", handle_Control);
  server.onNotFound(handle_NotFound);

  server.begin(); // Inicializar server
  Serial.println("HTTP server started");
}

void loop(void) {
  // La rutina se ejecuta si se abre la IP en el navegador.
  server.handleClient();
}

void handle_NotFound () {
  Serial.println("lost connection");
}

void handle_OnConnect() {
  Serial.println("Connected");
  String s = MAIN_page; // Leer de index.h el string MAIN_page
  server.send(200, "text/html", s); // Envia la página web al dispositivo que lo solicita
}

void handle_Stop() {
  on_off = !on_off;
  digitalWrite(Flashlight, on_off);
  Serial.println("Stopped");
  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_On() {
  // digitalWrite(Flashlight, HIGH);
  server.send(200, "text/plain", "ok");
}

void handle_Control(){
  if (server.arg("valueX")){
    int x = server.arg("valueX").toInt();
    int y = server.arg("valueY").toInt();
    int xn = server.arg("valueXn").toInt();
    int yn = server.arg("valueYn").toInt();
    Serial.print("x, y = ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    analogWrite(ledposx, x);
    analogWrite(ledposy, y);
    analogWrite(lednegx, xn);
    analogWrite(lednegy, yn);
    }
  server.send(200, "text/plain", "ok");
  }

