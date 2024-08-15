#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "knn.h"  
#include "FS.h"
#include "SPIFFS.h"
#include <map>
#include <vector>  

// *** WiFi & Telegram credentials ***

// Replace with your network credentials
const char* ssid = "**********************";  // 
const char* password = "******************";  // 

// Replace with your bot token
const char* botToken = "*******************************************";

// Time between reponses
const int botRequestDelay = 1000;

// ***********************************************************************


WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
unsigned long lastTimeBotRan = 0;
std::vector<Interaction> interactions;

void connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected successfully.");
}

void initializeSPIFFS() {
    Serial.println("Initializing SPIFFS...");
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to format/mount SPIFFS");
        return;
    }
    Serial.println("SPIFFS formatted and mounted successfully");
}

void loadTrainingData() {
    Serial.println("Opening training file...");
    File file = SPIFFS.open("/trainingData.txt", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
            processTrainingLine(line);
        }
    }
    file.close();
    Serial.println("Training data loaded and file closed successfully.");
    calculateTFIDF(interactions);  // Calculate TF-IDF after loading training data
}

String insertSpacesAroundPunctuation(const String& text) {
    String result = text;
    const char* punctuations = ".,!?";
    for (size_t i = 0; i < strlen(punctuations); i++) {
        result.replace(String(punctuations[i]), " " + String(punctuations[i]) + " ");
    }
    result.trim(); // Remove any leading or trailing spaces
    return result;
}

void processTrainingLine(const String& line) {
    int delimiterIndex = line.indexOf("->");
    if (delimiterIndex == -1) return;

    String input = line.substring(0, delimiterIndex);
    String response = line.substring(delimiterIndex + 2);
    input.trim();
    response.trim();

    // Insert spaces around punctuation ONLY for the input
    input = insertSpacesAroundPunctuation(input);

    // Add original interaction
    interactions.push_back({input, response});

    // Add interactions with synonyms
    std::vector<String> augmentedInputs = augmentWithSynonyms(input);
    for (const String& augmentedInput : augmentedInputs) {
        interactions.push_back({augmentedInput, response});
    }
}


String welcomeMessage() {
    return "Hello! 👋 I am Charlie, your Telegram ChatBot.\n\n"
           "I generate responses using a k-nearest neighbors-based model.\n\n"
           "Send me a message and I'll try to respond based on what I've learned.\n"
           "If you need help, type '/help'.";
}


String aboutMessage() {
    return "🤖 *About This Bot* 🤖\n\n"
           "This chatbot is powered by an ESP32 microcontroller and uses a k-nearest neighbors (k-NN) model "
           "with TF-IDF (Term Frequency-Inverse Document Frequency) to generate responses.\n\n"
           "*How it works:*\n"
           "1. **Text Processing:** Your message is cleaned, tokenized, and reduced to its root form.\n"
           "2. **Synonym Matching:** The bot recognizes different ways of asking the same question.\n"
           "3. **TF-IDF & k-NN:** The bot calculates the importance of words using cosine similarity and finds the closest match in its knowledge base.\n"
           "4. **Response Generation:** Based on the match, the bot generates an appropriate response.\n\n"
           "*Learning:* You can train the bot with new phrases using `/train input -> response`.\n\n"
           "*Open Source:* The code for this bot is available on GitHub. Check it out here!: [GitHub Repository](https://github.com/engares/KNN-Based-Telegram-Chatbot-hosted-in-ESP32)\n\n"           
           "For help, type `/help` or see this message again with `/about`.";
}


String helpMessage() {
    return "🆘 Help Menu 🆘\n\n"
           "Here are some things you can do:\n\n"
           "*1. Ask me anything* - Just type your question and I'll find the best answer I know.\n\n"
           "*2. Special Commands*\n"
           "  - /start to see the welcome message again.\n"
           "  - /help to view this help message.\n"
           "  - /train to teach new responses to the bot :) \n "
           "  - /about to access the _source code_ and to know more about how I work ";
}

void saveTrainingData(const String& trainingData) {
    File file = SPIFFS.open("/trainingData.txt", "a");
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    file.println(trainingData);
    file.close();
    Serial.println("Training data saved successfully.");
}

void handleNewMessages(int numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String user_text = bot.messages[i].text;

        // Normalize the text (for input matching only)
        String text = insertSpacesAroundPunctuation(user_text);
        text.toLowerCase(); // Convert text to lower case in-place

        // Print normalized text to serial monitor
        Serial.print("Normalized user input: ");
        Serial.println(text);

        if (text.startsWith("/train")) {
            if (text.length() == 6 || text.length() == 7) {  // "/train" command without additional text
                bot.sendMessage(chat_id, "To train me, please use the following format:\n/train input -> response\nFor example:\n/train hello -> Hi there! How can I help you?", "");
            } else {
                String trainingData = text.substring(7);
                processTrainingLine(trainingData);
                saveTrainingData(trainingData);
                
                // Get the new interaction created in processTrainingLine
                Interaction newInteraction = interactions.back(); 

                // Update TF-IDF incrementally for the new interaction
                updateTFIDFForNewInteraction(newInteraction);

                bot.sendMessage(chat_id, "Training data added successfully!", "");
            }
        } else if (text == "/start") {
            bot.sendMessage(chat_id, welcomeMessage(), "Markdown"); 
        } else if (text == "/help") {
            bot.sendMessage(chat_id, helpMessage(), "Markdown");
        } else if (text == "/about") {
            bot.sendMessage(chat_id, aboutMessage(), "Markdown");
        } else {
            Interaction bestMatch = findBestMatch(text, interactions);
            String response = bestMatch.response;

            // Print the best match and response to serial monitor
            Serial.print("Best match: ");
            Serial.println(bestMatch.input);

            // Print the final response to serial monitor
            Serial.print("Final response: ");
            Serial.println(response);

            bot.sendMessage(chat_id, response, "");
        }
    }
}


void setup() {
    Serial.begin(115200);
    connectToWiFi();
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Ensure this is defined correctly
    initializeSPIFFS();
    loadTrainingData(); // Load existing training data
    calculateTFIDF(interactions);  // Calculate TF-IDF for all interactions after loading data
}


void loop() {
    if (millis() - lastTimeBotRan > botRequestDelay) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        handleNewMessages(numNewMessages);
        lastTimeBotRan = millis();
    }
}
