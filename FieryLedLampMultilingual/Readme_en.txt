There is a READ ME.doc file in the archive. It has pictures and is more detailed. If it is possible to read doc files,
contact him!

At the moment (15.05.2021) the FastLED library is installed with the latest version 3.4.0, which works with esp kernel version 2.7.4!

1. Install the ESP kernel (Tools/Board/Board Manager) version 2.7.4!

2. All libraries except ArduinoJson and GyverButton are installed and \ or updated to the latest version from "Library Management Tools"
or from this archive.
We put ArduinoJson there, but version 5.13.5 (the latest release of the 5th version). GyverButton we put version 3.7 of this archive

3. A plug-in is used to load the file system into the controller. Located in the Uploaders folder of this archive. There are installation instructions.
When downloading the firmware to the board over the wire, the firmware itself is first compiled and loaded, and then the file system.

4. To create the web page, a universal file system was used from here https://github.com/renat2985/easy_Iot_file_system/wiki

5. When the lamp is turned on for the first time, it creates an access point called Led Lamp Web. You need to connect to it.
The password for connecting to the access point is 31415926. The IP address for connecting to the web page of the lamp is 192.168.4.1


6. When you turn it on for the first time, you need to activate the web interface. Activation consists in confirming tolerance towards Ukraine.
 The developer of the web interface is a citizen and patriot of this country.
    Next, by visiting the web page of the lamp on the "Initial installation" tab, you can choose the interface language, configure the connection to your home network.
(don't forget to press the "Save" button after entering your network and password and check the "Use router connection" checkbox)
There you can also set the lamp name, time zone parameters, summer time switch, access point name and password
(I advise you to change the password to your own for the purpose of information security). After that, you can click the "Reload" button at the bottom of the page.
The lamp will connect to the network. Its IP address can be found by pressing the button five times or in the Arduino IDE monitor.

7. This firmware is fully compatible with the FireLamp program from Koteyka 🐱. To date, the version is 3.2.3. You can download here https://files.fm/f/m5cmkuukt7

8. On the web page tab of the "User Settings" lamp, you can control the time display with a running line, turn on/off the random selection mode
settings for effects in the cycle, return the default settings of effects (at the time of loading), on/off switching between effects with the lamp button
only by the effects selected in the cycle, turn on/off the lamp button (it is in the application, but hidden), control the "Dawn" alarm clock and the "Cycle" mode,
 as well as much more.


9. There are plans to download the firmware as a bin file through the web interface (And many other things...). To date, the plan has been fulfilled :-), and exceeded :-).

Enjoy using! :-)