//(function(){
	var el = function(sel) {
	  return document.querySelector(sel);
	}
	console.log(el('span').innerHTML)
	el('span').innerHTML='started up'
	console.log('started up')
	el('button').addEventListener('click', function(){
		uclicked();
	})
	function uclicked(){
		var thecmd =  "{\"heat\":0,\"src\":1,\"empty\":0}"
		console.log(thecmd);
	}
//})();

