###Notes

The node modules are shared by tmp36.js and brew_temp.js. I need to merge plotly stuff from tmp36.js into brew_temp.js.

Note that the node_modules folder is included in .gitignore. When cloning this project, be sure to run `npm install plotly` and `npm install johnny-five` from the root of /nodejs_plotly.

#tmp_lm35.js

The Arduino is running the (unedited) ConfigurableFirmata sketch. The LM35 sensor is hooked up (with the flat side facing you):

* left pin to 5V
* middle pin to A0
* right pin to Ground