/*
 * ligar pino 3.3v - arduino // check
 * RST - PINO 9 // check
 * GND - arduino // check
 * IRQ - NÃO CONECTADO (INTERRUPÇÃO)
 * MISO - PINO 12 check //check
 * MOSI - PINO 11 check // check
 * SCK - PINO 13 check // check
 * SDA - PINO 10 // check
 * ADICIONAR BBIBLIOTECA .ZIP " RFID-MASTER.ZIP "
 */

//inclusão das bibliotecas necessárias
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9

// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
            
void setup() 
{

  // Inicia a serial
  Serial.begin(9600); // inicia a comunicação entre o arduino e o computador
  
  // Inicia  SPI bus
  SPI.begin(); // inicia a memória interna do arduino

  // Inicia MFRC522
  mfrc522.PCD_Init(); //liga o sensor de rfid
  
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop() 
{
  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  // Mostra UID na serial
  Serial.print("UID da tag :");
  

  byte letra;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }

  
  Serial.println();
  

  delay(1000);
}
