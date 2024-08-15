#include "knn.h"
#include <vector>

std::map<String, std::map<String, double>> documentTFIDF; // Store TF-IDF values for each document
std::map<String, int> documentFrequency; // Define the global variablevariable

// Define a set of stop words
const std::set<String> stopWords = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any", "as", "at", "be", "because",
    "been", "before", "being", "below", "between", "both", "but", "by", "can", "cannot", "could", "did", "do", "does", "doing",
    "down", "during", "each", "few", "for", "from", "further", "had", "has", "have", "having", "he", "her", "here", "hers",
    "him", "his", "i", "if", "in", "into", "is", "it", "its", "it's", "me", "more", "most", "my", "no", "nor", "not",
    "of", "off", "on", "once", "only", "or", "other", "our", "out", "over", "own", "same", "she", "so", "some", "such", "than",
    "that", "the", "their", "theirs", "them", "then", "there", "these", "they", "this", "those", "through", "to", "too", "under",
    "until", "up", "us", "very", "was", "we", "were", "when", "which", "while", "whom", "why", "with",
    "you", "your"
};

// Define a dictionary of synonyms
const std::map<String, String> synonymDictionary = {
    {"favorite", "favourite"},
    {"hello", "hi"},
    {"hello", "hey"},
    {"hi", "hey"},
    {"howdy", "hello"},
    {"morning", "good morning"},
    {"afternoon", "good afternoon"},
    {"evening", "good evening"},
    {"music", "tunes"},
    {"movies", "films"},
    {"books", "literature"},
    {"pet", "animal"},
    {"weather", "climate"},
    {"weekend", "saturday sunday"},
    {"happy", "joyful"},
    {"great", "fine"},
    {"sad", "unhappy"},
    {"eat", "consume"},
    {"drink", "sip"},
    {"travel", "journey"},
    {"fun", "entertainment"},
    {"exercise", "workout"},
    {"sport", "athletics"},
    {"run", "jog"},
    {"walk", "stroll"},
    {"chat", "talk"},
    {"speak", "converse"},
    {"help", "assist"}
};


bool isStopWord(const String& word) {
    return stopWords.find(word) != stopWords.end();
}

String getEquivalentWord(const String& word) {
    auto it = synonymDictionary.find(word);
    if (it != synonymDictionary.end()) {
        return it->second;
    }
    return word;
}

double cosineSimilarity(const std::map<String, double>& tfidf1, const std::map<String, double>& tfidf2) {
    double dotProduct = 0.0;
    double magnitude1 = 0.0, magnitude2 = 0.0;

    for (const auto& kv : tfidf1) {
        if (tfidf2.find(kv.first) != tfidf2.end()) {
            dotProduct += kv.second * tfidf2.at(kv.first);
        }
        magnitude1 += kv.second * kv.second;
    }

    for (const auto& kv : tfidf2) {
        magnitude2 += kv.second * kv.second;
    }

    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0; // Avoid division by zero
    }

    return dotProduct / (sqrt(magnitude1) * sqrt(magnitude2));
}

Interaction findBestMatch(const String& input, const std::vector<Interaction>& interactions) {
    std::map<String, int> termFrequency;
    
    // Normalize the input for matching purposes
    String normalizedInput = insertSpacesAroundPunctuation(input);
    tokenizeAndStem(normalizedInput, termFrequency);

    std::map<String, double> inputTFIDF;
    for (const auto& kv : termFrequency) {
        double tf = kv.second / (double)input.length();
        double idf = log(interactions.size() / (double)documentFrequency[kv.first]);
        inputTFIDF[kv.first] = tf * idf;
    }

    double bestScore = -1.0;
    Interaction bestMatch;
    for (const auto& interaction : interactions) {
        double score = cosineSimilarity(inputTFIDF, documentTFIDF[interaction.input]);
        if (score > bestScore) {
            bestScore = score;
            bestMatch = interaction;
        }
    }
    return bestMatch;
}


