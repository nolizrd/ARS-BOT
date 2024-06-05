#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "Config.h"
#include "Matrix.h"
#include "HC.h"
#include "Servos.h"


bool awaitingWord = false;

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);


void handleNewMessages(int numNewMessages)
{
    Serial.print("handleNewMessages ");
    Serial.println(numNewMessages);
    
    for (int i = 0; i < numNewMessages; i++)
    {
        String text = bot.messages[i].text;

        if (text == "/hi")
        {
          drawHSmile();
          hiServo();
          bot.sendMessage(chat_id, "hi, smile", "");
        }
        if (text == "/heart")
        {
          drawHeart();
          bot.sendMessage(chat_id, "love", "");
        }
        if (text == "/sad")
        {
          drawSSmile();
          bot.sendMessage(chat_id, "sad", "");
        }
        if (text == "/cat")
        {
          drawCat();
          bot.sendMessage(chat_id, "cat", "");
        }
        if (text == "/dog")
        {
          drawDog();
          bot.sendMessage(chat_id, "dog", "");
        }
        if (text == "/quest")
        {
          questionMark();
          bot.sendMessage(chat_id, "what?", "");
        }
        if (text == "/ex")
        {
          exclamationMark();
          bot.sendMessage(chat_id, "WOW!", "");
        }
        
        if (text == "/message") {
            // Если команда /message, устанавливаем флаг в true
            awaitingWord = true;
            bot.sendMessage(chat_id, "Пожалуйста, отправьте слово для отображения. Слово или предложение должны быть на АНГЛИЙСКОМ ЯЗЫКЕ!", "");
        }else if (awaitingWord == true) {
            // Предполагаем, что следующее сообщение - это слово для отображения
            String wordToDisplay = text;
            displayWord(wordToDisplay);
            awaitingWord = false; // Сброс флага
            bot.sendMessage(chat_id, "Слово отображено на матрице!", "");
        }
        
        if (text == "/help")
        {
            String welcome = "Вот, что я умею!\n";
            welcome += "/hi \n";
            welcome += "/heart \n";
            welcome += "/sad \n";
            welcome += "/message \n";
            welcome += "/cat \n";
            welcome += "/dog \n";
            welcome += "/ex \n";
            welcome += "/quest\n";
            bot.sendMessage(chat_id, welcome, "Markdown");
        }

        // if (text == "/start"){
        //   bot.sendMessage(chat_id, "Привет " + from_name + "! Я твой настольный робот. Готов к работе! Нажмите /help или зайдите в меню", "");
        // }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    Serial.print("Connecting to Wifi SSID ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\nWiFi connected. IP address: ");
    Serial.println(WiFi.localIP());

    bot.sendMessage(chat_id, "Привет! Я твой настольный робот. Готов к работе! Нажмите /help или зайдите в меню", "");

    setupMatrix();
    setupDat();
    setupServos();
}

void loop() {
    if (millis() - bot_lasttime > BOT_MTBS) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages) {
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        bot_lasttime = millis();
    }

    Serial.print(getDistance())

    if (getDistance() < 30 && getDistance() != 0) {
      twoHand();
    }
    
    delay(100);
}