/* Maxbotix LV-MaxSonar-EZ Series High Performance Sonar Range Finder
 * Sensor for ESPHome and HomeAssistant
 *
 * Datasheet for LV-EZ Sensors: https://www.maxbotix.com/documents/LV-MaxSonar-EZ_Datasheet.pdf
 * Based on code from ESPHome custom text sensor and others: https://esphome.io
 *
 * 2021-06-14 Stephen Houser <stephenhouser@gmail.com>
 *
 * From the LV-EZ Datasheet:
 * Pin 5-TX- When the *BW is open or held low, the TX output delivers
 * asynchronous serial with an RS232 format, except voltages are 0-Vcc.
 * The output is an ASCII capital “R”, followed by three ASCII character digits
 * representing the range in inches up to a maximum of 255, followed by a
 * carriage return (ASCII 13). The baud rate is 9600, 8 bits, no parity, with
 * one stop bit. Although the voltage of 0-Vcc is outside the RS232 standard,
 * most RS232 devices have sufficient margin to read 0-Vcc serial data. If
 * standard voltage level RS232 is desired, invert, and connect an RS232
 * converter such as a MAX232. When BW pin is held high the TX output sends a
 * single pulse, suitable for low noise chaining. (no serial data).
 *
 * Pin 6-+5V- Vcc – Operates on 2.5V - 5.5V. Recommended current capability of
 * 3mA for 5V, and 2mA for 3V. Please reference page 4 for minimum operating
 * voltage verses temperature information.
 *
 * Pin 7-GND- Return for the DC power supply. GND (& Vcc) must be ripple and
 * noise free for best operation.
 */
#include "esphome.h"

class LVMaxSonarEZSensor : public Component, public UARTDevice {
    public:
	Sensor *inch_sensor = new Sensor();
  	Sensor *meter_sensor = new Sensor();

  	LVMaxSonarEZSensor(UARTComponent *parent) : UARTDevice(parent) {		  
	}

	void setup() override {
	}

	/* Parse text string from MaxBotix sensor: Rnnn */
  	int parse_distance(char *buffer) {
        if (*buffer == 'R') {
			const char *value = (buffer + 1);
            return atoi(value);
        }

        return 0;
    }

	/* Collect read characters into buffer, newline delimited. */
	char *readline(int readch, char *buffer, int len) {
		static int pos = 0;

		if (readch > 0) {
			switch (readch) {
				case '\n':
				case '\r':
					pos = 0;
					return buffer;

				default:
					if (pos < len - 1) {
						buffer[pos++] = readch;
						buffer[pos] = 0;
					}
			}
		}

		/* return if we run out of buffer space */
		if (pos == len) {
			pos = 0;
			return buffer;
		}

		return NULL;
    }

	void loop() override {
        const int max_line_length = 10;
        static char buffer[max_line_length];

        while (available()) {
			if (readline(read(), buffer, max_line_length)) {
				float value = parse_distance(buffer);
                if (value > 0) {
                    inch_sensor->publish_state(value);
					meter_sensor->publish_state(value * 0.0254);
                }
			}
        }
    }
};