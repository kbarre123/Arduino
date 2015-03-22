// This was mofified from https://github.com/lakenen/hardware-stuff/blob/master/ds18b20.js. It is a lower level implementation
// of Firmata's OneWire protocol.

var five = require('johnny-five'), board

// the pin the DS18B20 is connected on
var pin = 2;
var board = new five.Board();

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

        console.info(getDateString(), 'celsius\t\t', celsius.toFixed(2), "ºC");
        console.info(getDateString(), 'fahrenheit\t', fahrenheit.toFixed(2), "ºF\n");
      });
    };
    // read the temperature now
    readTemperature();
    // and every 1 second
    setInterval(readTemperature, 1000);
  });
});

// helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 18000000).toISOString().replace(/T/, ' ').replace(/Z/, '');
  return datestr;
}