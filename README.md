# LVGL Temperature and Humidity Monitor

This project is a temperature and humidity monitor using LVGL (Light and Versatile Graphics Library) with Arduino and a DHT11 sensor, designed to work with the CYD.
Thanks to [witnessmenow](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display?tab=readme-ov-file) for the good Documentation
## Features

- Displays real-time temperature and humidity readings on a cheap yellow TFT display.
- Uses LVGL for graphical user interface elements.
- Updates sensor readings every second.
- Shows live charts for temperature and humidity values.

## Hardware Requirements

- ESP32-Cheap-Yellow-Display
- DHT11 temperature and humidity sensor

## Installation

1. Connect the DHT11 sensor, cheap yellow display following the wiring instructions.
2. Upload the provided Arduino sketch to your Arduino board.
3. Open the Serial Monitor to view temperature and humidity readings.

## Wiring Instructions
Documentation [PINS](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/main/PINS.md)
- DHT11 Sensor:
  - VCC to 5V
  - DATA to pin 27
  - GND to GND

- Cheap Yellow TFT Display:
  - Connect according to the display's wiring instructions.

- XPT2046 Touchscreen Controller:
  - IRQ to pin 36
  - MOSI to pin 32
  - MISO to pin 39
  - CLK to pin 25
  - CS to pin 33

## Usage

- After uploading the sketch, the cheap yellow TFT display will show real-time temperature and humidity readings.
- You can interact with the touchscreen to navigate through the interface.

## Author

- [Cabzla](https://github.com/Cabzla)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
