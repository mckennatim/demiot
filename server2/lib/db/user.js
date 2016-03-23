//var db = require('../cfg').db();
var env = require('../../../env.json')
var cfg= env[process.env.NODE_ENV||'development']
var db = cfg.db
var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var usersSchema = new Schema({
	name: {type:String, index:{unique: true}},
	email: String,
	apikey: String,
	lists: Array,
	timestamp: String,
	role: String
}, { strict: false });
mongoose.connect(db.url);


module.exports = mongoose.model('User', usersSchema);
