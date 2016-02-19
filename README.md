# Edison-OLED-Wifi-Scanner
This app scans all the nearby available WiFi access points, gets the data for them, and displays it on the screen. 

#Installation

Either you can download this whole repository and compile it from source in the Intel IoT version of Eclipse, or you can download the compiled program with wget on the Edison

`wget icodethings.info/uploads/Wifi_Scanner.o`

And then run it with

`./Wifi_Scanner.o`

# Usage

This is intended for use with the Intel Edison and the Sparkfun OLED block (https://www.sparkfun.com/products/13035), although I daresay it could easily be adapted to run on other Linux-based SoCs, and to work with other displays. 

Once you run the program it will take a few seconds to scan the nearby accesses points, and then it will display them on the screen. You can use up A and B to scroll through the scanned WiFi access points, and joystick select to re-scan. 

#Credits

The contents of the sparkfun folder is created by Sparkfun, with a few modifications by me to fix some errors. 

Original libraries from Sparkfun can be found here: https://github.com/sparkfun/Edison_OLED_Block/tree/master/Firmware/pong
