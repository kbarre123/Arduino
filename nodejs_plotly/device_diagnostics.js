// This program prints out device diagnostics.

var five = require('johnny-five'), board
var board = new five.Board();
// the pin the DS18B20 is connected on
var pin = 2;

var data = [{
  x:[]
}];

// Don't do anything until the board is ready for communication.
board.on('ready', function () {
  // This requires OneWire support using the ConfigurableFirmata
  var temperature = new five.Temperature({
    controller: "DS18B20",
    address: 0x60598c0,
    pin: pin,
    freq: 1000
  });
  // Announce all devices on bus
  temperature.once("data", function() {
    //console.log(temperature);
    console.log('Address', this.address.toString(16));
  });

  temperature.on("data", function(err, data) {
    if (err) console.log(err);
    var data = {
      x : data.fahrenheit
    };
    console.log(data);
  });
}); // End board.on
