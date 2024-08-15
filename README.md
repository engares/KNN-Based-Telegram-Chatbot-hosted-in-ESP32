# KNN-Based-Telegram-Chatbot-hosted-in-ESP32

| Welcome to this small NLP project! This is Charlie, a little chatbot that can be hosted in a microcontroller and utilizes a k-nearest neighbors (k-NN) model along with TF-IDF (Term Frequency-Inverse Document Frequency) to generate intelligent responses. The bot is capable of learning from new data and interacting with users via Telegram. | ![Logo](https://github.com/user-attachments/assets/14cd1647-8e75-4963-bdc9-239a65828505) |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------|

<div align="center">
  <video src="https://github.com/user-attachments/assets/0a645983-165f-4615-9b0f-7f11725a200e" width="400" />
</div>

## Features
- **Natural Interaction on Telegram:** A smooth chat experience with a bot that runs efficiently on low-resource devices.
- **Synonym and Variance Tolerance:** The bot recognizes synonyms and variations, allowing it to understand different ways of asking the same question.
- **Learning Capability:** Users can train the bot with new phrases and responses using simple commands.
- **Highly Customizable with Small Data:** You just have to upload your own training data in a .txt file, more info below.

## How It Works

The bot uses a combination of text processing and machine learning algorithms to generate intelligent responses in real-time. All data is stored in the ESP32's flash memory, making it a self-contained solution. Here’s a summmarised explanation:

- **Text Processing:** The bot first cleans and normalizes user input by breaking down the text into tokens (words) and applying stemming techniques to focus on the root form of each word. The data is also augmented with a dictionary of common synonyms. This ensures that different forms of a word or similar words (e.g., "running" vs. "run", "movies" vs "films") are treated as the same concept.

- **TF-IDF & k-NN Algorithms:** 
  - **TF-IDF (Term Frequency-Inverse Document Frequency):** This algorithm calculates the importance of each word in the user's input relative to the entire knowledge base. Words that are more unique to the input are given higher weights.
  - **k-NN (k-Nearest Neighbors):** The bot then uses the k-NN algorithm to compare the processed input against all stored interactions, calculating the *cosine similarity* between vectors representing these interactions. The bot selects the response corresponding to the most similar previous interaction.

### Implementation of Cosine Similarity

To determine the most relevant response, the bot calculates the cosine similarity between the input vector and each interaction vector in its knowledge base. Cosine similarity measures the angle between two vectors, providing a numerical value that represents their similarity.

**Simple example of Cosine Similarity**
<p align="center">
  <img src="https://github.com/user-attachments/assets/c76f036a-48a3-412c-ab8d-c5bc4d35f176" alt="Two-simplified-documents-are-represented-as-vectors-of-word-count-Their-cosine">
</p>

*Source of the image: Vu, N. Q., & Bezemer, C. (2021). Improving the Discoverability of Indie Games by Leveraging their Similarity to Top-Selling Games. Research Gate. [https://doi.org/10.1145/3472538.3472548](https://doi.org/10.1145/3472538.3472548)*

*Note: The training data used by this bot is synthetic and was generated using a LLM. This allows for a controlled and scalable dataset but may not perfectly replicate all real-world scenarios.*


## Getting Started

### Prerequisites

To get started with this project, you'll need the following:

- **An ESP32 Board**: For this project I used an ESP32-SR-VROOM N16R8 board.
  It should be compatible with other ESP32 boards. I got mine for [~4$ on Aliexpress](https://es.aliexpress.com/item/1005006716318106.html?srcSns=sns_Copy&sourceType=570&spreadType=socialShare&bizType=ProductDetail&social_params=60746885794&aff_fcid=410ea7705d764e55ab1ae1ab0cc71776-1723747881531-02552-_EvqHskt&tt=MG&aff_fsk=_EvqHskt&aff_platform=default&sk=_EvqHskt&aff_trace_key=410ea7705d764e55ab1ae1ab0cc71776-1723747881531-02552-_EvqHskt&shareId=60746885794&businessType=ProductDetail&platform=AE&terminal_id=12ba69b1bfa641daad13ae1e7a9b015b&afSmartRedirect=y&gatewayAdapt=glo2esp)
- **Arduino IDE 1.8.X**: The project requires ***Arduino IDE version 1.8.X.*** Greater versions are NOT compatible with ES32FS Tool.
- **ESP32 Board Support**: [Install the ESP32 board support in Arduino IDE.](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) 
- **ESP32FS Tool**: This project uses the [ESP32FS tool](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload data to the ESP32's SPIFFS (SPI Flash File System). Make sure to install this plugin in your Arduino IDE.

### Setting Up the Project

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/engares/KNN-Based-Telegram-Chatbot-hosted-in-ESP32.git
    cd esp32-chatbot
    ```

2. **Open the Project in Arduino IDE:**
    - Open `main.ino` in the Arduino IDE.

3. **Upload Data to SPIFFS:**
    - If you want to personalize the chatbot responses, prepare the `data` folder with your training data file (`trainingData.txt`) inside it. If not, just leave it like that. 
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

