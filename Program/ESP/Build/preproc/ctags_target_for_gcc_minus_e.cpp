# 1 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
# 2 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 3 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 4 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 5 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 6 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 7 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 8 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2
# 9 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 2

// pin and hardware
//{



SoftwareSerial uno(D4, D3); // RX TX

String message = "";
bool messageReady = false;
bool fromSerial = false;
bool unoReady = false;
// }

// Firebase stuff
//{



String WIFI_SSID = "Mas MHD"; // WiFi SSID to which you want NodeMCU to connect
String WIFI_PASSWORD = "anaknyinyul17816"; // Password of your wifi network

FirebaseData FBData;
StaticJsonDocument<800> dataBase;
//}

// LCD stuff
//  {

LiquidCrystal_I2C lcd[2] =
    {LiquidCrystal_I2C(0x23, 16, 2),
     LiquidCrystal_I2C(0x27, 16, 2)};
String lcdText[2][2];
bool lcdUpdate = false;
// }

// Tik stuff goes here
//{

unsigned long int Tik[3];
int Delay[3] =
    {100, 1000, 1000};
//}

void setup()
{
    Serial.begin(57600);
    SPI.begin(); //Firebase kinda need this idk wtf is this
    uno.begin(4800);
    debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 58 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "58" "." "387" "\", \"aSM\", @progbits, 1 #"))) = (
# 58 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           "\nSerial communication started\n"
# 58 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           ); &__pstr__[0];}))
# 58 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           ))));
    /* lcd*/ {
        for (int i = 0; i < 2; i++)
        {
            lcd[i].begin();
            lcd[i].backlight();
        }
    }
    /*connect wifi*/ {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        debug(((reinterpret_cast<const __FlashStringHelper *>(
# 68 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
             (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "68" "." "388" "\", \"aSM\", @progbits, 1 #"))) = (
# 68 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
             "Connecting to "
# 68 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
             ); &__pstr__[0];}))
# 68 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
             ))));
        debugln(WIFI_SSID);

        int manyDot;
        StaticJsonDocument<300> doc;
        String bottomText = WIFI_SSID;
        String LCDmassage[2] =
            {((reinterpret_cast<const __FlashStringHelper *>(
# 75 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "75" "." "389" "\", \"aSM\", @progbits, 1 #"))) = (
# 75 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            "Connecting to"
# 75 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            ); &__pstr__[0];}))
# 75 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            ))), bottomText};
        while (WiFi.status() != WL_CONNECTED)
        {
            changeWifi(doc);
            if (WiFi.status() == WL_WRONG_PASSWORD)
            {
                debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 81 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "81" "." "390" "\", \"aSM\", @progbits, 1 #"))) = (
# 81 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       "Wrong pasword you dummy"
# 81 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       ); &__pstr__[0];}))
# 81 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       ))));
                LCDmassage[0] = ((reinterpret_cast<const __FlashStringHelper *>(
# 82 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "82" "." "391" "\", \"aSM\", @progbits, 1 #"))) = (
# 82 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               "wrong pasword"
# 82 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               ); &__pstr__[0];}))
# 82 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               )));
                LCDmassage[1] = ((reinterpret_cast<const __FlashStringHelper *>(
# 83 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "83" "." "392" "\", \"aSM\", @progbits, 1 #"))) = (
# 83 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               "u fukin moron"
# 83 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               ); &__pstr__[0];}))
# 83 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               )));
                writeBothLCD(LCDmassage, true);
                while (WiFi.status() == WL_WRONG_PASSWORD)
                    changeWifi(doc);
            }
            if (Tik[1] <= millis())
            {
                Tik[1] = millis() + Delay[1];

                debug(((reinterpret_cast<const __FlashStringHelper *>(
# 92 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                     (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "92" "." "393" "\", \"aSM\", @progbits, 1 #"))) = (
# 92 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                     "."
# 92 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                     ); &__pstr__[0];}))
# 92 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                     ))));
                bottomText += ((reinterpret_cast<const __FlashStringHelper *>(
# 93 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                             (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "93" "." "394" "\", \"aSM\", @progbits, 1 #"))) = (
# 93 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                             "."
# 93 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                             ); &__pstr__[0];}))
# 93 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                             )));
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
        LCDmassage[0] = ((reinterpret_cast<const __FlashStringHelper *>(
# 105 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "105" "." "395" "\", \"aSM\", @progbits, 1 #"))) = (
# 105 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       "Connected to"
# 105 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       ); &__pstr__[0];}))
# 105 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       )));
        LCDmassage[1] = WIFI_SSID;
        writeBothLCD(LCDmassage, true);
        delay(1000);
        Firebase.begin("codolos-caa52-default-rtdb.firebaseio.com" /* Your Firebase Project URL goes here without "http:" , "\" and "/"*/, "HIA7h1oGLooWkk9EG1pn7exC4wuOZxX1Pv2HVXTd" /* Your Firebase Database Secret goes here*/);
    }
    /*startComunication*/ {
        0 == 1;
        StaticJsonDocument<200> doc;
        doc[((reinterpret_cast<const __FlashStringHelper *>(
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "114" "." "396" "\", \"aSM\", @progbits, 1 #"))) = (
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           "type"
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           ); &__pstr__[0];}))
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           )))] = ((reinterpret_cast<const __FlashStringHelper *>(
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                        (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "114" "." "397" "\", \"aSM\", @progbits, 1 #"))) = (
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                        "startComunication"
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                        ); &__pstr__[0];}))
# 114 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                        )));
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
            if (doc[((reinterpret_cast<const __FlashStringHelper *>(
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "127" "." "398" "\", \"aSM\", @progbits, 1 #"))) = (
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                   "type"
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                   ); &__pstr__[0];}))
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                   )))] == ((reinterpret_cast<const __FlashStringHelper *>(
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                 (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "127" "." "399" "\", \"aSM\", @progbits, 1 #"))) = (
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                 "response_comunication"
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                 ); &__pstr__[0];}))
# 127 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                 ))) && !unoReady)
            {
                unoReady = true;
                debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 130 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "130" "." "400" "\", \"aSM\", @progbits, 1 #"))) = (
# 130 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       "POG CHAMP, DUDE!!!!"
# 130 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                       ); &__pstr__[0];}))
# 130 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                       ))));
            }
            if (doc[((reinterpret_cast<const __FlashStringHelper *>(
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "132" "." "401" "\", \"aSM\", @progbits, 1 #"))) = (
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                   "type"
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                   ); &__pstr__[0];}))
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                   )))] == ((reinterpret_cast<const __FlashStringHelper *>(
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                 (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "132" "." "402" "\", \"aSM\", @progbits, 1 #"))) = (
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                 "lookingForWemos"
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                 ); &__pstr__[0];}))
# 132 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                 ))))
            {
                StaticJsonDocument<200> docom = doc;
                docom[((reinterpret_cast<const __FlashStringHelper *>(
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                     (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "135" "." "403" "\", \"aSM\", @progbits, 1 #"))) = (
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                     "type"
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                     ); &__pstr__[0];}))
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                     )))] = ((reinterpret_cast<const __FlashStringHelper *>(
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                  (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "135" "." "404" "\", \"aSM\", @progbits, 1 #"))) = (
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                  "startComunication"
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                                  ); &__pstr__[0];}))
# 135 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                                  )));
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
void writeBothLCD(String massage[2], bool writeItNow)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
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
    for (int i = 0; i < 2; i++)
    {
        lcd[i].clear();
        for (int j = 0; j < 2; j++)
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
    if (true)
        Serial.print(massage);
}
void debugln(const String &massage)
{
    if (true)
        Serial.println(massage);
}
void debugJson(StaticJsonDocument<200> &doc)
{
    debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 190 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "190" "." "405" "\", \"aSM\", @progbits, 1 #"))) = (
# 190 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           ""
# 190 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           ); &__pstr__[0];}))
# 190 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           ))));
    if (true && Serial)
        serializeJson(doc, Serial);
}

