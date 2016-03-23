var express = require('express');
var router = express.Router();

module.exports = function(passport) {
	router.get('/', function(req, res) {
		res.jsonp({message: "in root of datetime module"})		
	});
	router.get('/date', function (req,res){
	  res.send(Date());
	});
	router.get('/time', function (req,res){
	  console.log(Date.now())
	  console.log(typeof(Date.now()))
	  res.send("{\"datetime\":"+ Date.now()/1000 +"}");
	});
	return router;
}