void calculateTFIDF(const std::vector<Interaction>& interactions) {
    documentFrequency.clear(); // Clear the global variable before calculation

    // Count document frequency for each term
    for (const auto& interaction : interactions) {
        std::map<String, int> termFrequency;
        tokenizeAndStem(interaction.input, termFrequency);
        for (const auto& kv : termFrequency) {
            documentFrequency[kv.first]++;
        }
    }

    int totalDocuments = interactions.size();
    for (const auto& interaction : interactions) {
        std::map<String, int> termFrequency;
        tokenizeAndStem(interaction.input, termFrequency);

        std::map<String, double> tfidf;
        for (const auto& kv : termFrequency) {
            double tf = kv.second / (double)interaction.input.length();
            double idf = log(totalDocuments / (double)documentFrequency[kv.first]);
            tfidf[kv.first] = tf * idf;
        }
        documentTFIDF[interaction.input] = tfidf;
    }
}


void updateTFIDFForNewInteraction(const Interaction& interaction) {
    std::map<String, int> termFrequency;
    tokenizeAndStem(interaction.input, termFrequency);

    // Update document frequency for new terms
    for (const auto& kv : termFrequency) {
        documentFrequency[kv.first]++;
    }

    int totalDocuments = interactions.size(); // Update total document count
    std::map<String, double> tfidf;

    // Calculate TF-IDF for the new interaction
    for (const auto& kv : termFrequency) {
        double tf = kv.second / (double)interaction.input.length();
        double idf = log(totalDocuments / (double)documentFrequency[kv.first]);
        tfidf[kv.first] = tf * idf;
    }

    // Store the TF-IDF values for this interaction
    documentTFIDF[interaction.input] = tfidf;
}


String stem(const String& word) {
    // Enhanced stemming function: remove common suffixes
    if (word.endsWith("ing")) return word.substring(0, word.length() - 3);
    if (word.endsWith("ed")) return word.substring(0, word.length() - 2);
    if (word.endsWith("es")) return word.substring(0, word.length() - 2);
    if (word.endsWith("s") && word.length() > 1) return word.substring(0, word.length() - 1); // Ensure word length > 1 to avoid removing 's' from words like "is"
    if (word.endsWith("ly")) return word.substring(0, word.length() - 2);
    if (word.endsWith("ment")) return word.substring(0, word.length() - 4);
    if (word.endsWith("ness")) return word.substring(0, word.length() - 4);
    if (word.endsWith("tion")) return word.substring(0, word.length() - 4);
    if (word.endsWith("ions")) return word.substring(0, word.length() - 4);
    if (word.endsWith("al")) return word.substring(0, word.length() - 2);
    if (word.endsWith("er")) return word.substring(0, word.length() - 2);
    if (word.endsWith("est")) return word.substring(0, word.length() - 3);
    if (word.endsWith("ful")) return word.substring(0, word.length() - 3);
    return word;
}

void tokenizeAndStem(const String& text, std::map<String, int>& freq) {
    int start = 0, end = 0;
    while ((end = text.indexOf(' ', start)) != -1) {
        if (end != start) {
            String token = text.substring(start, end);
            token.toLowerCase();
            token.trim();
            token = stem(token);
            token = getEquivalentWord(token);
            if (!isStopWord(token) && token.length() > 0) {
                freq[token]++;
            }
        }
        start = end + 1;
    }
    if (start < text.length()) {
        String token = text.substring(start);
        token.toLowerCase();
        token.trim();
        token = stem(token);
        token = getEquivalentWord(token);
        if (!isStopWord(token) && token.length() > 0) {
            freq[token]++;
        }
    }
}

String joinWords(const std::vector<String>& words) {
    String result = "";
    for (const String& word : words) {
        if (!result.isEmpty()) {
            result += " ";
        }
        result += word;
    }
    return result;
}

std::vector<String> augmentWithSynonyms(const String& input) {
    std::vector<String> augmentedInputs;
    std::vector<String> words;
    int start = 0, end = 0;

    // Split the input into words
    while ((end = input.indexOf(' ', start)) != -1) {
        if (end != start) {
            String word = input.substring(start, end);
            word.trim();
            words.push_back(word);
        }
        start = end + 1;
    }
    if (start < input.length()) {
        String word = input.substring(start);
        word.trim();
        words.push_back(word);
    }

    // Generate all combinations with synonyms
    for (int i = 0; i < words.size(); ++i) {
        String word = words[i];
        String equivalentWord = getEquivalentWord(word);
        if (equivalentWord != word) {
            std::vector<String> newWords = words;
            newWords[i] = equivalentWord;
            String newInput = joinWords(newWords);
            augmentedInputs.push_back(newInput);
        }
    }

    return augmentedInputs;
}
