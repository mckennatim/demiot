var moment = require('moment-timezone');

var cb=function(){
	console.log("in cb")
}

var flattenProgObj =function(progs){	
	a=[]
	progs.map(function(prog, idx){
		prog.wk.map(function(w,ix){
			w.map(function(x){
				za = {};
				za.senrel = prog.senrel
				za.day = ix
				x.map(function(y,i){
					switch(i){
						case 0:
							za.hour=y
							break;
						case 1:
							za.min=y
							break;
						case 2:
							za.val=y
							break;
					}
				})
				a.push(za)
			})
		})
	})
	return a
}

var parseProg =function(prog){
	console.log(prog)
}

var getTime = function(devid, mosca, cb){
	console.log("it's 4 oclock")
  console.log(Date.now())
  console.log(typeof(Date.now()))
  var nynf = parseInt(moment().tz("America/New_York").format("X"))
  var nyf = moment().tz("America/New_York").format('LLLL')
  var nyz = parseInt(moment().tz("America/New_York").format('Z'))
  var pkt = {
  	unix: nynf,
  	LLLL: nyf,
  	zone: nyz,
  };	
	console.log(JSON.stringify(pkt))
	var topi = devid+'/devtime'
	var oPacket = {
		topic: topi,
		payload: JSON.stringify(pkt),
		retain: false,
		qos: 0
  };
  console.log(topi) 
  mosca.publish(oPacket, cb);
  setTimeout(function(){
  	sendSchedule(devid, mosca, cb)
  }, 1000)
	//(devid, mosca, cb)  ;
}

var sendSchedule= function(devid, mosca, cb){
	var sched = [		
		{temp0: [
	  	[6,12,68],
			[8,20,57],
			[22,0,68],
			[23,30,58]]
		},
		{temp1: [
	  	[6,0,67],
			[9,20,57],
			[18,0,68],
			[21,30,58]]
		}
	]
	var sched3 = "{\"serels\":[0,99,1],\"progs\":[[[0,0,80,77],[6,12,82,75],[8,20,85,75],[22,0,78,74],[23,30,85,75]],[[0,0,58],[18,0,68],[21,30,58]]]}";
	var sched2 = [{temp0: {vals:[6,12,68,8,20,57,22,0,68,23,30,58], col:3}},
		{temp1: {vals: [6,0,67,9,20,57,18,0,68,21,30,58], col:3}}]
	console.log(sched[0].temp0[1][1])
	var topi = devid+'/progs'
	var oPacket = {
		topic: topi,
		//payload: JSON.stringify(sched3),
		payload: sched3,
		retain: false,
		qos: 0
  };
  console.log(topi) 
  mosca.publish(oPacket, cb);		
}

module.exports ={
	flattenProgObj: flattenProgObj,
	parseProg: parseProg,
	getTime: getTime
}