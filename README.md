# ratgdo-mqtt

ratgdo gives you **local** MQTT & dry contact control plus status feedback for your residential Chamberlain/LiftMaster/Merlin garage door opener _including_ Security+ 2.0 openers. (Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door). 

ratgdo wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate directly with NodeRED or Home Assistant, eliminating the need for another "smart" device.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

ratgdo also can control door openers that use a dry contact interface. Status feedback is provided through two reed switches.

> **ratgdo available to order**
> Shipping domestic USA via USPS or internaltional via Global Post.
>
> * [ratgdo v2.5 control board only](https://square.link/u/B5pW7OZW)
> * [ratgdo v2.5 control board with installation kit](https://square.link/u/FKqlMSWT)

# [Visit the github.io page for instructions](https://paulwieland.github.io/ratgdo/).
[ratgdo on GitHub.io](https://paulwieland.github.io/ratgdo/)

# This Version
This version of the ratgdo-mqtt firmware is copied from Paul Wieland's https://github.com/ratgdo/mqtt-ratgdo project with appears to have been abandoned, and includes the source from https://github.com/PaulWieland/arduinoImprovBootstrapper in a nice neat package See its [README](bootstrap/README.md).

It is focused exclusively on the v2.5i hardware, which MAY or MAY NOT still be available.  This project does NOT support `Ratgdo 32` or `Ratgdo 32 Disco`. 

It has been modified to:
- Not come out of sync with the garage door opener when motion is detected which may result in the light on detection coming out of sync with the actual opener, but it doesn't lose connection with the opener if the power is lost.
- Report a similar version to Paul Wielands code but modified to indicate it has been fixed (`2.59a` currently).
- Be loaded using the loader in the `loader` directory, or if you're comfortable with it: https://squids.ca/loader/flash.html

# Paul Wieland's Special thanks

Special thanks to kosibar, Brad and TechJunkie01 - without whom this project would not have been possible.

# Building
- Clone the repository
- Use the VSCode "Platform IO IDE" to open the directory.
- Select "ratgdo_esp8266_hV25 / Build"
- Load using the `loader/flash.html` - must be loaded on an actual web server