//}

//Hardware functions
//{
void sendError(const String &massage, bool onlyToSerial)
{
    StaticJsonDocument<200> errorMSG;
    errorMSG[((reinterpret_cast<const __FlashStringHelper *>(
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "202" "." "406" "\", \"aSM\", @progbits, 1 #"))) = (
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            "type"
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            ); &__pstr__[0];}))
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            )))] = ((reinterpret_cast<const __FlashStringHelper *>(
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                         (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "202" "." "407" "\", \"aSM\", @progbits, 1 #"))) = (
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                         "error"
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                         ); &__pstr__[0];}))
# 202 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                         )));
    errorMSG[((reinterpret_cast<const __FlashStringHelper *>(
# 203 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "203" "." "408" "\", \"aSM\", @progbits, 1 #"))) = (
# 203 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            "message"
# 203 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
            ); &__pstr__[0];}))
# 203 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
            )))] = massage;
    if (!onlyToSerial)
        sendJson(errorMSG, false);
    else
        debugJson(errorMSG);
}
void sendJson(StaticJsonDocument<200> &doc, bool fromUno)
{
    debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 211 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "211" "." "409" "\", \"aSM\", @progbits, 1 #"))) = (
# 211 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           ""
# 211 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
           ); &__pstr__[0];}))
# 211 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
           ))));
    serializeJson(doc, uno);
    debugJson(doc);
    if (fromUno)
        debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 215 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "215" "." "410" "\", \"aSM\", @progbits, 1 #"))) = (
# 215 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               "-fromUno"
# 215 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               ); &__pstr__[0];}))
# 215 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               ))));
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
        if (doc[((reinterpret_cast<const __FlashStringHelper *>(
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "253" "." "411" "\", \"aSM\", @progbits, 1 #"))) = (
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               "type"
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               ); &__pstr__[0];}))
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               )))] == ((reinterpret_cast<const __FlashStringHelper *>(
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                             (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "253" "." "412" "\", \"aSM\", @progbits, 1 #"))) = (
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                             "changeWifi"
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                             ); &__pstr__[0];}))
# 253 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                             ))))
        {
            WIFI_SSID = doc[((reinterpret_cast<const __FlashStringHelper *>(
# 255 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                           (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "255" "." "413" "\", \"aSM\", @progbits, 1 #"))) = (
# 255 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                           "wifi_ssid"
# 255 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                           ); &__pstr__[0];}))
# 255 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                           )))].as<String>();
            WIFI_PASSWORD = doc[((reinterpret_cast<const __FlashStringHelper *>(
# 256 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "256" "." "414" "\", \"aSM\", @progbits, 1 #"))) = (
# 256 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               "wifi_password"
# 256 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
                               ); &__pstr__[0];}))
# 256 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
                               )))].as<String>();
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
        debugln(((reinterpret_cast<const __FlashStringHelper *>(
# 271 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "ESP.ino" "." "271" "." "415" "\", \"aSM\", @progbits, 1 #"))) = (
# 271 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               "can't update data"
# 271 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino" 3
               ); &__pstr__[0];}))
# 271 "c:\\Users\\Shaladdin\\GitHub\\Codolos\\Program\\ESP\\ESP.ino"
               ))));
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
