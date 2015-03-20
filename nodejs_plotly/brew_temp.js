/**
 * https://github.com/rwaldron/johnny-five/issues/285
 * https://github.com/lakenen/hardware-stuff/blob/master/ds18b20.js#L1
 */

var plotly = require('plotly')('kbarre123', 'g5nzo225vi');
var five = require('johnny-five');
var board = new five.Board();

// the pin the DS18B20 is connected on
var pin_1 = 2;
// data object to send to plotly
var data = [{x:[], y:[], stream:{token:'wboncpxs1m', maxpoints:100}}];
var layout = {fileopt : "extend", filename : "DS18B20 Stream"};

board.on('ready', function () {

  // johnny-five sensor object; need to add another one for the second sensor
  var sensor1 = new five.Sensor({
    pin: pin_1,
    freq: 1000,
    thresh: 0.5
  });

  board.firmata = board.io;
  board.firmata.sendOneWireConfig(pin_1, true);
  board.firmata.sendOneWireSearch(pin_1, function(error, devices) {
    if(error) {
      console.error(error);
      return;
    }

    // only interested in the first device; THIS WILL CHANGE WHEN ADDING A SECOND SENSOR
    var device = devices[0];

    var readTemperature = function() {

      // start transmission
      board.firmata.sendOneWireReset(pin_1);

      // a 1-wire select is done by ConfigurableFirmata
      board.firmata.sendOneWireWrite(pin_1, device, 0x44);

      // the delay gives the sensor time to do the calculation
      board.firmata.sendOneWireDelay(pin_1, 1000);

      // start transmission
      board.firmata.sendOneWireReset(pin_1);

      // tell the sensor we want the result and read it from the scratchpad
      board.firmata.sendOneWireWriteAndRead(pin_1, device, 0xBE, 9, function(error, data) {
        if(error) {
          console.error(error);
          return;
        }
        var raw = (data[1] << 8) | data[0];
        var celsius = raw / 16.0;
        var fahrenheit = celsius * 1.8 + 32.0;

        console.info('celsius', celsius);
        console.info('fahrenheit', fahrenheit);
      });
    };
    // and every 1 second
    setInterval(readTemperature, 1000);
  });
  
  // initialize the plotly graph
  plotly.plot(data,layout,function (err, res) {
    if (err) console.log(err);
    console.log(res);
    //once it's initialized, create a plotly stream
    //to pipe your data!
    var stream = plotly.stream('wboncpxs1m', function (err, res) {
      if (err) console.log(err);
      console.log(res);
    });
    // this gets called each time there is a new sensor reading
    sensor1.on("data", function() {
      var data = {
        x : getDateString(),
        y : readTemperature()
      };
      console.log(data);
      // write the data to the plotly stream
      stream.write(JSON.stringify(data)+'\n');
    });
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
