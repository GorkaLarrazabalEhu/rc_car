const int ledposx = 32;
const int lednegx = 26;
const int ledposy = 27;
const int lednegy = 33;

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud rate
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
}

void loop() {
  if (Serial.available()) {  // Check if data is available
    int command = Serial.read();  // Read the incoming byte
    if (command == 119){ // w in ascii
        analogWrite(ledposx, 0);
        analogWrite(ledposy, 0);
        analogWrite(lednegx, 0);
        analogWrite(lednegy, 0);
        analogWrite(ledposy, 255);
    }
    else if (command == 115){ // s in ascii
        analogWrite(ledposx, 0);
        analogWrite(ledposy, 0);
        analogWrite(lednegx, 0);
        analogWrite(lednegy, 0);
        analogWrite(lednegy, 255);

    }
    else if (command == 97){ // a in ascii
        analogWrite(ledposx, 0);
        analogWrite(ledposy, 0);
        analogWrite(lednegx, 0);
        analogWrite(lednegy, 0);
        analogWrite(lednegx, 255);

    }
    else if (command == 100){ // d in ascii
        analogWrite(ledposx, 0);
        analogWrite(ledposy, 0);
        analogWrite(lednegx, 0);
        analogWrite(lednegy, 0);
        analogWrite(ledposx, 255);
    }
    else if (command == 113){ // q in ascii
        analogWrite(ledposx, 0);
        analogWrite(ledposy, 0);
        analogWrite(lednegx, 0);
        analogWrite(lednegy, 0);
    }
    

    Serial.print("Received: ");
    Serial.println(command);  // Echo the received command
  }
}
