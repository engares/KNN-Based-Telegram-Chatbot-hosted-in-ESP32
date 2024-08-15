# KNN-Based-Telegram-Chatbot-hosted-in-ESP32

Welcome to this small NLP project! This chatbot runs on an ESP32-S3 N16 R8 microcontroller and utilizes a k-nearest neighbors (k-NN) model along with TF-IDF (Term Frequency-Inverse Document Frequency) to generate intelligent  responses. The bot is capable of learning from new data and interacting with users via Telegram.

## Features
- **Natural Interaction on Telegram:** A smooth chat experience with a bot that runs efficiently on low-resource devices.
- **Synonym and Variance Tolerance:** The bot recognizes synonyms and variations, allowing it to understand different ways of asking the same question.
- **Learning Capability:** Users can train the bot with new phrases and responses using simple commands.
- **Highly Customizable with Small Data:** You just have to upload your own training data in a .txt file, more info below.

##  How it works
- **Text Processing:** The bot cleans and normalizes user input by tokenizing the text and applying stemming techniques to focus on word roots.
- **TF-IDF & k-NN:** These algorithms are used to calculate the importance of words and find the most relevant response calculating the *cosine similarity* from the bot's knowledge base.



**Implementation of the Cosine Similarity**
<p align="center">
  <img src="https://github.com/user-attachments/assets/c76f036a-48a3-412c-ab8d-c5bc4d35f176" alt="Two-simplified-documents-are-represented-as-vectors-of-word-count-Their-cosine">
</p>

*Source of the image: Vu, N. Q., & Bezemer, C. (2021). Improving the Discoverability of Indie Games by Leveraging their Similarity to Top-Selling Games. Research Gate. [https://doi.org/10.1145/3472538.3472548](https://doi.org/10.1145/3472538.3472548)*


## Getting Started

### Prerequisites

To get started with this project, you'll need the following:

- **Arduino IDE 1.8.X**: The project requires Arduino IDE version 1.8.X.
- **ESP32 Board Support**: Install the ESP32 board support in Arduino IDE.
- **ESP32FS Tool**: This project uses the [ESP32FS tool](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload data to the ESP32's SPIFFS (SPI Flash File System). Make sure to install this plugin in your Arduino IDE.

### Setting Up the Project

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/your-username/esp32-chatbot.git
    cd esp32-chatbot
    ```

2. **Open the Project in Arduino IDE:**
    - Open `main.ino` in the Arduino IDE.

3. **Upload Data to SPIFFS:**
    - If you want to personalize the chatbort responses, prepare the `data` folder with your training data file (`trainingData.txt`) inside it. If not, just leave it like that. 
    - Make sure it's formated like this:
      ```bash
      # [User message] -> [Bot's response]
    
      # Examples
      Hey! -> Hello! How can I help you today?
      Where is the store located? -> You can find us in 123 Fake Street.
      ...  -> ...
       ```
    - *Note that the extension of the data the ESP32 can handle varies from the memory available on the ESP32 model, the original data contains 17753 characters*
    - Use the "ESP32 Sketch Data Upload" tool to upload this data to the ESP32.

5. **Configure WiFi and Telegram in `main.ino`:**
    - Update the `ssid` and `password` variables with your WiFi credentials 
    - Add your Telegram bot token in the `botToken` variable.

6. **Upload the Code:**
    - Connect your ESP32 to your computer.
    - Select the correct board and port in Arduino IDE. *(Additional configuration may be required, please check a simple guide [here](https://lastminuteengineers.com/esp32-arduino-ide-tutorial/))*
    - Upload the code to your ESP32.

### Using the Bot

- **Start the Bot:** After uploading, the bot will connect to WiFi and be ready to receive messages via Telegram.
- **Just Chat!** 
- **Aditional Commands:**
  - `/start`: See the welcome message.
  - `/help`: Get a list of commands and help information.
  - `/about`: Learn more about how the bot works.
  - `/train input -> response`: Train the bot with new phrases.

### Project Structure

- **`main.ino`**: The main Arduino sketch that initializes the bot and handles communication with Telegram.
- **`knn.h` and `knn.cpp`**: These files contain the k-nearest neighbors logic, TF-IDF calculations, and text processing functions.
- **`data/`**: Contains the training data file (`trainingData.txt`) that is uploaded to SPIFFS.
- **`.ign/`**: Images and stuff for this README.
  
### Contributing
Feel free to open issues or submit pull requests with improvements, bug fixes, or new features :) 

