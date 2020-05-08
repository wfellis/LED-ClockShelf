# LED-ClockShelf

This is the software of a variation of the LED clock from [here.](https://www.instructables.com/id/How-to-Build-a-Giant-Hidden-Shelf-Edge-Clock/)
With some reconfiguration it should be possible to use this code also for the original version.

I decided to code it completley from scratch at I wanted to use a ESP32 instead of the Arduino nano and RTC that is used in the original project.
This enables some cool features like smartphone app control, OTA updates and fetching of the time using the internet.

Additionally this has full support for animations. By default I provide all needed anmiations for a 12h clock to morph one digit into another soothly.
Further animations can be easily added and existing animation can be adjusted to your liking.

The whole codebase is highly modular and configurable and can be tweaked exactly to your liking.

The documentation will be improved in the future. It will only conatin some basic information for now.

Futher documentation is on the [Wiki.](https://github.com/florianL21/LED-ClockShelf/wiki)

# Development Environment:

I am using VScode with PlatformIO.
VSCode can be downloaded from [here.](https://code.visualstudio.com/)
And PlatformIO is an extention that can easily be installed from inside of VSCode.

Via the platformio home the following libraries have to be installed:
 * "Blynk" by Volodymyr Shymanskyy
 * "FastLED" by Daniel Garcia
 * "LinkedList" by Ivan Seidel

# Important files for configuration:
Files that are important for configuration (sorted by importance):
 1. At the top of /src/main.cpp -> General functionallity, like OTA updates, Blynk etc.
 2. /lib/DisplayManager/DisplayConfiguration.cpp -> Configuration of which leds belong to which segments and whioch segments belong to which display, the order of the segments and their animation directions *It is really important to get this one right!*
 3. At the top of /lib/DisplayManager/DisplayManager.h -> Configuring the number of LEDs, Data pin, display options etc.
 4. at the top of /lib/SevenSegment/SevenSegment.cpp -> Configuring the appearance of the digits *If the appearence of the segments is altered the animations have to be adjusted as well*
 5. /lib/SevenSegment/SegmentTransitions.cpp -> Configuring the animations between the digits
 6. at the top of /lib/SevenSegment/SevenSegment.h -> Configuraiong the animation speed between digits
 7. /lib/DisplayManager/Animations.cpp -> configuration of anmimations like the loading animation


More documentation on the configuration and detailed configuration explenation is coming in the future