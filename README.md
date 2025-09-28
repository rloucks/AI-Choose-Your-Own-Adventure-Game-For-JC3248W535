# ESP32 AI Choose Your Own Adventure Game for the JC3248W535

> Interactive text-based adventure game powered by local AI running on ESP32 with touchscreen display.

## Features

- **26 Genres**: Horror, Sci-Fi, Fantasy, Mystery, Thriller, Adventure, and 20+ more
- **30 Topics**: Dark, Crime, Survival, Time Travel, Magic, Prophecy, and more
- **Local AI Processing**: Uses LM Studio for privacy and offline capability
- **Dynamic Story Generation**: Every playthrough is unique
- **Touchscreen UI**: Beautiful LVGL-based interface with custom icons
- **Conversation Memory**: AI remembers your choices throughout the game
- **Game Over Detection**: Automatic ending recognition
- **Restart Anytime**: Tap the icon to restart with new genre/topic

## Dependencies

### Arduino Libraries
```
- chinScreen (display & UI library) https://github.com/rloucks/chinScreen
- WiFi
- HTTPClient
- ArduinoJson (v6+)
```

### Server Requirements
- **LM Studio** - Local AI model server
- Compatible language model (Mistral, LLaMA, etc.)
- Local network connection

## Installation

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/esp32-cyoa-game.git
cd esp32-cyoa-game
```

### 2. Configure Settings

Edit the configuration section in `cyoa-local.ino`:

```cpp
// WiFi Configuration
const char* WIFI_SSID = "YourNetwork";
const char* WIFI_PASSWORD = "YourPassword";

// LM Studio Configuration
const char* LM_STUDIO_HOST = "192.168.1.100";  // Your PC's local IP
const int LM_STUDIO_PORT = 1234;                // Default LM Studio port
```

### 3. Set Up LM Studio

#### Install LM Studio
1. Download from [lmstudio.ai](https://lmstudio.ai/)
2. Install and launch the application
3. Download a compatible model (recommended: Mistral 7B, LLaMA 2 7B, or similar)

#### Start the Server
1. Open LM Studio
2. Load your chosen model
3. Click "Local Server" tab
4. Click "Start Server"
5. Note the port (default: 1234)
6. Ensure "CORS" is enabled for network access

#### Verify Server
```bash
# Test if server is running
curl http://localhost:1234/v1/models

# Should return list of available models
```

### 4. Upload to ESP32

1. Open `cyoa-local.ino` in Arduino IDE
2. Install required libraries via Library Manager
3. Select your ESP32 board and port
4. Click Upload

## Usage

### Starting a Game

1. **Power On**: Device connects to WiFi and tests LM Studio connection
2. **Choose Genre**: Select from 5 randomly presented genres
3. **Choose Topic**: Select from 5 randomly presented topics
4. **Play**: Read story and choose from 3 options
5. **Continue**: Story adapts based on your choices

### Controls

- **Tap Option Buttons**: Make story choices (A/B/C)
- **Tap Icon**: Restart game with new selections (when visible)
- **Auto Restart**: Game over screen allows fresh start

### Game Flow

```
Loading Screen
     ↓
Genre Selection (5 random options)
     ↓
Topic Selection (5 random options)
     ↓
Story Generation
     ↓
Story Display + 3 Choices
     ↓
Choice Made → New Story Segment
     ↓
Repeat until Game Over
```

## Configuration Options

### LM Studio Settings
```cpp
const char* LM_STUDIO_MODEL = "local-model";  // Model identifier
```

### AI Parameters (in code)
```cpp
requestDoc["temperature"] = 0.8;   // Creativity (0.0-2.0)
requestDoc["max_tokens"] = 500;    // Response length
```

### Timeout Settings
```cpp
http.setTimeout(30000);  // 30 seconds for AI processing
```
## Advanced Features

### Conversation Memory

The game maintains context across choices:
```cpp
conversation_history = content;  // Stores AI's last response
```

This allows the AI to remember previous events and create coherent narratives.

### Game Over Detection

Automatic ending when all options contain "GAMEOVER":
```cpp
bool isGameOver() {
  return (optA.indexOf("gameover") != -1 && 
          optB.indexOf("gameover") != -1 && 
          optC.indexOf("gameover") != -1);
}
```

### Fallback JSON Parser

Handles malformed AI responses:
```cpp
void parseGameResponseFallback(String response)
```

Extracts story and options even when JSON is imperfect.

## Customization

### Add New Genres
```cpp
String genres[] = {
  "Horror", "Sci-Fi", /* ... */
  "Your New Genre"  // Add here
};
```

### Add New Topics
```cpp
String topics[] = {
  "Dark", "Crime", /* ... */
  "Your New Topic"  // Add here
};
```

### Modify AI Prompt
```cpp
String prompt = "You are a choose-your-own-adventure game master...";
// Customize system instructions here
```

### Change Display Layout
```cpp
// Adjust button positions (320x480 display)
lv_obj_set_pos(optionAButton, 0, 80);   // Y position
lv_obj_set_pos(optionBButton, 0, 130);
lv_obj_set_pos(optionCButton, 0, 180);
```

## Acknowledgments

- [LM Studio](https://lmstudio.ai/) - Local AI inference platform
- [LVGL](https://lvgl.io/) - Embedded graphics library
- chinScreen - ESP32 display wrapper library
- Open source AI models community


## Changelog

### v1.0.0
- Initial release
- 26 genres, 30 topics
- LM Studio integration
- Touchscreen UI
- Conversation memory
- Fallback JSON parsing

---

**Built with 🎮 and 🤖 for endless adventures**
