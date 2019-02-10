/*******************************************************************
    Get Project Info on Hackaday.io and display to Serial
    monitor.
 
    By Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

#include <HackadayIoApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses
// NOTE: There is a breaking change in the 6.x.x version,
// install the 5.x.x version instead
// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

#define HACKADAY_IO_API_KEY "1234567890654rfscFfsdfdsffd"
// API Key can be retrieved from here
// https://dev.hackaday.io/applications

// https://hackaday.io/project/163680-hackadayio-api-library-for-arduino
#define HACAKDAY_PROJECT_ID 163680

//------- ---------------------- ------

WiFiClientSecure client;
HackadayIoApi hacakdayIo(client, HACKADAY_IO_API_KEY);

unsigned long delayBetweenRequests = 60000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

void setup()
{

    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was Previously
    // connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);

    // Only avaible in ESP8266 V2.5 RC1 and above
    client.setFingerprint(HACKADAYIO_FINGERPRINT);

    // If you want to enable some extra debugging
    //hacakdayIo._debug = true;
}

void printProjectInfoToSerial(ProjectInfo projectInfo)
{
    if (!projectInfo.error)
    {
        Serial.println("--------- Project Info ---------");

        Serial.print("Id: ");
        Serial.println(projectInfo.id);

        Serial.print("URL: ");
        Serial.println(projectInfo.url);

        Serial.print("Owner ID: ");
        Serial.println(projectInfo.owner_id);

        Serial.print("Name: ");
        Serial.println(projectInfo.name);

        Serial.print("Summary: ");
        Serial.println(projectInfo.summary);

        Serial.print("Image URL: ");
        Serial.println(projectInfo.image_url);

        Serial.print("Views: ");
        Serial.println(projectInfo.views);

        Serial.print("Comments: ");
        Serial.println(projectInfo.comments);

        Serial.print("Followers: ");
        Serial.println(projectInfo.followers);

        Serial.print("Skulls: ");
        Serial.println(projectInfo.skulls);

        Serial.print("Created: ");
        Serial.println(projectInfo.created);

        Serial.print("Updated: ");
        Serial.println(projectInfo.updated);

        Serial.println("------------------------");
    }
}

void loop()
{

    if (millis() > requestDueTime)
    {
        Serial.print("Free Heap: ");
        Serial.println(ESP.getFreeHeap());

        Serial.print("Getting project info for ");
        Serial.println(HACAKDAY_PROJECT_ID);
        ProjectInfo projectInfo = hacakdayIo.getProjectInfo(HACAKDAY_PROJECT_ID);

        printProjectInfoToSerial(projectInfo);

        requestDueTime = millis() + delayBetweenRequests;
    }
}