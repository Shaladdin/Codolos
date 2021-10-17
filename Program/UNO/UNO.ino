// Library
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// RFID NERD STUF
//{
#define manyReaders 2
#define resetPin 9

const byte ssPins[manyReaders] = {3, 10};
MFRC522 Reader[manyReaders];

unsigned int currentIDs[manyReaders];
bool onDelay[manyReaders];
//}

// Tik stuff
//  {
unsigned long int Tik_RFID[manyReaders];
const int delay_RFID = 500;
// }

// Hardware stuff
//{
// "C:\\Users\\LENOVO\\Documents\\Arduino\\libraries\\rfid-master\\src",
#define DEBUG true
#define connectionLED 8
const int RFID_LED[2] = {7, 4};
SoftwareSerial wemos(5, 6); // RX TX
String message = "";
bool messageReady = false;
//}

void setup()
{
    Serial.begin(115200);
    wemos.begin(4800);
    debugln(F("Serial communication started"));

    // pin stuff
    //{
    for (int i = 0; i < sizeof(RFID_LED); i++)
        pinMode(RFID_LED[i], OUTPUT);
    pinMode(connectionLED, OUTPUT);
    //}

    // RFID stuff
    //{
    for (int reader = 0; reader < manyReaders; reader++)
        Tik_RFID[reader] = delay_RFID;

    SPI.begin();

    for (uint8_t reader = 0; reader < manyReaders; reader++)
    {
        Reader[reader].PCD_Init(ssPins[reader], resetPin);

        int antennaStrength = Reader[reader].PCD_GetAntennaGain();
        if (antennaStrength < 64)
        {
            debugln("Reader on pin " + String(ssPins[reader]) + F(" is not conected properly"));
        }
        // Dump some information
        debug("Reader #" + String(reader) + F(" initilised on pin ") +
              String(ssPins[reader]) + F(". Antenna strength: ") +
              String(Reader[reader].PCD_GetAntennaGain()) +
              F(". Version ."));
        if (DEBUG)
            Reader[reader].PCD_DumpVersionToSerial();
        delay(100);
    }
    //}
    debugln(F("\n--- END SETUP --"));
    debugln(F("Waiting to connect to wemos..."));
}

void loop()
{
    /*Serial*/ {
        while (Serial.available())
        {
            message = Serial.readString();
            messageReady = true;
        }
        while (wemos.available())
        {
            message = wemos.readString();
            messageReady = true;
        }
        if (messageReady)
        {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, message);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                messageReady = false;
                return;
            }
        }
    } /*RFID*/
    {
        bool cardDetected[manyReaders];
        for (uint8_t reader = 0; reader < manyReaders; reader++)
        {
            Reader[reader].PCD_Init();
            cardDetected[reader] = Reader[reader].PICC_IsNewCardPresent() && Reader[reader].PICC_ReadCardSerial();

            if (!onDelay[reader])
            {
                if (cardDetected[reader])
                {
                    currentIDs[reader] = ReadTag(reader);
                    debugln(String(currentIDs[reader]));
                    blynk(RFID_LED[reader], 3, 500);
                    onDelay[reader] = true;
                }
            }
            else
            {
                if (millis() > Tik_RFID[reader])
                {
                    Tik_RFID[reader] = millis() + delay_RFID;
                    onDelay[reader] = cardDetected[reader];
                }
            }
        }
    }
}

// Debug stuff
//{
void debug(String massage)
{
    if (DEBUG)
        Serial.print(massage);
}
void debugln(String massage)
{
    if (DEBUG)
        Serial.println(massage);
}
// }

// RFID functions
//{
unsigned int ReadTag(int reader)
{
    byte *buffer = Reader[reader].uid.uidByte;
    byte bufferSize = Reader[reader].uid.size;
    String output;
    for (byte i = 0; i < bufferSize; i++)
    {
        output += buffer[i] < 0x10 ? "0" : "";
        output += String(buffer[i]);
    }
    return +output.toInt();
}
// }

// Hardware stuff
//{
void blynk(int LED, int howmany, int inDuration)
{
    bool state = true;
    howmany = howmany * 2;
    for (int i = 0; i < howmany; i++)
    {
        digitalWrite(LED, state);
        state = !state;
        delay(inDuration / howmany);
    }
    digitalWrite(LED, 0);
}
void sendError(String *massage)
{
    StaticJsonDocument<200> errorMSG;
}
// }
// TODO: Make triple check if the card inserted is not the same card that scanned twice in less than a second
// not started yet, but not neded in my opinion

// TODO: Dont ba a dumbass
// on progress, but thats hard, dude