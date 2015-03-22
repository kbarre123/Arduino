var five = require('johnny-five'), board

var pin = 2;
var board = new five.Board();

// Need a place to store the temperatures
var temps = [];

// Dumps the array of temps after converting each to Fahrenheit
var outputFTemps = function() {
    var out = [];
    for (var i = 0, len = temps.length; i < len; i++) {
        var celsius = temps[i] / 16.0;
        out[i] = (celsius * 1.8 + 32.0).toFixed(2);
    }
    console.log(out);
}

// Don't do anything until the board is ready for communication.
board.on("ready", function () {

    // Find all the devices on the 1-wire bus.  Reset, then send command search.
    board.io.sendOneWireConfig(pin, true);
    board.io.sendOneWireSearch(pin, function(error, allDevices) {
        if(error) {
          console.error(error);
          return;
        }

        // Parse out only the temperature probes (DS18B20)
        var devices = [];
        for (var i = 0, len = allDevices.length; i < len; i++) {
            if(allDevices[i][0] == 0x28) {
                devices.push(allDevices[i]);
                }
        }

        // Method to read a single temperature probe and store it in the temps array.
        var readSingle = function(deviceNum) {

            // If we reached the end of the list of devices, then start again by calling readTemperatures in a non-blocking way.
            if(!(deviceNum in devices)) {
                setTimeout(readTemperatures,0);
                return;
            }

            // Send the reset on the bus so it is ready for next command.
            board.io.sendOneWireReset(pin);

            // tell the sensor we want the result and read it from the scratchpad
            board.io.sendOneWireWriteAndRead(pin, devices[deviceNum], 0xBE, 9, function(error, data) {
                if(error) {
                  console.error(error);
                  return;
                }

                // Convert the data into a raw integer.
                var raw = (data[1] << 8) | data[0];
                temps[deviceNum] = raw;

                // We have read this temp, lets read the next one.
                setTimeout(readSingle,0,deviceNum+1);
            }.bind(deviceNum));     
        }

        // Method to sent CONVERT command to all devices and then start the reading chain.
        var readTemperatures = function() {
        //console.log('==================== Read Cycle Start ==========================');

            // Reset and start convert on all probes.
            for (var i = 0, len = devices.length; i < len; i++) {
                board.io.sendOneWireReset(pin);
                board.io.sendOneWireWrite(pin, devices[i], 0x44);
            }

            // Read the first device temperature.
            setTimeout(readSingle, 0, 0);       
        };

        // Output the temps every 1/10 second.
        setInterval(outputFTemps, 1000);

        // Start the loop to continually read the temps.
        readTemperatures();
    });
});