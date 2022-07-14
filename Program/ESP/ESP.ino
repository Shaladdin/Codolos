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

// temperature sensor stuff
//{
Adafruit_MLX90614 tempSensor = Adafruit_MLX90614();
#define tempIndicator D8
#define maxTemp 37.0f
//}

// Servo stuff
//{
Servo servo[manyDoors][2];
const int servoPins[manyDoors][2] = {{D7, D6}, {D5, D0}};
const int servoDegre[2][2] = {{0, 189}, {0, 180}};
//}

// Firebase stuff
//{
#define FIREBASE_HOST "codolos-caa52-default-rtdb.firebaseio.com" // Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "HIA7h1oGLooWkk9EG1pn7exC4wuOZxX1Pv2HVXTd"  // Your Firebase Database Secret goes here

String WIFI_SSID = "Mas MHD";              // WiFi SSID to which you want NodeMCU to connect
String WIFI_PASSWORD = "anaknyinyul17816"; // Password of your wifi network

FirebaseData FBData;
StaticJsonDocument<800> dataBase;
// StaticJsonDocument<1100> History;
//}

// acount stuff
//  {
#define maxPpl 4
String *account_name = NULL;
unsigned int *account_ID = NULL;
unsigned int *account_inside = NULL;
int manyPeopleInside = 0;
int account_many;

bool search_Found;
int SearchHolder;

int currentVersion[5];       // date, month, year, hour, minute
int VersionOnTheDatabase[5]; // date, month, year, hour, minute

// }

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
#define TikLength 2
unsigned long int Tik[TikLength];
int interval[TikLength] =
    {100, 1000};
//}

void setup()
{
    Serial.begin(57600);
    SPI.begin(); // Firebase kinda need this idk wtf is this
    uno.begin(4800);
    debugln(F("\nSerial communication started\n"));
    tempSensor.begin();
    pinMode(tempIndicator, INPUT);
    /* lcds and servos*/ {
        for (int i = 0; i < manyDoors; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                servo[i][j].attach(servoPins[i][j]);
                servo[i][j].write(0);
            }
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
            // changeWifi(doc);
            if (WiFi.status() == WL_WRONG_PASSWORD)
            {
                debugln(F("Wrong pasword you dummy"));
                LCDmassage[0] = F("wrong pasword");
                LCDmassage[1] = F("u fukin moron");
                writeBothLCD(LCDmassage, true);
                // while (WiFi.status() == WL_WRONG_PASSWORD)
                //     changeWifi(doc);
            }
            if (Tik[1] <= millis())
            {
                Tik[1] = millis() + interval[1];

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
            delay(100); // it work, no thoucy thoucy
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
    getDatabase();
    {
        for (int i = 0; i < manyDoors; i++)
            restartLCD(i, false);
    }
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
                sendJson(docom, false);
            }
            if (doc[F("type")] == F("SendCardData"))
            {
                unsigned int cardID = doc[F("cardID")].as<unsigned int>();
                int onReader = doc[F("onReader")].as<int>();

                writeLCD(onReader, F("Card detected"), 0, false);
                writeLCD(onReader, F("please wait..."), 1, true);
                getDatabase();
                updateData();

                DynamicSearch<unsigned int>(cardID, account_ID, account_many);
                bool notAllowed = !search_Found;
                bool notFound = !search_Found;
                bool resetItself = false;
                bool pushIt = false;
                bool full = manyPeopleInside > maxPpl;
                bool readTemp = false;

                if (search_Found)
                {
                    int IDrequest = SearchResult();
                    DynamicSearch<unsigned int>(cardID, account_inside, manyPeopleInside);

                    bool onEnterence = onReader == enterence;
                    bool alreadyHere = search_Found;
                    readTemp = onEnterence && !alreadyHere && !full;

                    bool writeDaName = false;
                    if (onEnterence)
                    {
                        if (!alreadyHere)
                        {
                            if (!full)
                            {
                                writeLCD(onReader, F("Welcome"), 0, false);
                                writeDaName = true;
                                pushIt = true;
                            }
                            else
                            {
                                notAllowed = true;
                                resetItself = true;
                            }
                        }
                        else
                        {
                            writeLCD(onReader, F("You already in"), 0, false);
                            writeDaName = true;
                        }
                    }
                    else
                    {
                        if (alreadyHere)
                        {
                            Remove<unsigned int>(account_inside, manyPeopleInside, SearchResult());
                            writeLCD(onReader, F("Farewell,"), 0, false);
                            pushIt = true;
                        }
                        else
                            writeLCD(onReader, F("You already out,"), 0, false);
                        writeDaName = true;
                    }

                    if (writeDaName)
                        writeLCD(onReader, *(account_name + IDrequest), 1, true);

                    if (readTemp)
                    {
                        delay(2000);
                        writeLCD(onReader, F("Please measure"), 0, false);
                        writeLCD(onReader, F("your temperature"), 1, true);
                        sendAllow(true, onReader);
                        while (!digitalRead(tempIndicator))
                            delay(10);
                        delay(1000);
                        tempSensor.readObjectTempC(); // ignore this, it work, so stfu
                        float temp = tempSensor.readObjectTempC();
                        if (temp < maxTemp)
                        {
                            debugln(temp);
                            writeLCD(onReader, F("Your temp is ") + String(int(round(temp))), 0, false);
                            writeLCD(onReader, F("you can come in"), 1, true);
                            Resize<unsigned int>(account_inside, manyPeopleInside, manyPeopleInside + 1, *(account_ID + IDrequest));
                        }
                        else
                        {
                            pushIt = false;
                            writeLCD(onReader, F("Your temp is ") + String(int(round(temp))), 0, false);
                            writeLCD(onReader, F("you cant come in"), 1, true);
                        }
                    }
                    if (!resetItself)
                    {
                        delay(3000);
                        restartLCD(onReader, true);
                    }
                    if (pushIt)
                        pushData();
                }

                StaticJsonDocument<200> docom;
                if (notAllowed)
                {
                    docom[F("type")] = F("not_allowed");
                    docom[F("onReader")] = onReader;
                    sendJson(docom, false);
                    delay(1000);
                    if (notFound)
                    {
                        writeLCD(onReader, F("This card is"), 0, false);
                        writeLCD(onReader, F("unrecognized"), 1, true);
                    }
                    else if (full)
                    {
                        writeLCD(onReader, F("Sorry, building"), 0, false);
                        writeLCD(onReader, F("is to full!"), 1, true);
                    }
                }
                else if (!readTemp)
                    sendAllow(readTemp, onReader);
            }
            if (doc[F("type")] == F("not_allowed_response"))
            {
                delay(2000);
                int onReader = doc[F("onReader")];
                restartLCD(onReader, false);
            }
            fromSerial = false;
            messageReady = false;
        }
    }
    LCDmanager();
}

