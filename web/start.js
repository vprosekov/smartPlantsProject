const utils = {
	sp: (int) => {
		int = int.toString();
		return int.split('').reverse().join('').match(/[0-9]{1,3}/g).join('.').split('').reverse().join('');
	},
	rn: (int, fixed) => {
		if (int === null) return null;
		if (int === 0) return '0';
		fixed = (!fixed || fixed < 0) ? 0 : fixed;
		let b = (int).toPrecision(2).split('e'),
			k = b.length === 1 ? 0 : Math.floor(Math.min(b[1].slice(1), 14) / 3),
			c = k < 1 ? int.toFixed(0 + fixed) : (int / Math.pow(10, k * 3) ).toFixed(1 + fixed),
			d = c < 0 ? c : Math.abs(c),
			e = d + ['', 'тыс', 'млн', 'млрд', 'трлн'][k];

			e = e.replace(/e/g, '');
			e = e.replace(/\+/g, '');
			e = e.replace(/Infinity/g, 'ДОХЕРА');

		return e;
	},
	gi: (int) => {
		int = int.toString();

		let text = ``;
		for (let i = 0; i < int.length; i++)
		{
			text += `${int[i]}&#8419;`;
		}

		return text;
	},
	decl: (n, titles) => { return titles[(n % 10 === 1 && n % 100 !== 11) ? 0 : n % 10 >= 2 && n % 10 <= 4 && (n % 100 < 10 || n % 100 >= 20) ? 1 : 2] },
	random: (x, y) => {
		return y ? Math.round(Math.random() * (y - x)) + x : Math.round(Math.random() * x);
	},
	pick: (array) => {
		return array[utils.random(array.length - 1)];
	}
}


let temp='', light='', LED='', water='';


const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort("COM9", { baudRate: 9600 });

const parser = new Readline();
port.pipe(parser);

parser.on('data', line => {
  let state = line;
  let arr = state.split('|');
  temp = arr[0];
  light = arr[1];
  LED = arr[2];
  water = parseInt(arr[3]);
  console.log(`> ${state}`);
  console.log(temp + ' ' + light + ' ' + LED + ' ' + water)
});



var mysql = require('mysql');

var con = mysql.createConnection({
  host: "host",
  user: "user",
  password: "pass",
  database: "dbname"
});




const fs = require('fs');

var cc = fs.readFileSync(__dirname + "/public/info.json");
var info = JSON.parse(cc);

let tre;

var NodeWebcam = require( "node-webcam" );
var opts = {
  width: 1280,
  height: 720,
  quality: 100,
  delay: 0,
  saveShots: true,
  output: "jpeg",
  device: false,
  callbackReturn: "location",
  verbose: false
};

function base64_encode(file) {
      let bitmap = fs.readFileSync(file);
      return new Buffer(bitmap).toString('base64');
    }

var Webcam = NodeWebcam.create( opts );
Webcam.capture( "test", function( err, data ) {} );


async function saveInfo()
{
	fs.writeFile(__dirname + '/public/info.json', JSON.stringify(info, null, '\t'), function(err) {
		if(err) throw err;
	});
	return true;
}


setInterval(async () => {
    Webcam.capture( "test", function( err, data ) {} );
    info.image = base64_encode('test.jpg');
    info.temp = temp;
    info.light = light;
    info.LED = LED;
    info.water = water;
    saveInfo();
    /*con.connect(function(err) {
      if (err) console.log(err);
      console.log("Connected!");
      var sql = "INSERT INTO `plants`(`json`) VALUES ('" + JSON.stringify(info) + "')";
      con.query(sql, function (err, result) {
        if (err) console.log(err);
        console.log('sent');
      });
    });*/
}, 30000);


/*var express = require('express');
var app = express();

app.use(express.static('public'));

app.get('/', function (req, res) {
  
  let html = `<!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta http-equiv="refresh" content="5" />
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" >
    <link type = "text/css" rel = "stylesheet" href = "css\\test.css">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>YO!</title>
    <script src="https://kit.fontawesome.com/843e7f8a54.js"></script>
  </head>
  <body>
    <div class="container main align-items-center d-flex">
    <div class="row">
      <div class="col-md-6 col-12">
        <img id='image' src='img/test.jpg' class='w-100'>
  
  
      </div>
  
      <div class="col-md-6 col-12 justify-content-center d-flex align-items-center">
        <div class="test">
        <p><i class="fa fa-thermometer-quarter" aria-hidden="true"></i> temp = <i id="temp">${temp}</i></p>
        <p><i class="fas fa-sun"></i> light = <i id="light">${light}</i></p>
        <p><i class="far fa-lightbulb"></i> LED = <i id="led">${LED}</i></p>
        <p><i class="fa fa-tint" aria-hidden="true"></i> water = <i id="water">${water}</water></p>
      </div>
      </div>
  
  
  
    </div>
  </div>
  
  <script>
    
    setTimeout(function() {
      $.getJSON('/info.json', function(data) {
        document.getElementById("temp").innerHTML=data.temp;
        document.getElementById("light").innerHTML=data.light;
        document.getElementById("led").innerHTML=data.LED==1?"ON":"OFF";
        document.getElementById("water").innerHTML=data.water;
        document.getElementById("image").src='data:image/jpeg;base64,'+data.image;
        console.log("aSIADSDASD");
      });
    }, 1000);
  </script>
  
  
    <script
    src="https://code.jquery.com/jquery-3.2.1.min.js"
    integrity="sha256-hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4="
    crossorigin="anonymous">
    </script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
  </body>
  </html>`;
  res.send(html);
});
//document.getElementById("image").src='data:image/jpeg;base64,'+data.image;
//<img id='image' src=\'data:image/jpeg;base64,${tre}\' class='w-100'>
app.get('/test', function (req, res) {
  res.sendFile(__dirname + '/test.jpg');
});
app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});*/