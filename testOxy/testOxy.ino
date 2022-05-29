
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "405 QUANGMINHHOUSE.VN 0983397152"
#define WIFI_PASSWORD "888888889"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://do-an-9cc98-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "2VOB0uSU0x28eF67d0OUowmxWHtlt0c8XgBCoc7z "

/* 4. Define the user Email and password that alreadey registerd or added in your project */

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;


const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

String chuoi = "90,90", chuoi1, chuoi2;
byte moc;

void setup()
{

    Serial.begin(115200);
    // initialize LCD
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(10000);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = DATABASE_SECRET;
    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Or use legacy authenticate method
    // config.database_url = DATABASE_URL;
    // config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(DATABASE_URL, DATABASE_SECRET);
    // Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);
}

void loop()
{
    if(Serial.available())
   {
        chuoi = Serial.readString();
        Serial.print("test");
        Serial.println(chuoi);
        for (int i = 0; i < chuoi.length(); i++) {
          if (chuoi.charAt(i) == ',') {
              moc = i; //Tìm vị trí của dấu ","
          }
        }
        chuoi1 = chuoi;
        chuoi2 = chuoi;
        chuoi1.remove(moc); //Tách giá trị thanh trượt 1 ra chuoi1
        chuoi2.remove(0, moc + 1); ////Tách giá trị thanh trượt 2 ra chuoi2
        if (Firebase.ready()){
          FirebaseJson json;
         // json.setDoubleDigits(3);
          json.add("heart", chuoi1);
          json.add("oxy", chuoi2);
          Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/deveiceB", &json) ? "ok" : fbdo.errorReason().c_str());
        }
   }
}
