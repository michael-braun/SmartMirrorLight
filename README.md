# SmartMirrorLight

SmartMirrorLight is a simple smart mirror that should be cheap. It uses an ESP-8266, two OLED-displays, a TM1637 module, a TCA9548A I2C multiplexer, a GY-9960 gesture sensor and an IKEA RIBBA picture frame.

## Setup

Copy the file _Configuration.hpp.copy_ and rename it to _Configuration.hpp_. After this set the required settings inside the _Configuration.hpp_ file.

You have to set the url of a [rasterizr-server](https://github.com/michael-braun/rasterizr-server), which generates the required icons.
