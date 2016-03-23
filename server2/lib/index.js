var http = require('http')
var passport = require('passport');
//var express = require('express');
//var logger = require('morgan');//logs GET/ 304 12ms
var env = require('../../env.json')
var cfg= env[process.env.NODE_ENV||'development']
require('./modules/mqtt/mqtt.js')
var app = require('./corsPassport');
var regtokau = require('./modules/regtokau/routes')(passport);

var currentPacket;

app.use('/', regtokau);

app.get('/', function(req, res) {
  res.send(currentPacket);
});
app.get('/api', function (req,res){
  res.send("<h4>in dreakin /api</h4>")
});
app.get('/api/date', function (req,res){
  res.send(Date());
});
app.get('/api/time', function (req,res){
  console.log(Date.now())
  console.log(typeof(Date.now()))
  res.send("{\"datetime\":"+ Date.now()/1000 +"}");
});

app.set('port', cfg.port.express || 3000);
var server = http.createServer(app);

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + server.address().port);
  console.log('MQTT broker operating on port ' + cfg.port.mqtt);
  console.log('WebClient server MQTT through WS on port ' + cfg.port.ws);
});