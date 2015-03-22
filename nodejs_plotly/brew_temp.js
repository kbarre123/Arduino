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

var data = [{
  x:[], 
  y:[],
  name: 'Top',
  stream:{
    token:'0om2z8lncl', 
    maxpoints:200
  }
}];

var layout = { 
  title: "Arduino Temp Stream (DS18B20)",
  xaxis: {
    title: "Datetime"
  },
  yaxis: {
    title: "Temperature (*F)"
  },
  autosize: true,
  showlegend: true,
  legend: {
    x: 100,
    y: 1
  }
};

var graphOptions = {
  layout: layout,
  fileopt: "overwrite", 
  filename : "Arduino Temp Stream (DS18B20)",
}; // End Plotly stuff

board.on('ready', function () {
  // This requires OneWire support using the ConfigurableFirmata
  var temperature = new five.Temperature({
    controller: "DS18B20",
    pin: 2,
    freq: 1000
  });

  // initialize the plotly graph
  plotly.plot(data, graphOptions, function (err, res) {
    if (err) console.log(err);
    console.log(res);
    //once it's initialized, create a plotly stream
    //to pipe your data!
    var stream = plotly.stream('0om2z8lncl', function (err, res) {
      if (err) console.log(err);
      console.log(res);
    });
    temperature.on("data", function(err, data) {
      if (err) console.log(err);
      var data = {
        x : getDateString(),
        y : data.fahrenheit
      };
      //console.log(data.celsius + "°C", data.fahrenheit + "°F");
      console.log(data);
      stream.write(JSON.stringify(data)+'\n');
    });
  }); // End Plotly
}); // End board.on

// helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 18000000).toISOString().replace(/T/, ' ').replace(/Z/, '');
  return datestr;
}