// LCD void stuff
//  {
void restartLCD(int wichlcd, bool now)
{
    writeLCD(wichlcd, F("Put your card"), 0, now);
    writeLCD(wichlcd, F("here"), 1, now);
}
void writeBothLCD(String massage[LCD_height], bool writeItNow)
{
    for (int i = 0; i < manyDoors; i++)
        for (int j = 0; j < LCD_height; j++)
            writeLCD(i, massage[j], j, false);
    if (writeItNow)
        UpdateLCD();
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
void LCDmanager()
{
    if (Tik[0] <= millis())
    {
        Tik[0] = millis() + interval[0];
        if (lcdUpdate)
        {
            UpdateLCD();
            lcdUpdate = false;
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
void debugln()
{
    debugln(F(""));
}
void debugln(const String &massage)
{
    if (DEBUG)
        Serial.println(massage);
}
template <typename T>
void debugln(T massage)
{
    debugln(String(massage));
}
void debugJson(StaticJsonDocument<200> &doc)
{
    debugln(F(""));
    if (DEBUG && Serial)
        serializeJson(doc, Serial);
}
//}

// Hardware functions
//{
void sendAllow(const bool &readTemp, const int &onReader)
{
    StaticJsonDocument<200> doc;
    doc[F("type")] = F("allowed");
    doc[F("onReader")] = onReader;
    doc[F("readTemp")] = readTemp;
    sendJson(doc, false);
}
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
            debugln(F("-fromUno"));
        }
    }
}
// bool changeWifi(StaticJsonDocument<300> &doc)
// {
//     readSerial(doc);
//     if (messageReady)
//     {
//         if (doc[F("type")] == F("changeWifi"))
//         {
//             WIFI_SSID = doc[F("wifi_ssid")].as<String>();
//             WIFI_PASSWORD = doc[F("wifi_password")].as<String>();
//             WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//             return true;
//         }
//     }
//     return false;
// }
// }

// Database functions
//{
template <typename T>
void Reset(T *&arr, int newSize)
{
    delete[] arr;
    arr = new T[newSize];
}
template <typename T>
void Resize(T *&arr, int &currentSize, int newSize, T value_to_fill_the_empety_ones)
{
    if (arr == NULL)
        currentSize = 0;
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
void Remove(T *&arr, int &size, int index)
{
    for (int i = index; i < size; i++)
        *(arr + i) = *(arr + i + 1);
    Resize<T>(arr, size, size - 1, 0);
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
bool getDatabase()
{
    if (!Firebase.get(FBData, "/AccountData"))
    {
        debugln(F("can't update data"));
        debugln(F("why? because ") + FBData.errorReason() + F(" aperently."));
        return false;
    }
    FirebaseJson &json = FBData.jsonObject();
    String str;
    json.toString(str, true);
    DeserializationError err = deserializeJson(dataBase, str);
    if (err != DeserializationError::Ok)
    {
        sendError(err.c_str(), true);
        return false;
    }
    return true;
}
void updateData()
{
    account_many = dataBase[F("Account")][F("ID")].size();
    Reset(account_name, account_many);
    Reset(account_ID, account_many);
    for (int i = 0; i < account_many; i++)
    {
        *(account_name + i) = dataBase[(F("Account"))][F("Name")][i].as<String>();
        *(account_ID + i) = dataBase[(F("Account"))][F("ID")][i].as<unsigned int>();
    }
}
void pushData()
{
    if (Firebase.getInt(FBData, F("/PeopleInside/0")))
    {
        debugln(F("POG"));
        int currentSize = FBData.intData();
        for (int i = currentSize; i > manyPeopleInside; i--)
            Firebase.deleteNode(FBData, F("/PeopleInside/") + String(i));
        for (int i = manyPeopleInside; i > 0; i--)
        {
            if (!Firebase.setInt(FBData, F("/PeopleInside/") + String(i), *(account_inside + i - 1)))
                debugln(F("Error at index") + String(i));
            else
                debugln(*(account_inside + i - 1));
        }
        Firebase.setInt(FBData, F("/PeopleInside/") + String(0), manyPeopleInside);
    }
}
//}