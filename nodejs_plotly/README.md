#Arduino, Nodejs, and Plot.ly

These sketches involve using an [Arduino](http://arduino.cc) collecting sensor data and posting it to [Plot.ly](https://plot.ly/rest/) via [Nodejs](http://nodejs.org) for real-time streaming of data (via the host-computer's network connection; thanks [Johnny-Five](https://github.com/rwaldron/johnny-five)) without page-refreshes. You know, fancy stuff.

Note that the `node_modules` folder is included in `.gitignore`. When cloning this project, be sure to run `npm install plotly` and `npm install johnny-five` from the root of your `project_folder`. Your project folder should look something like this:

    project_folder
    ├── brew_temp.js
    ├── node_modules
    │   ├── johnny-five
    │   └── plotly
    ├── README.md
    └── tmp_lm35.js

##tmp_lm35.js

The Arduino is running the (unedited) ConfigurableFirmata sketch. The LM35 sensor is hooked up (with the flat side facing you):

* left pin to 5V
* middle pin to A0
* right pin to Ground

## brew_temp.js

The Arduino is running the (unedited) ConfigurableFirmata sketch. There are two DS18B20 water-proof temperature sensors attached. [Firmata OneWire protocol](https://github.com/firmata/protocol/blob/master/onewire.md) is used to interact with the sensors, via Johnny-Five object. Node performs the backend stuff and Plotly performs the graphical display.