# ESPHome Maxbotix LV-EZ Distance Sensor 

Maxbotix LV-MaxSonar-EZ Series High Performance Sonar Range Finder Sensor for ESPHome and HomeAssistant

NOTE: Only works on ESP32. The ESP8266 cannot "invert" the UART signals. If you want to use this with an ESP8266, you will have to invert the incoming signal from the Maxbotix's TX pin.

- Datasheet for LV-EZ Sensors: https://www.maxbotix.com/documents/LV-MaxSonar-EZ_Datasheet.pdf
- Based on code from ESPHome custom text sensor and others: https://esphome.io
- YAML Example from: https://github.com/kquinsland/ws3-to-esphome-bridge/blob/master/esphome/ws3.yaml

## Notes

When looging to web server and getting log output, ESPHome crashes with...

```
[15:09:14]Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandled.
[15:09:14]Core 1 register dump:
[15:09:14]PC      : 0x400e4f74  PS      : 0x00060030  A0      : 0x800e500a  A1      : 0x3ffb1b90  
[15:09:14]A2      : 0x3ffd550c  A3      : 0x3ffc56c8  A4      : 0x0000008f  A5      : 0x3ffd4a42  
[15:09:14]A6      : 0x0000000a  A7      : 0x1b796361  A8      : 0x800e4f80  A9      : 0x3ffb1b70  
[15:09:14]A10     : 0x0e0f0d0e  A11     : 0x3ffd49c3  A12     : 0x0000008f  A13     : 0x3ffd48f4  
[15:09:14]A14     : 0x00000000  A15     : 0x5678006d  SAR     : 0x0000000a  EXCCAUSE: 0x0000001c  
[15:09:14]EXCVADDR: 0x0e0f0d16  LBEG    : 0x4000c2e0  LEND    : 0x4000c2f6  LCOUNT  : 0xffffffff  
[15:09:14]
[15:09:14]Backtrace: 0x400e4f74:0x3ffb1b90 0x400e5007:0x3ffb1bb0 0x400e5023:0x3ffb1bd0 0x400e5079:0x3ffb1bf0 0x400d97be:0x3ffb1c20 0x400d7395:0x3ffb1c40 0x400d74dd:0x3ffb1c70 0x400df549:0x3ffb1c90 0x400df576:0x3ffb1cc0 0x400d8a2d:0x3ffb1d20 0x400d8222:0x3ffb1d80 0x400d828d:0x3ffb1da0 0x400d823e:0x3ffb1dd0 0x400d8228:0x3ffb1e00 0x400d824e:0x3ffb1e20 0x400d8269:0x3ffb1e50 0x400d823e:0x3ffb1e90 0x400d8ab4:0x3ffb1ec0 0x400e01c5:0x3ffb1ef0 0x40164039:0x3ffb1f20 0x40164105:0x3ffb1f40 0x400ddbfe:0x3ffb1f60 0x400e0322:0x3ffb1f90 0x400eca6d:0x3ffb1fb0 0x40088e91:0x3ffb1fd0
[15:09:14]
[15:09:14]Rebooting...
[15:09:14]ets Jun  8 2016 00:22:57
[15:09:14]
[15:09:14]rst:0xc (SW_CPU_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
[15:09:14]configsip: 0, SPIWP:0xee
[15:09:14]clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
[15:09:14]mode:DIO, clock div:2
[15:09:14]load:0x3fff0018,len:4
[15:09:14]load:0x3fff001c,len:1044
[15:09:14]load:0x40078000,len:8896
[15:09:14]load:0x40080400,len:5828
[15:09:14]entry 0x400806ac
[15:09:14][I][logger:170]: Log initialized
```

Tracking this down with the backtrace reveals.

```
0x400e4f74: AsyncEventSourceClient::_runQueue() at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1858
0x400e5007: AsyncEventSourceClient::_queueMessage(AsyncEventSourceMessage*) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1858
0x400e5023: AsyncEventSourceClient::write(char const*, unsigned int) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1858
0x400e5079: AsyncEventSource::send(char const*, char const*, unsigned int, unsigned int) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1858
0x400d97be: std::_Function_handler<void (int, char const*, char const*), esphome::web_server::WebServer::setup()::{lambda(int, char const*, char const*)#2}>::_M_invoke(std::_Any_data const&, int&&, char const*&&, int&&) at /config/ultrasonic/src/esphome/components/web_server/web_server.cpp:719
 (inlined by) _M_invoke at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1871
0x400d7395: std::function<void (int, char const*, char const*)>::operator()(int, char const*, char const*) const at /config/ultrasonic/src/esphome/components/logger/logger.cpp:175
 (inlined by) esphome::CallbackManager<void (int, char const*, char const*)>::call(int, char const*, char const*) at /config/ultrasonic/src/esphome/core/helpers.h:212
 (inlined by) esphome::logger::Logger::log_message_(int, char const*, int) at /config/ultrasonic/src/esphome/components/logger/logger.cpp:113
0x400d74dd: esphome::logger::Logger::log_vprintf_(int, char const*, int, char const*, __va_list_tag) at /config/ultrasonic/src/esphome/components/logger/logger.cpp:175
0x400df549: esphome::esp_log_vprintf_(int, char const*, int, char const*, __va_list_tag) at /config/ultrasonic/src/esphome/core/log.cpp:32
0x400df576: esphome::esp_log_printf_(int, char const*, int, char const*, ...) at /config/ultrasonic/src/esphome/core/log.cpp:14
0x400d8a2d: esphome::sensor::Sensor::internal_send_state_to_frontend(float) at /config/ultrasonic/src/esphome/components/sensor/sensor.cpp:92
0x400d8222: esphome::sensor::Filter::output(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d828d: esphome::sensor::OrFilter::PhiNode::new_value(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d823e: esphome::sensor::Filter::input(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d8228: esphome::sensor::Filter::output(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d824e: esphome::sensor::Filter::input(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d8269: esphome::sensor::OrFilter::new_value(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d823e: esphome::sensor::Filter::input(float) at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/bits/move.h:187
0x400d8ab4: esphome::sensor::Sensor::publish_state(float) at /config/ultrasonic/src/esphome/components/sensor/sensor.cpp:92
0x400e01c5: LVMaxSonarEZSensor::loop() at /config/ultrasonic/src/esphome/components/uart/uart.h:125
0x40164039: esphome::Component::call_loop() at /root/.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/functional:1754
0x40164105: esphome::Component::call() at /config/ultrasonic/src/esphome/core/component.cpp:82
0x400ddbfe: esphome::Application::loop() at /config/ultrasonic/src/esphome/core/application.cpp:72
0x400e0322: loop() at /config/ultrasonic/src/esphome/components/uart/uart.h:125
0x400eca6d: loopTask(void*) at /root/.platformio/packages/framework-arduinoespressif32/cores/esp32/main.cpp:19
0x40088e91: vPortTaskWrapper at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/freertos/port.c:355 (discriminator 1)
```

- The logger.cpp component has a comment arount the log_message about the ESP32 and only logging if there is 2048 bytes available on the stack.