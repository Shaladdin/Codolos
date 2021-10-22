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
#define TikLength 1
unsigned long int Tik[TikLength];
int Delay[TikLength] =
    {1000};

unsigned long int Tik_RFID[manyReaders];
const int delay_RFID = 500;
// }

// Hardware stuff
//{
// "C:\\Users\\LENOVO\\Documents\\Arduino\\libraries\\rfid-master\\src",
#define DEBUG true
#define connectionLED 8
#define Buzzer 3
const int RFID_LED[2] = {7, 4};
SoftwareSerial wemos(5, 6); // RX TX
bool wemosReady = false;

bool connectionLEDstate;

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
            debugln("Reader on pin " +
                    String(ssPins[reader]) +
                    F(" is not conected properly"));
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
    debug(F("Waiting to connect to wemos..."));
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
            StaticJsonDocument<300> doc;
            DeserializationError error = deserializeJson(doc, message);
            if (error != DeserializationError::Ok)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                sendError(error.c_str());
                messageReady = false;
                return;
            }
            if (doc["type"] == "startComunication")
            {
                StaticJsonDocument<200> response;
                wemosReady = true;
                response["type"] = "response_comunication";
                sendJson(&response);
            }
        }
        if (!wemosReady)
        {
            if (Tik[0] <= millis())
            {
                Tik[0] = millis() + Delay[0];
                debug(".");
                digitalWrite(connectionLED, connectionLEDstate);
                connectionLEDstate = !connectionLEDstate;
            }
            return;
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
                    /*Send the data to wemos*/ {
                        StaticJsonDocument<200> doc;
                        doc["type"] = "SendCardData";
                        doc["cardID"] = currentIDs[reader];
                        doc["onReader"] = reader;
                        sendJson(&doc);
                    }
                    indicate(RFID_LED[reader], 3, Buzzer, 2, 600, 500);
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
void debugJson(StaticJsonDocument<200> *doc)
{
    if (DEBUG && Serial)
        serializeJson(*doc, Serial);
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
void indicate(int LED, int howmany_LED, int buzzer, int howmany_buzzer, int frequency, int inDuration)
{
    bool state[2] = {true, true};
    int howmany[2] = {
        howmany_LED * 2,
        howmany_buzzer * 2,
    };

    int time = millis() + inDuration;
    unsigned int Tik_indicator[2];
    while (time > millis())
    {
        for (int i; i < 2; i++)
        {
            if (Tik_indicator[i] <= millis())
            {
                Tik_indicator[i] = millis() + inDuration / howmany[i];
                if (i = 0)
                    digitalWrite(LED, state[0]);
                else if (state[0])
                    tone(buzzer, frequency);
                else
                    noTone(buzzer);
                state[i] = !state[i];
            }
        }
    }
    noTone(buzzer);
    digitalWrite(LED, 0);
}
void sendError(String massage)
{
    StaticJsonDocument<200> errorMSG;
    errorMSG["type"] = "error";
    errorMSG["message"] = massage;
    sendJson(&errorMSG);
}
void sendJson(StaticJsonDocument<200> *doc)
{
    serializeJson(*doc, wemos);
    debugJson(doc);
}
// }
// TODO: Make triple check if the card inserted is not the same card that scanned twice in less than a second
// not started yet, but not neded anyway in my opinion

// TODO: Dont ba a dumbass
// on progress, but thats hard, dude