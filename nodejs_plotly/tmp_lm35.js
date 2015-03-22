var plotly = require('plotly')('kbarre123', 'g5nzo225vi');
var five = require("johnny-five");
var brew_bot = new five.Board();

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
    title: "Datetime " + getXAxisTime(data[0].stream.maxpoints)
  },
  yaxis: {
    title: "Temperature (*F)"
  }
};

var graphOptions = {
  layout: layout,
  fileopt: "overwrite", 
  filename : "Arduino Temp Stream (LM35)",
};

brew_bot.on("ready", function() {

  // create a new tmp36 sensor object
  var tmp36 = new five.Sensor({
    pin: "A0",
    freq: 1000, // get reading every 1000ms
    thresh: 0.5
  });
  
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
    tmp36.on("data", function() {
      var data = {
        x : getDateString(),
        y : convertTemperature(this.value)
      };
      console.log(data);
      // write the data to the plotly stream
      stream.write(JSON.stringify(data)+'\n');
    });
  });
});

// helper function to convert sensor value to temp
function convertTemperature (value) {
  var milliVolts = (value / 1024.0) * 5000;
  var celsius = milliVolts / 10;
  var fahrenheit = ((celsius * 9) / 5) + 32;
  return fahrenheit;
}

// little helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 18000000).toISOString().replace(/T/, ' ').replace(/Z/, '');
  return datestr;
}

function getXAxisTime(maxPoints) {
  var time = 0;
  var dateTime = "";
  if (maxPoints > 3600) {
    time = (maxPoints / 3600).toFixed(2);
    dateTime = "(Past " + time + " Hours)";
  } else {
    time = (maxPoints / 60).toFixed(2);
    dateTime = "(Past " + time + " Minutes)";
  }
  return dateTime;
}