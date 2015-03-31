// This program prints out device diagnostics.

var five = require('johnny-five'), board
var board = new five.Board();
// the pin the DS18B20 is connected on
var pin = 2;

// Don't do anything until the board is ready for communication.
board.on('ready', function () {
  // This requires OneWire support using the ConfigurableFirmata
  var temperature_1 = new five.Temperature({
    controller: "DS18B20",
    //address: 0x60630e7, // Probe A
    pin: pin,
    bus_pos: 1,
    freq: 1000
  });
  
  /*var temperature_2 = new five.Temperature({
    controller: "DS18B20",
    //address: 0x60598c0, // Probe B
    //pin: pin,
    bus_pos: 1,
    freq: 1000
  });*/
  
  // Announce all devices on bus
  temperature_1.once("data", function() {
    console.log('Address', this.address.toString(16));
  });

  /*temperature_2.once("data", function() {
    console.log('Address', this.address.toString(16));
  });*/

  temperature_1.on("data", function(err, data) {
    if (err) console.log(err);
    var data = {
      x : data.fahrenheit
    };
    console.log(data);
  });

  /*temperature_2.on("data", function(err, data) {
    if (err) console.log(err);
    var data = {
      x : data.fahrenheit
    };
    console.log('Sensor 2', data);
  });*/

}); // End board.on
