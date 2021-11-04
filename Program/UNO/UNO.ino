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
#define Buzzer 2
const int buzzerNote[] = {1000, 300, 100}; //accept, stratup, deny
const int RFID_LED[2] = {4, 7};
SoftwareSerial wemos(5, 6); // RX TX
bool wemosReady = false;

bool connectionLEDstate;

String message = "";
bool messageReady = false;
bool fromSerial = false;

//}

void setup()
{
    Serial.begin(57600);
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
    //incase the arduino uno restatred alone
    StaticJsonDocument<200> doc;
    doc[F("type")] = F("lookingForWemos");
    sendJson(doc);

    debugln(F("\n--- END SETUP --"));
    debug(F("Waiting wemos to connect to wifi"));
}

void loop()
{
    /*Serial*/ {
        StaticJsonDocument<300> doc;
        readSerial(doc);
        if (doc[F("type")] == F("startComunication") && !wemosReady)
        {
            StaticJsonDocument<200> response;
            wemosReady = true;
            response[F("type")] = F("response_comunication");
            sendJson(response);
            indicate(connectionLED, 3, Buzzer, buzzerNote[1], 500);
        }
        if (doc[F("type")] == F("card_error"))
        {
            int onReader = doc[F("onReader")];
            StaticJsonDocument<200> docom = doc;
            docom.clear();
            docom[F("type")] = F("card_error_response");
            docom[F("onReader")] = onReader;
            sendJson(docom);
            indicate(RFID_LED[onReader], 3, Buzzer, buzzerNote[2], 2000);
        }
        messageReady = false;
    }
    if (!wemosReady)
    {
        if (Tik[0] <= millis())
        {
            Tik[0] = millis() + Delay[0];
            debug(F("."));
            digitalWrite(connectionLED, connectionLEDstate);
            connectionLEDstate = !connectionLEDstate;
        }
        return;
    }
    digitalWrite(connectionLED, HIGH);
    fromSerial = false;
    /*RFID*/
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
                    /*Send the data to wemos*/ {
                        StaticJsonDocument<200> doc;
                        doc[F("type")] = F("SendCardData");
                        doc[F("cardID")] = currentIDs[reader];
                        doc[F("onReader")] = reader;
                        sendJson(doc);
                    }
                    indicate(RFID_LED[reader], 2, Buzzer, buzzerNote[0], 500);
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
void debug(const String &massage)
{
    if (DEBUG)
        Serial.print(massage);
}
void debugln(const String &massage)
{
    if (DEBUG)
        Serial.println(massage);
}
void debugJson(StaticJsonDocument<200> &doc)
{
    if (DEBUG && Serial)
        serializeJson(doc, Serial);
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
        output += buffer[i] < 0x10 ? F("0") : F("");
        output += String(buffer[i]);
    }
    return +output.toInt();
}
// }

// Hardware stuff
//{
void indicate(int LED, int howmany, int buzzer, int frequency, int duration)
{
    howmany = howmany * 2;
    bool state = true;
    for (int i = 0; i < howmany; i++)
    {
        digitalWrite(LED, state);
        if (state)
            tone(buzzer, frequency);
        else
            noTone(buzzer);
        state = !state;
        delay(duration / howmany);
    }
    digitalWrite(LED, LOW);
    noTone(buzzer);
}
void sendError(const String &massage, bool onlyToSerial)
{
    StaticJsonDocument<200> errorMSG;
    errorMSG[F("type")] = F("error");
    errorMSG[F("message")] = massage;
    if (!onlyToSerial)
        sendJson(errorMSG);
    else
        debugJson(errorMSG);
}
void sendJson(StaticJsonDocument<200> &doc)
{
    debugln(F(""));
    serializeJson(doc, wemos);
    debugJson(doc);
}
void readSerial(StaticJsonDocument<300> &doc)
{
    while (Serial.available())
    {
        message = Serial.readString();
        messageReady = true;
        fromSerial = true;
    }
    while (wemos.available())
    {
        message = wemos.readString();
        messageReady = true;
    }

    if (messageReady)
    {
        DeserializationError error = deserializeJson(doc, message);
        if (error != DeserializationError::Ok)
        {
            sendError(error.c_str(), fromSerial);
            messageReady = false;
            fromSerial = false;
            return;
        }
        if (!fromSerial)
        {
            StaticJsonDocument<200> debugDoc = doc;
            debugJson(debugDoc);
        }
    }
}
// }
