console.log('stuff after ')
var Navigo = require('navigo')

var router;
var routing = function (mode) {
  router = new Navigo(null, mode === 'hash');
  router.on({
  	'reset': ()=>Reset(),
  	'graph/:id': function(params){
			console.log(`am graphing ${params.id} `)
  	},
  	'adj/:id': (params)=>Adj(params.id),
  	'selsen': ()=>Selsen(),
  })
  router.on(function(){
  	console.log('at /')
  })
}

const el=(sel)=>document.querySelector(sel)

const Adj = function(id){
	let tstr = generateTemplateString(el('#templ-adj').innerHTML)
	let templ = tstr({dogname: 'Fritz'})
	el('#app').innerHTML =templ
	el('#button').addEventListener('click', function(){
		onoff=!onoff;
		var thecmd =  "{\"heat\":"+onoff*1+",\"src\":1,\"empty\":0}"
		console.log(thecmd);
		client.publish(cmd, thecmd)
	})	
}
const Selsen = function(){
	let tstr = generateTemplateString(el('#templ-selsen').innerHTML)
	let templ = tstr({cat: 'Mabibi'})
	el('#app').innerHTML =templ
}

const Reset = function(){
	let tstr = generateTemplateString(el('#templ-reset').innerHTML)
	let templ = tstr()
	el('#app').innerHTML =templ
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

var generateTemplateString = (function(){
  var cache = {};
  function generateTemplate(template){
    var fn = cache[template];
    if (!fn){
      // Replace ${expressions} (etc) with ${map.expressions}.
      var sanitized = template
        .replace(/\$\{([\s]*[^;\s]+[\s]*)\}/g, function(_, match){
            return `\$\{map.${match.trim()}\}`;
        })
        // Afterwards, replace anything that's not ${map.expressions}' (etc) with a blank string.
        .replace(/(\$\{(?!map\.)[^}]+\})/g, '');
      fn = Function('map', `return \`${sanitized}\``);
    }
    return fn;
  };
return generateTemplate;
})();

window.onload = init;
