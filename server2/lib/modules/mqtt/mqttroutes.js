var express = require('express');
var router = express.Router();
import {currentPacket} from '../../modules/mqtt/mqtt.js'

module.exports = function(passport) {
	router.get('/', function(req, res) {
		res.jsonp({message: "in root of mqtt module"})		
	});
	router.get('/pkt', function (req,res){
	  res.send(currentPacket);
	});
	router.get('/time', function (req,res){
	});
	return router;
}