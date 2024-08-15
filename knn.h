#ifndef KNN_H
#define KNN_H

#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <Arduino.h>
#include <set>
#include <unordered_map>

struct Interaction {
    String input;
    String response;
};

extern std::vector<Interaction> interactions;
String insertSpacesAroundPunctuation(const String& text);
double cosineSimilarity(const std::map<String, double>& tfidf1, const std::map<String, double>& tfidf2);
Interaction findBestMatch(const String& input, const std::vector<Interaction>& interactions);
void calculateTFIDF(const std::vector<Interaction>& interactions);
void updateTFIDFForNewInteraction(const Interaction& interaction);
void tokenize(const String& text, std::map<String, int>& freq);
void tokenizeAndStem(const String& text, std::map<String, int>& freq);
String stem(const String& word);
bool isStopWord(const String& word);
String getEquivalentWord(const String& word);
std::vector<String> augmentWithSynonyms(const String& input);

extern std::map<String, int> documentFrequency; 

#endif // KNN_H
