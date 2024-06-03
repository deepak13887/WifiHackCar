#include <ESP8266WiFi.h>

const char *ssid = "ASF-Wifi-Car"; // The name of the Wi-Fi network that will be created
const char *password = "Welcome@User";   // The password required to connect to it, leave blank for an open network

//Motor PINs
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5
 
bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed = 100;

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Setup Wifi server
  delay(10);
  Serial.println('\n');
  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");
  Serial.print("IP address:\t");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);         // Send the IP address of the ESP8266 to the computer

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();
        String response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nAccess-Control-Allow-Origin: *\r\n\r\n";

        if (request.indexOf("/forward") != -1) {
          forward = true;
          backward = false;
          left = false;
          right = false;
          //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nForward Command Received");
          response += "Forward Command Received";
        } else if (request.indexOf("/backward") != -1) {
          forward = false;
          backward = true;
          left = false;
          right = false;
          response += "Backward Command Received";
        } else if (request.indexOf("/left") != -1) {
          forward = false;
          backward = false;
          left = true;
          right = false;
          response += "Left Command Received";
        } else if (request.indexOf("/right") != -1) {
          forward = false;
          backward = false;
          left = false;
          right = true;
          //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nRight Command Received");
          response += "Right Command Received";
        } else if (request.indexOf("/speed") != -1) {
          int index = request.indexOf("/speed") + 7;
          Speed = request.substring(index).toInt();
          response += "Speed Set";
        } else if (request.indexOf("/stop") != -1) {
          forward = false;
          backward = false;
          left = false;
          right = false;
          response += "Stop Command Received";
        }

        smartcar();
        client.print(response);
        client.stop();
      }
    }
  }

}

void smartcar() {
  if (forward) {
    carforward();
    Serial.println("carforward");
  } else if (backward) {
    carbackward();
    Serial.println("carbackward");
  } else if (left) {
    carturnleft();
    Serial.println("carfleft");
  } else if (right) {
    carturnright();
    Serial.println("carright");
  } else {
    carStop();
    Serial.println("carstop");
  }
}

void carforward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}