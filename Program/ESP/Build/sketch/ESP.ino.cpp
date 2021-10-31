#include <Arduino.h>
#line 1 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
#include <Wire.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

// pin and hardware
//{
#define manyDoors 2
#define enterence 0
#define DEBUG true
SoftwareSerial uno(D4, D3); // RX TX

String message = "";
bool messageReady = false;
bool fromSerial = false;
bool unoReady = false;
// }

// Firebase stuff
//{
#define FIREBASE_HOST "codolos-caa52-default-rtdb.firebaseio.com" // Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "HIA7h1oGLooWkk9EG1pn7exC4wuOZxX1Pv2HVXTd"  // Your Firebase Database Secret goes here

String WIFI_SSID = "Mas MHD";              // WiFi SSID to which you want NodeMCU to connect
String WIFI_PASSWORD = "anaknyinyul17816"; // Password of your wifi network

FirebaseData FBData;
StaticJsonDocument<800> dataBase;
//}

// LCD stuff
//  {
#define LCD_height 2
LiquidCrystal_I2C lcd[manyDoors] =
    {LiquidCrystal_I2C(0x23, 16, LCD_height),
     LiquidCrystal_I2C(0x27, 16, LCD_height)};
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

#line 53 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void setup();
#line 120 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void loop();
#line 148 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void writeBothLCD(String massage[2], bool writeItNow);
#line 154 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void writeLCD(int Wichlcd, String massage, int atLine, bool writeItNow);
#line 162 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void UpdateLCD();
#line 178 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void debug(const String &massage);
#line 183 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void debugln(const String &massage);
#line 188 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void debugJson(StaticJsonDocument<200> &doc);
#line 199 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void sendError(const String &massage, bool onlyToSerial);
#line 209 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void sendJson(StaticJsonDocument<200> &doc, bool fromUno);
#line 217 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void readSerial(StaticJsonDocument<300> &doc);
#line 248 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
bool changeWifi(StaticJsonDocument<300> &doc);
#line 267 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void updateData();
#line 53 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
void setup()
{
    Serial.begin(57600);
    SPI.begin(); //Firebase kinda need this idk wtf is this
    uno.begin(4800);
    debugln(F("\nSerial communication started\n"));
    /* lcd*/ {
        for (int i = 0; i < manyDoors; i++)
        {
            lcd[i].begin();
            lcd[i].backlight();
        }
    }
    /*connect wifi*/ {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        debug(F("Connecting to "));
        debugln(WIFI_SSID);

        int manyDot;
        StaticJsonDocument<300> doc;
        String bottomText = WIFI_SSID;
        String LCDmassage[LCD_height] =
            {F("Connecting to"), bottomText};
        while (WiFi.status() != WL_CONNECTED)
        {
            changeWifi(doc);
            if (WiFi.status() == WL_WRONG_PASSWORD)
            {
                debugln(F("Wrong pasword you dummy"));
                LCDmassage[0] = F("wrong pasword");
                LCDmassage[1] = F("u fukin moron");
                writeBothLCD(LCDmassage, true);
                while (WiFi.status() == WL_WRONG_PASSWORD)
                    changeWifi(doc);
            }
            if (Tik[1] <= millis())
            {
                Tik[1] = millis() + Delay[1];

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
            delay(100); //it work, no thoucy thoucy
        }
        LCDmassage[0] = F("Connected to");
        LCDmassage[1] = WIFI_SSID;
        writeBothLCD(LCDmassage, true);
        delay(1000);
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    }
    /*startComunication*/ {
        ARDUINOJSON_USE_LONG_LONG == 1;
        StaticJsonDocument<200> doc;
        doc[F("type")] = F("startComunication");
        sendJson(doc, false);
    }
    updateData();
}

void loop()
{
    /*Serial stuff*/ {
        StaticJsonDocument<300> doc;
        readSerial(doc);
        if (messageReady)
        {
            if (doc[F("type")] == F("response_comunication") && !unoReady)
            {
                unoReady = true;
                debugln(F("POG CHAMP, DUDE!!!!"));
            }
            if (doc[F("type")] == F("lookingForWemos"))
            {
                StaticJsonDocument<200> docom = doc;
                docom[F("type")] = F("startComunication");
                sendJson(docom, true);
            }
            fromSerial = false;
            messageReady = false;
        }
    }
    while (!unoReady)
        return;
}

// LCD void stuff
//  {
void writeBothLCD(String massage[LCD_height], bool writeItNow)
{
    for (int i = 0; i < manyDoors; i++)
        for (int j = 0; j < LCD_height; j++)
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
    for (int i = 0; i < manyDoors; i++)
    {
        lcd[i].clear();
        for (int j = 0; j < LCD_height; j++)
        {
            lcd[i].setCursor(0, j);
            lcd[i].print(lcdText[j][i]);
        }
    }
}
// }

// DEBUG stuff
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
    debugln(F(""));
    if (DEBUG && Serial)
        serializeJson(doc, Serial);
}

//}

//Hardware functions
//{
void sendError(const String &massage, bool onlyToSerial)
{
    StaticJsonDocument<200> errorMSG;
    errorMSG[F("type")] = F("error");
    errorMSG[F("message")] = massage;
    if (!onlyToSerial)
        sendJson(errorMSG, false);
    else
        debugJson(errorMSG);
}
void sendJson(StaticJsonDocument<200> &doc, bool fromUno)
{
    debugln(F(""));
    serializeJson(doc, uno);
    debugJson(doc);
    if (fromUno)
        debugln(F("-fromUno"));
}
void readSerial(StaticJsonDocument<300> &doc)
{
    while (Serial.available())
    {
        message = Serial.readString();
        messageReady = true;
        fromSerial = true;
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
bool changeWifi(StaticJsonDocument<300> &doc)
{
    readSerial(doc);
    if (messageReady)
    {
        if (doc[F("type")] == F("changeWifi"))
        {
            WIFI_SSID = doc[F("wifi_ssid")].as<String>();
            WIFI_PASSWORD = doc[F("wifi_password")].as<String>();
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            return true;
        }
    }
    return false;
}
//}

//Database functions
//{
void updateData()
{
    if (!Firebase.get(FBData, "/data"))
    {
        debugln(F("can't update data"));
        debugln("why? because " + FBData.errorReason());
        return;
    }
    FirebaseJson &json = FBData.jsonObject();
    String str;
    json.toString(str, true);
    DeserializationError err = deserializeJson(dataBase, str);
    if (err != DeserializationError::Ok)
        sendError(err.c_str(), true);
}
//}
