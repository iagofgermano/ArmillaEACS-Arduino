#include <MFRC522.h>
#include <MFRC522Extended.h>

#include <SPI.h>
#include <WiFi.h>

#define SS_PIN 21
#define RST_PIN 22

const char* ssid     = "my-ssid";
const char* password = "password";

const char* host = "website-domain";

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup()
{

    // We start by connecting to a WiFi network

    SPI.begin(); // inicia a memória interna do arduino

  // Inicia MFRC522
    mfrc522.PCD_Init(); //liga o sensor de rfid

    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
    delay(5000);

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }
    

  String tag = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    tag.concat(String(mfrc522.uid.uidByte[i]<0x10 ? " 0" : "%20"));
    tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

    tag.toUpperCase();
    tag.remove(0,3);

    // We now create a URI for the request
    String url = "/api/tag";
    url += "?tag=";
    url += tag;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}