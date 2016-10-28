'use strict'

var fs = require('fs');
var getPixels = require('get-pixels');


if(process.argv.length < 3)
{
  console.log(" ");
  console.log("-------------error------------------");
  console.log("You need to input three image source.");
  console.log("ex: node app.js <img1> <img2> <img3>");
  console.log("-------------error------------------");
  return
}
else
{
  // show images names
  console.log(" ");
  console.log("-------------show images name--------");
  process.argv.forEach(function (val, index) {
    if(index > 1)
    {
      console.log('image: ' + val);
    }
  });
}

for(var i = 0, i < process.argv.length, i++)
{
  console.log("!");
}

getPixels("class.png", function(err, pixels) {
  if(err) 
  {
    console.log("Bad image path")
    return
  }
  else
  {
    for (var pixel = 0; pixel < 3; pixel++) {
      var i = pixel * 4;
      console.log("R: " + pixels.data[i] + ", G: " + pixels.data[i + 1] + ", B: " + pixels.data[i + 2]);
    }
  }
  console.log("got pixels", pixels.shape)
});
