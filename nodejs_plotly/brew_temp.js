/**
 * NOTE: this does not run on the built-in firmware. Steps to make it work:
 *  - get and install the arduino IDE from http://arduino.cc/en/Main/Software#toc1
 *  - rm -r /Applications/Arduino.app/Contents/Resources/Java/libraries/Firmata
 *  - git clone git@github.com:firmata/arduino.git --branch configurable --single-branch /Applications/Arduino.app/Contents/Resources/Java/libraries/Firmata
 *  - flash the board with ConfigurableFirmata
 *    - File > Examples > Firmata > ConfigurableFirmata
 *    - Click 'Upload'
 *
 * https://github.com/rwaldron/johnny-five/issues/285
 */  

var plotly = require('plotly')('kbarre123', 'g5nzo225vi');
var five = require('johnny-five'), board
var board = new five.Board();
// the pin the DS18B20 is connected on
var pin = 2;

// Plotly stuff
var data = [{
  x:[], 
  y:[], 
  stream:{
    token:'wboncpxs1m', 
    maxpoints:1800
  }
}];

var layout = { 
  title: "Arduino Temp Stream (LM35)",
  xaxis: {
    title: "Datetime"
  },
  yaxis: {
    title: "Temperature (*F)"
  }
};

var graphOptions = {
  layout: layout,
  fileopt: "overwrite", 
  filename : "Arduino Temp Stream (LM35)",
}; // End Plotly stuff

board.on('ready', function () {

  board.firmata = board.io;
  board.firmata.sendOneWireConfig(pin, true);
  board.firmata.sendOneWireSearch(pin, function(error, devices) {
    if(error) {
      console.error(error);
      return;
    }

    // only interested in the first device
    var device = devices[0];

    var readTemperature = function() {
      // led.on();

      // start transmission
      board.firmata.sendOneWireReset(pin);

      // a 1-wire select is done by ConfigurableFirmata
      board.firmata.sendOneWireWrite(pin, device, 0x44);

      // the delay gives the sensor time to do the calculation
      board.firmata.sendOneWireDelay(pin, 1000);

      // start transmission
      board.firmata.sendOneWireReset(pin);

      // tell the sensor we want the result and read it from the scratchpad
      board.firmata.sendOneWireWriteAndRead(pin, device, 0xBE, 9, function(error, data) {
        if(error) {
          console.error(error);
          return;
        }
        var raw = (data[1] << 8) | data[0];
        var celsius = raw / 16.0;
        var fahrenheit = celsius * 1.8 + 32.0;
        return fahrenheit;
      });
    };
    // initialize the plotly graph
    plotly.plot(data, graphOptions, function (err, res) {
      if (err) console.log(err);
      console.log(res);
      //once it's initialized, create a plotly stream
      //to pipe your data!
      var stream = plotly.stream('wboncpxs1m', function (err, res) {
        if (err) console.log(err);
        console.log(res);
      });
      // this gets called each time there is a new sensor reading
      readTemperature().on("data", function() {
        var data = {
          x : getDateString(),
          y : this.value
        };
        console.log(data);
        // write the data to the plotly stream
        stream.write(JSON.stringify(data)+'\n');
      });
    });
    // read the temperature now
    //readTemperature();
    // and every 1 second
    //setInterval(readTemperature, 1000);
  });
});

// little helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 18000000).toISOString().replace(/T/, ' ').replace(/Z/, '');
  return datestr;
}
