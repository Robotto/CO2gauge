# CO2gauge
A gauge that shows how much of the gridmix is carbon neutral

![Image of the thing](https://raw.githubusercontent.com/robotto/CO2gauge/master/CO2gauge.jpg?s=200)

# API usage and key
Get your own from [co2signal.com](https://www.co2signal.com/) for free! Check  [electricitymap.org](https://www.electricitymap.org/) for your grid ID (country code), or the [co2signal.com](https://www.co2signal.com/) docs for how to do a lookup using lat/lon instead of grid ID.

# Hardware
ESP8266 running Arduino on a Wemos D1 mini board.

Gauges are from [here](https://www.ebay.com/itm/Black-Voltmeter-SO-45-AC-0-300V-Round-Analog-Dial-Panel-Meter-Voltmeter-Gauge/312322530464). I have replaced the 100K resistor inside with a 1K resistor to change the max voltage to ~3V, thus enabling direct PWM control of the gauge.
