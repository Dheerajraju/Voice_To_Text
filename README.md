# 🎙️ ESP32-P4 AI Voice Assistant (Voice-to-Text)

An embedded AI Voice-to-Text assistant prototype powered by the **ESP32-P4** microcontroller and the **Waveshare ESP32-P4-WIFI6-Touch-LCD-XC** development board. This project captures spoken audio via onboard microphones, processes it into a PCM WAV file, streams it over Wi-Fi to a local OpenAI Whisper speech recognition server, and renders the transcribed text in real-time on a touch-interactive LVGL display.

---

## ✨ Features
* **Interactive Touch UI:** Built using LVGL v9.5 with real-time status updates (*Ready, Recording, Uploading, Processing*).
* **Audio Acquisition:** Captures 5 seconds of 16-bit Mono PCM audio at 16 kHz using the onboard ES7210 audio codec.
* **Voice Activity Detection (VAD):** Analyzes RMS and Peak amplitudes to detect the presence of speech before processing.
* **Flash & PSRAM Memory Management:** Allocates large audio buffers safely inside 32 MB PSRAM and stores generated `.wav` files into a SPIFFS partition using chunked streaming to prevent display flickering.
* **Wi-Fi 6 Connectivity:** Seamless wireless communication using the ESP-Hosted network architecture.
* **AI Transcription:** Streams multipart audio payloads via HTTP POST to a Whisper speech-to-text API server.

---

## 🛠️ Hardware Specifications
* **Microcontroller:** Espressif ESP32-P4 Dual-core RISC-V (running at 360 MHz)
* **Memory:** 32 MB PSRAM | 16 MB SPI Flash | 768 KB Internal SRAM
* **Display:** Waveshare MIPI Capacitive Touch LCD (JD9365 / GT911 touch controller)
* **Audio Hardware:** Dual digital microphones with ES7210 I2S audio codec

---

## 💻 Software & Toolchain
* **Framework:** ESP-IDF v5.5.4
* **Graphics Library:** LVGL v9.5
* **Board Support Package:** Waveshare ESP32-P4 BSP v2.0.0
* **Backend Server:** Python FastAPI / Uvicorn running `faster-whisper`

---

## 🚀 Getting Started

### 1. Prerequisites
Ensure you have [ESP-IDF v5.x](https://docs.espressif.com/projects/esp-idf/en/latest/esp32p4/get-started/) installed and configured on your system.

### 2. Clone the Repository
```bash
git clone [https://github.com/YourUsername/ESP32-P4-AI-Voice-Assistant.git](https://github.com/YourUsername/ESP32-P4-AI-Voice-Assistant.git)
cd ESP32-P4-AI-Voice-Assistant
