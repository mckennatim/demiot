var mqtt = require('mqtt');
//require('./app.html');

const deviceId ='CYURD001'
const statu = deviceId+'/status'
const tmr = deviceId+'/tmr'
const progs = deviceId+'/progs'
const cmd = deviceId +'/cmd'
const devt = deviceId +'/time'
const sched = deviceId +'/sched'
var client_Id = 'mqttjs_';
var host = 'ws://10.0.1.100:3333';
var client = mqtt.connect(host);
var v = document.getElementById("tbox")
var m = document.getElementById("mbox")
var crement=10;
var onoff = 1;

function uclicked(){
	document.getElementById('lue').innerHTML=onoff
	onoff=!onoff;
	var thecmd =  "{\"heat\":"+onoff*1+",\"auto\":1,\"hilimit\":80,\"lolimit\":74,\"empty\":0}"
	console.log(thecmd);
	client.publish(cmd, thecmd)
}
function empty(){
	var thecmd =  "{\"heat\":"+onoff*1+",\"auto\":1,\"hilimit\":85,\"lolimit\":75,\"empty\":1}"
	console.log(thecmd);
	client.publish(cmd, thecmd)			
}
function devtime(){
	var thecmd =  "trigger time from web cliient"
	console.log(thecmd);
	console.log(devt);
	client.publish(devt, thecmd)			
}
function vchanged(){
	//console.log("val changed")
}
function mchanged(){
	console.log("st changed")
}
function newCrement(){
	console.log("crement changed")
	sendSchedule();
}
function sendSchedule(){
	//console.log(v.value);
	crement = document.getElementById("crement").value
	var d = new Date();
	var strtmin = m.value;
	var d0 = Date.now()+60000*strtmin
	var d2 = new Date(d0);

	var setmin = v.value
	var d3 = new Date(d0+ 60000*(setmin));
	console.log(d);
	var hr = d2.getHours();
	var mi = d2.getMinutes();
	var hr3 = d3.getHours();
	var mi3 = d3.getMinutes();
	console.log(`${d2} - ${hr}:${mi}`)
	console.log(`${d3} - ${hr3}:${mi3}`)
	var thecmd =  "{\"crement\":"+crement+",\"serels\":[0,99,1,2],\"progs\":[[[0,0,80,77],[6,12,82,75],[8,20,85,78],["+hr+","+mi+",78,74],["+hr3+","+mi3+",83,79]],[[0,0,58],[18,0,68],[21,30,58]],[[0,0,0],["+hr+","+mi+",1],["+hr3+","+mi3+",0]]]}";
	console.log(thecmd);
	client.publish(sched, thecmd)			
}
client.on('connect', function(){
	console.log('maybe connected')
	client.subscribe(statu) 
	client.subscribe(tmr) 
	client.subscribe(progs)
	client.on('message', function(topic, payload) {
		var pls = payload.toString()
		var plo = JSON.parse(pls)
		console.log(plo)
		console.log('['+topic+'] '+payload.toString())
    var sp = topic.split("/")
    var job = sp[1];	
    switch(job){
    	case "status":
				document.getElementById('lue').innerHTML=plo.heat
				document.getElementById('temp1').innerHTML=plo.temp1
				break;
			case "tmr":
				document.getElementById('timr1').innerHTML=plo.timr1
				document.getElementById('timr2').innerHTML=plo.timr2
				document.getElementById('timr3').innerHTML=plo.timr3
				break;
    }			
	});	
	client.publish('presence', 'Web Client is alive.. Test Ping! ');
});


// window = {
// 	uclicked:uclicked,
// 	devtime:devtime
// }
window.uclicked = uclicked
window.devtime = devtime
window.vchanged = vchanged
window.mchanged = mchanged
window.newCrement = newCrement
window.sendSchedule = sendSchedule
//window.document=document