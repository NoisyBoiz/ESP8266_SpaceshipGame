# SpaceshipGame :rocket::rocket::rocket:

In this game, you will be a ship that has been attacked by aliens. Your mission is to kill the enemy, defeat the last boss, and win. The game uses an ESP8266 bootloader and a 128x64 OLED I2C SSD1306 display.

## Connection Instructions :electric_plug::spiral_notepad:

#### - In this project we’ll use :white_check_mark:
```
- 1 ESP8266-12E (You can use another board that supports the I2C protocol.)
- 1 OLED I2C SSD1306 128x64 pixel resolution
- 3 resistor 1kΩ
- 2 buttons for moving left and right. 1 button for OK and attack
```

#### - to connect ESP8266 with oled SSD1306 :white_check_mark:
```
ESP8266                 SSD1306
3.3V            -->     VCC
GND             -->     GND
GPIO5 (D1)      -->     SCL
GPIO4 (D2)      -->     SDA
```

#### - to connect ESP8266 with button :arrow_backward::arrow_forward::record_button:
- Here, I use the GPIO16 (D0) pin to build the OK button and attack. The GPIO14 (D5) and GPIO15 (D8) pins to make the button move left and right. You can use other pins, but you must check the ESP8266 datasheet to see which pins can be used and then also re-declare the pins in the code.

- The first step in making the buttons is to bring the GPIO16 (D0), GPIO14 (D5), and GPIO15 (D8) pins to a low level by connecting one end of the resistor to the GND pin and the other end to the GPIO16 (D0), GPIO14 (D5), and GPIO15 (D8) pins.
- The second step is connecting one end of the button to the 3.3V pin and the other end to the GPIO16 (D0), GPIO14 (D5), and GPIO15 (D8) pins.


You can see detailed instructions below
<img src="https://cdn.glitch.global/f41a9bd0-8a31-41ac-a400-886f727e1815/1.png?v=1680442654646" alt="Connection Instructions Image" />

## Installing Libraries :books:
#### - After connecting next step is to write the code and upload it to microcontroller using Arduino IDE, So here with OLED display module we need to install few libraries which are required for SSD1306 driver.
```
Adafruit SSD1306
Adafruit GFX Library
```

You can install these libraries from the Arduino IDE itself by simply following the below steps:
```
from the menu bar click on Sketch -> Manage Libraries -> Search for required library in search box -> choose and click on install.
```

#### - Installing the ESP boards :notebook_with_decorative_cover:
To install the ESP8266 board in your Arduino IDE, follow these next instructions:
```
1. In your Arduino IDE, go to File> Preferences.
2. In "Other Boards Manager URLs," type http://arduino.esp8266.com/stable/package esp8266com index.json. After that, click "OK".
3. Open the Boards Manager. Go to Tools > Board > Boards Manager…
4. Search for ESP8266 and click the install button for the "ESP8266 by ESP8266 Community".
5. Everything done. After a few seconds, it should have been installed.
```

## Uploading the code through Arduino IDE :bulb:
After installing the necessary libraries, you can clone my code into your Arduino IDE and upload it to your ESP8266 board.
Everything is done. Good luck.:tada::tada::tada:
