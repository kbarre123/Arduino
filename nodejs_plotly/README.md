#Arduino, Nodejs, and Plot.ly

**NOTE:** The main focus of these programs is to get a functioning OneWire bus comprised of at least two DS18B20 sensors reporting to Plotly for use in my home brewing operation. The main program here is `brew_temp.js`. All other programs are used as research to further the development of the main program.

`brew_temp.js` uses [Nodejs](https://github.com/joyent/node) for real-time streaming of data collected by an [Arduino](http://www.arduino.cc/en/Main/Software) and several [DS18B20 temperature sensors](http://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html) to [Plot.ly](https://plot.ly/rest/) via the host-computer's network connection (thanks to [Firmata](https://github.com/jgautier/firmata) and [Johnny-Five](https://github.com/rwaldron/johnny-five)!).

## Hardware Setup

Wire up the DS18B20 sensors to the Arduino according to this diagram.

[DS18B20 Waterproof Sensor](http://i.imgur.com/sKhYbkM.jpg)

## Software Setup

### Arduino

Download and install the [Arduino IDE](http://www.arduino.cc/en/Main/Software). I've written a tutorial on doing so in a Linux environment [here](http://kbarre123.github.io/blog/2015/03/10/building-the-arduino-ide-from-source-and-launching-it-from-the-applications-menu/).

### Firmata

The Firmata library and examples included with the Arduino IDE needs to be replaced as it doesn't contain the necessary example sketch entitled `ConfigurableFirmata`. This sketch is needed because it supports the `OneWire` protocol (along with many others) needed to work with the DS18B20. To install the correct Firmata library:

 * get and install the arduino IDE from http://www.arduino.cc/en/Main/Software
 * `rm -r /home/username/arduino-1.6.1/libraries/Firmata`
 * `git clone git@github.com:firmata/arduino.git --branch configurable --single-branch /home/username/arduino-1.6.1/libraries/Firmata`
 * upload the new ConfigurableFirmata to your Arduino:
 	* File > Examples > Firmata > ConfigurableFirmata
 	* Click 'Upload'

### Nodejs

`sudo apt-get install node`. If you're using a Debian-derived distribution, I'd recommend using the `node-legacy` package instead. Long story, just use it instead of `node`. It's the same thing, but less hassle. You'll also want to install Node's package manager: `sudo apt-get install npm`.

**Note:** I've included the `node_modules` folder in my `.gitignore` file to keep the repo size down. Your project folder should look *something* like this once everythings set up and ready to go:

    project_folder
    ├── brew_temp.js
    ├── node_modules
    │   ├── johnny-five
    │   └── plotly
    ├── README.md
    └── tmp_lm35.js


### Johnny-Five

`npm install johnny-five`. Also, check out this [issue](https://github.com/rwaldron/johnny-five/issues/285) regarding using multiple DS18B20 sensors. It doesn't seem to be resolved, but I'm moving forward and will hopefully figure out something.

### Plotly

`npm install plotly`. Easy as that. You'll have to create an account, at which time a login token and streaming token will be available under your profile page. You'll need these to stream your data to Plotly. 

## Program Details

### brew_temp.js

There are two DS18B20 water-proof temperature sensors attached.

### tmp_lm35.js

The LM35 sensor is hooked up (with the flat side facing you):

* left pin to 5V
* middle pin to A0
* right pin to Ground

