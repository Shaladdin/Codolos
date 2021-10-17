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
#define FIREBASE_HOST "project558-default-rtdb.firebaseio.com"   // Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "BOUUYLtfbVJy5zjvU8PCf0BKOJIlDVisgVFvMQKG" // Your Firebase Database Secret goes here

#define WIFI_SSID "Mas MHD"              // WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "anaknyinyul17816" // Password of your wifi network

FirebaseData firebaseData;

//}

// acount stuff
//  {
String *account_name = NULL;
int *account_inside = NULL;
int *account_ID = NULL;
int account_many;
bool search_Found;
// }

// pin and hardware
//{
#define manyDoors 2
#define buzzer D0
#define DEBUG true
SoftwareSerial uno(D4, D3); // RX TX
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
#define TikLength 2
unsigned long int Tik[TikLength];
int Delay[TikLength] =
    {100, 1000};
//}

void setup()
{
    noTone(buzzer);
    Serial.begin(115200);
    SPI.begin();

    // lcd
    for (int i; i < manyDoors; i++)
    {
        lcd[i].begin();
        lcd[i].backlight();
    }

    // Tik
    for (int i; i < TikLength; i++)
        Tik[i] = Delay[i] + millis();

    debugln(F("\nSerial communication started\n"));

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
}

void loop()
{

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

// LC void stuff
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

// account void stuff for
//{
int search(int tag, int *array)
{
    search_Found = false;
    if (array != NULL)
        for (int i = 0; i < sizeof(*array); i++)
            if (array[i] == tag)
            {
                search_Found = true;
                return i;
            }
    return 0;
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
//}