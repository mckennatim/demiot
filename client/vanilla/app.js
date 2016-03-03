var Navigo = require('navigo')

var router;
var routing = function (mode) {
  router = new Navigo(null, mode === 'hash');
  router.on({
  	'reset': function(){
			console.log('am reset')
  	},
  	'graph/:id': function(){
			console.log(`am graphing ${params.id} `)
  	},
  	'adj': function(){
			setContent('adj')
  	},
  	'selsen': function(){
			setContent('selsen')
  	}
  })
  router.on(function(){
  	console.log('at /')
  })
}


const el=(sel)=>document.querySelector(sel)

// el('#button').addEventListener('click', function(){
// 	uclicked()
// })

const setContent=(id, content)=>{
 	//console.log('setting content')
 	//console.log(el('#templ-'+id).innerHTML)
 	el('#app').innerHTML = el('#templ-'+id).innerHTML
}

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

var init = function () {
  routing('hash');
};

window.onload = init;
