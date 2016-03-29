

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

module.exports ={
	flattenProgObj: flattenProgObj,
	parseProg: parseProg
}