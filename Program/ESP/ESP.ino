#include <Wire.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

// Firebase stuff
//{
#define FIREBASE_HOST "codolos-caa52-default-rtdb.firebaseio.com" // Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "HIA7h1oGLooWkk9EG1pn7exC4wuOZxX1Pv2HVXTd"  // Your Firebase Database Secret goes here

String WIFI_SSID = "Mas MHD";              // WiFi SSID to which you want NodeMCU to connect
String WIFI_PASSWORD = "anaknyinyul17816"; // Password of your wifi network

FirebaseData FBData;

//}

// acount stuff
//  {
String *account_name = NULL;
int *account_inside = NULL;
int *account_ID = NULL;
int account_many;
bool search_Found;

int SearchHolder;
// }

// pin and hardware
//{
#define manyDoors 2
#define DEBUG true
SoftwareSerial uno(D4, D3); // RX TX

String message = "";
bool messageReady = false;
// }

// LCD stuff
//  {
#define LCD_height 2
LiquidCrystal_I2C lcd[manyDoors] =
    {LiquidCrystal_I2C(0x27, 16, LCD_height),
     LiquidCrystal_I2C(0x23, 16, LCD_height)};
String lcdText[LCD_height][manyDoors];
bool lcdUpdate = false;
// }

// Tik stuff goes here
//{
#define TikLength 3
unsigned long int Tik[TikLength];
int Delay[TikLength] =
    {100, 1000, 1000};
//}

void setup()
{
    Serial.begin(57600);
    SPI.begin();
    uno.begin(4800);
    debugln(F("\nSerial communication started\n"));

    /* lcd*/ {
        for (int i; i < manyDoors; i++)
        {
            lcd[i].begin();
            lcd[i].backlight();
        }
    }
    /* Tik*/ {
        for (int i; i < TikLength; i++)
            Tik[i] = Delay[i] + millis();
    }
    /*connect wifi*/ {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        debug(F("Connecting to "));
        debugln(WIFI_SSID);

        int manyDot;
        String bottomText = WIFI_SSID;
        String LCDmassage[LCD_height] =
            {F("Connecting to"), bottomText};
        while (WiFi.status() != WL_CONNECTED)
        {
            if (Tik[1] <= millis())
            {
                Tik[1] = millis() + Delay[1];
                /*Check serial if change wifi*/ {
                    StaticJsonDocument<200> doc = SerialMassage();
                    if (doc["type"] == "Change_Wifi")
                    {
                        WIFI_SSID = doc["Wifi"].as<String>();
                        WIFI_PASSWORD = doc["Pasword"].as<String>();
                        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
                        //response
                        StaticJsonDocument<200> syncRequest;
                        syncRequest["type"] = "Response_Wifi";
                        sendJson(syncRequest);
                    }
                }
                debug(F("."));
                bottomText += F(".");
                if (manyDot > 3)
                {
                    bottomText = WIFI_SSID;
                    manyDot = 0;
                }
                manyDot++;
                LCDmassage[1] = bottomText;
                writeBothLCD(LCDmassage, true);
            }
        }
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    }
    /*Start Arduino communication*/ {
        ARDUINOJSON_USE_LONG_LONG == 1;
        bool responded = false;
        while (!responded)
        {
            StaticJsonDocument<64> doc = SerialMassage();
            responded = messageReady && doc["type"] == "response_comunication";

            if (Tik[2] <= millis())
            {
                Tik[2] = millis() + Delay[2];
                StaticJsonDocument<200> syncRequest;
                syncRequest["type"] = "startComunication";
                sendJson(syncRequest);
            }
        }
    }
}

void loop()
{
    StaticJsonDocument<100> doc = SerialMassage();
    if (messageReady)
    {
        if (doc["type"] == "SendCardData")
        {
            unsigned int cardID = doc["cardID"].as<unsigned int>();
            int onReader = doc["onReader"].as<int>();
        }
    }

    if (Tik[0] <= millis())
    {
        Tik[0] = millis() + Delay[0];
        if (lcdUpdate)
        {
            UpdateLCD();
            lcdUpdate = false;
        }
    }
}

// LCD void stuff
//  {
void writeBothLCD(String massage[LCD_height], bool writeItNow)
{
    for (int i; i < manyDoors; i++)
        for (int j; j < LCD_height; j++)
            writeLCD(i, massage[j], j, writeItNow);
}
void writeLCD(int Wichlcd, String massage, int atLine, bool writeItNow)
{
    lcdText[atLine][Wichlcd] = massage;
    if (writeItNow)
        UpdateLCD();
    else
        lcdUpdate = true;
}
void UpdateLCD()
{
    for (int i; i < manyDoors; i++)
    {
        lcd[i].clear();
        for (int j; j < LCD_height; j++)
        {
            lcd[i].setCursor(0, j);
            lcd[i].print(lcdText[j][i]);
        }
    }
}
// }

// account and data void stuff
//{
template <typename T>
void DynamicResize(T *&arr, int &currentSize, int newSize, T value_to_fill_the_empety_ones)
{
    if (arr == NULL)
        return;
    T *placeHolder = new T[currentSize];
    for (int i = 0; i < currentSize; i++)
        *(placeHolder + i) = *(arr + i);
    delete[] arr;
    arr = new T[newSize];
    for (int i = 0; i < newSize; i++)
        *(arr + i) = i < currentSize ? *(placeHolder + i) : value_to_fill_the_empety_ones;
    currentSize = newSize;
    delete[] placeHolder;
}
template <typename T>
T DynamicSearch(T LookedFor, T *&array, int &arraySize)
{
    search_Found = false;
    if (array != NULL)
        for (int i = 0; i < arraySize; i++)
            if (array[i] == LookedFor)
            {
                search_Found = true;
                SearchHolder = i;
                return i;
            }
    return 0;
}
int SearchResult()
{
    search_Found = false;
    return SearchHolder;
};
void updateData()
{
    if (Firebase.getArray(FBData, "Account") && FBData.dataType() == "array")
    {
        //TODO
    }
}
void pushData()
{
    FirebaseJsonArray ppl_inside;
    for (int i = 0; i < account_many; i++)
        ppl_inside.add(*(account_inside + i));
}
void pushData_Raw(FirebaseJsonArray &ppl_inside)
{
    if (Firebase.setArray(FBData, "PeopleInside", ppl_inside))
        debugln(F("Sucsesfully Push Data! :D"));
    else
    {
        debugln(F("Failed to PushData, retrying..."));
        pushData_Raw(ppl_inside);
    }
}
//}

// DEBUG stuff
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
void debugJson(StaticJsonDocument<200> &doc)
{
    if (DEBUG && Serial)
        serializeJson(doc, Serial);
}
//}

//Hardware functions
//{
void sendError(String massage)
{
    StaticJsonDocument<200> errorMSG;
    errorMSG["type"] = "error";
    errorMSG["message"] = massage;
    serializeJson(errorMSG, uno);
    debugJson(errorMSG);
}
void sendJson(StaticJsonDocument<200> &doc)
{
    serializeJson(doc, uno);
    debugJson(doc);
}
StaticJsonDocument<300> SerialMassage()
{
    StaticJsonDocument<300> doc;
    while (Serial.available())
    {
        message = Serial.readString();
        messageReady = true;
    }
    while (uno.available())
    {
        message = uno.readString();
        messageReady = true;
    }
    if (messageReady)
    {
        DeserializationError error = deserializeJson(doc, message);
        if (error != DeserializationError::Ok)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            sendError(error.c_str());
            messageReady = false;
        }
    }
    return doc;
}
//}