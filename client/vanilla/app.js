var Navigo = require('navigo')
//import Navigo from 'navigo'

var router = new Navigo();


router.on('/reset', function(){
	console.log('am reset')
})
router.on('/graph/:id', function(params){
	console.log(`am graphing ${params.id} `)
})
router.on('/adj', function(){
	Adj()
})
router.on('/selsen', function(){
	console.log('am selsen')
})

const Adj = function(){
	console.log('in adjusting')
}

const deviceId ='CYURD001'
const statu = deviceId+'/status'
const cmd = deviceId +'/cmd'
var client = mqtt.connect('ws://10.0.1.104:3333')

var onoff = 1
function uclicked(){
	//document.getElementById('lue').innerHTML=onoff
	onoff=!onoff;
	var thecmd =  "{\"heat\":"+onoff*1+",\"src\":1,\"empty\":0}"
	console.log(thecmd);
	client.publish(cmd, thecmd)
}
function empty(){
	var thecmd =  "{\"heat\":"+onoff*1+",\"src\":1,\"empty\":1}"
	console.log(thecmd);
	client.publish(cmd, thecmd)			
}
client.on('connect', function(){
	console.log('maybe connected')
	client.subscribe(statu) 
	client.on('message', function(topic, payload) {
		var pls = payload.toString()
		var plo = JSON.parse(pls)
		console.log(plo)
		console.log('['+topic+'] '+payload.toString())
		document.getElementById('lue').innerHTML=plo.heat
		document.getElementById('temp1').innerHTML=plo.temp1
	});	
	client.publish('presence', 'Web Client is alive.. Test Ping! ');
});

const adj = (
`<div id="main">
		<h1>read and change device</h1>
		<span  id="temp1"></span><br>
		roof heater on  ? <span id='lue'></span><br>
		<button id="button" onclick="uclicked()">toggle device</button>
	</div>`

)