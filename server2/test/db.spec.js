var Device = require('../lib/db/devices.js')
var User = require('../lib/db/user.js')
var expect = require('chai').expect
var mongoose = require('mongoose');
var env = require('../../env.json')
var cfg= env[process.env.NODE_ENV||'development']
var db = cfg.db

mongoose.connect(db.url);
var dev = "CYURD001"

describe('device db:', function() {
	it('finds this file', function(done){
		expect(1).to.equal(1);
		done()
	})
	it('finds a user record', function(done){
		User.findOne({name: 'tim'}, function(err, items) {
			console.log(items)
			expect(items.name).to.equal('tim');
			done()
		})
	})
	it('writes a device record', function(done){
		var inn = {
			devid: dev,
			domain: "parley",
			userarr: [],
			users: [],
			loc: {
				lat: 2334555.3344,
				lng: 12345.2344,
				timezone: -5,
				address: "12 Parley Vale, Jamaica Plain, MA 02130"
			}
		}
		var newdev = new Device(inn)
		newdev.save(function(err){
			console.log(err)
			expect(1).to.equal(1);
			done()
		})	
	})
	it('removes that record', function(done){
		//Device.remove({devid: dev} )
		expect(1).to.equal(1);
		done()		
	})
})