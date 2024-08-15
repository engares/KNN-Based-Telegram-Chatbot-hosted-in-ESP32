# KNN-Based-Telegram-Chatbot-hosted-in-ESP32

Welcome to the ESP32-based chatbot project! This chatbot runs on an ESP32-S3 N16 R8 microcontroller and utilizes a k-nearest neighbors (k-NN) model along with TF-IDF (Term Frequency-Inverse Document Frequency) to generate intelligent  responses. The bot is capable of learning from new data and interacting with users via Telegram.

## Features

- **Text Processing:** The bot cleans and normalizes user input by tokenizing the text and applying stemming techniques to focus on word roots.
- **Synonym Matching:** The bot recognizes synonyms, allowing it to understand different ways of asking the same question.
- **TF-IDF & k-NN:** These algorithms are used to calculate the importance of words and find the most relevant response calculating the cosine similarity from the bot's knowledge base.
- **Learning Capability:** Users can train the bot with new phrases and responses using simple commands.
- **Open Source:** The code is open for exploration and contribution, available in this repository.

- Vu, N. Q., & Bezemer, C. (2021). Improving the Discoverability of Indie Games by Leveraging their Similarity to Top-Selling Games. Research Gate. https://doi.org/10.1145/3472538.3472548

## Getting Started

### Prerequisites

To get started with this project, you'll need the following:

- **Arduino IDE 1.8.X**: The project requires Arduino IDE version 1.8.X.
- **ESP32 Board Support**: Install the ESP32 board support in Arduino IDE.
- **ESP32FS Tool**: This project uses the [ESP32FS tool](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload data to the ESP32's SPIFFS (SPI Flash File System). Make sure to install this plugin in your Arduino IDE.

### Installing the ESP32FS Tool

1. Download the ESP32FS tool from the [GitHub repository](https://github.com/me-no-dev/arduino-esp32fs-plugin).
2. Navigate to your Arduino IDE installation directory.
3. Place the `ESP32FS` tool in the `tools` directory under `Arduino IDE`.
4. Restart the Arduino IDE. You should now see an option for "ESP32 Sketch Data Upload" under the Tools menu when an ESP32 board is selected.

### Setting Up the Project

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/your-username/esp32-chatbot.git
    cd esp32-chatbot
    ```

2. **Open the Project in Arduino IDE:**
    - Open `knn_chat_bot_v11.ino` in the Arduino IDE.

3. **Upload Data to SPIFFS:**
    - Prepare the `data` folder with your training data file (`trainingData.txt`) inside it.
    - Use the "ESP32 Sketch Data Upload" tool to upload this data to the ESP32.

4. **Configure WiFi and Telegram:**
    - Update the `ssid` and `password` variables with your WiFi credentials.
    - Add your Telegram bot token in the `botToken` variable.

5. **Upload the Code:**
    - Connect your ESP32 to your computer.
    - Select the correct board and port in Arduino IDE.
    - Upload the code to your ESP32.

### Using the Bot

- **Start the Bot:** After uploading, the bot will connect to WiFi and be ready to receive messages via Telegram.
- **Commands:**
  - `/start`: See the welcome message.
  - `/help`: Get a list of commands and help information.
  - `/about`: Learn more about how the bot works.
  - `/train input -> response`: Train the bot with new phrases.

### Project Structure

- **`knn_chat_bot_v11.ino`**: The main Arduino sketch that initializes the bot and handles communication with Telegram.
- **`knn.h` and `knn.cpp`**: These files contain the k-nearest neighbors logic, TF-IDF calculations, and text processing functions.
- **`data/`**: Contains the training data file (`trainingData.txt`) that is uploaded to SPIFFS.

### How the Bot Works

- **Text Processing:** The bot normalizes and tokenizes user input, reducing words to their stems to understand the core meaning.
- **TF-IDF Calculation:** TF-IDF values are computed to weigh the importance of words in the context of the entire knowledge base.
- **k-NN Matching:** The bot uses k-nearest neighbors to find the most similar previous interaction based on TF-IDF scores.
- **Learning New Data:** Users can teach the bot new interactions using the `/train` command, which updates the bot’s knowledge base dynamically.

### Contributing

Contributions are welcome! Feel free to open issues or submit pull requests with improvements, bug fixes, or new features.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Acknowledgments

- Special thanks to [me-no-dev](https://github.com/me-no-dev) for the ESP32FS tool.
- Thanks to the Arduino and ESP32 communities for their support and resources.
