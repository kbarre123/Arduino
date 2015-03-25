var plotly = require('plotly')('kbarre123', 'g5nzo225vi');
var five = require('johnny-five'), board
var board = new five.Board();
// the pin the DS18B20 is connected on
var pin = 2;

// Plotly Stuff
var data = [{
  x:[], 
  y:[],
  name: 'Top',
  stream:{
    token:'0om2z8lncl', 
    maxpoints: 1440
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
  autosize: false, // set to 'true' during development; 'false' for customized display on devo
  height: 750, // comment out during development; customized for devo screen
  width: 940//, // comment out during development; customized for devo screen
  //showlegend: true,
  //legend: {
  //  x: 1,
  //  y: 1
  //}
};

var graphOptions = {
  layout: layout,
  fileopt: "extend", 
  filename : "Arduino Temp Stream (DS18B20)",
}; // End Plotly stuff

// Don't do anything until the board is ready for communication.
board.on('ready', function () {
  // This requires OneWire support using the ConfigurableFirmata
  var temperature = new five.Temperature({
    controller: "DS18B20",
    address: 0x60598c0,
    pin: pin,
    freq: 30000
  });

  // Initialize the plotly graph
  plotly.plot(data, graphOptions, function (err, res) {
    if (err) console.log(err);
    console.log(res);
    //Once it's initialized, create a plotly stream
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
