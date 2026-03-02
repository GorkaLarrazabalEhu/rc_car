#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  // Mi pagina web
#include <Arduino.h>

Servo myservo;  // create servo object to control a servo

// GPIO the servo is attached to
static const int servoPin = 15;

// flashlight
const int Flashlight = 2;

int pos = 90; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment

// wired connections
#define MOTOR_PWM 33 //  Speed
#define MOTOR_DIR 32 //  Direction

// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
#define PWM_SLOW 220 // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

const int freq = 5000;
const int ledChannel_PWM = 5;
const int ledChannel_DIR = 6;
const int resolution = 8;

// SSID y contraseña (longitud minimo 8 caracteres)
const char* ssid = "el_Gorki_carro_xd";
const char* password = "12345678";

WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup...");

  // El servoPin como OUTPUT
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(pos);

  // Los pines como OUTPUT
  pinMode(Flashlight, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  
  // configurar funcionalidades LED PWM y asociar el canal al GPIO a controlar
  ledcAttachChannel(MOTOR_PWM, freq, resolution, ledChannel_PWM);
  ledcAttachChannel(MOTOR_DIR, freq, resolution, ledChannel_DIR);

  ledcWriteChannel(ledChannel_DIR, dir_Forward);
  ledcWriteChannel(ledChannel_PWM, PWM_STOP);

  //Apaga el flashlight
  digitalWrite(Flashlight, LOW); 

  //ESP32 IP: 192.168.4.1
  WiFi.mode(WIFI_AP); // modo Access Point (para conectarse)
  WiFi.softAP(ssid, password);   

  server.on("/", handle_OnConnect);
  server.on("/off", handle_Stop);
  server.on("/FlashLight", handle_Flashlight_Off);
  server.on("/Pos", handle_Steering_Slider_Input);
  server.on("/Drive", handle_Drive_Slider_Input);
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
  String s = MAIN_page; // Leer de index.h el string MAIN_page
  server.send(200, "text/html", s); // Envia la página web al dispositivo que lo solicita
}

void handle_Steering_Slider_Input() {
  if (server.arg("value")){
    int slidervalue = server.arg("value").toInt();
    pos = slidervalue;
    Serial.print("x = ");
    Serial.println(slidervalue);

    // mover el servo a la posición pos
    // myservo.write(slidervalue);
    // pos = slidervalue;
  }
}

void handle_Drive_Slider_Input() {
  if (server.arg("value")){
    int slidervalue = server.arg("value").toInt();
    pos = slidervalue;
    Serial.print("y = ");
    Serial.println(slidervalue);

    // mover el servo a la posición pos
    // myservo.write(slidervalue);
    // pos = slidervalue;
  }
  server.send(200, "text/plain", "ok");
}


void handle_Stop() {
  digitalWrite(Flashlight, LOW);
  // ledcWriteChannel( ledChannel_PWM, PWM_STOP );
  // ledcWriteChannel( ledChannel_DIR, dir_Forward);
  Serial.println("Stopped");
  server.send(200, "text/plain", "ok");
}


void handle_Flashlight_Off() {
  digitalWrite(Flashlight, LOW);
  server.send(200, "text/plain", "ok");
}