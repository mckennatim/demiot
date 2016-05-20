var path = require('path');
module.exports={
	entry: './app.js',
	output: {
		path: __dirname,
		filename: 'bundle.js'
	},
  loaders: [
    { test: path.join(__dirname, 'es6'), loader: 'babel-loader' },
    { test: /\.css$/, loader: "style!css?modules" },
    { test: /\.html$/, loader: "babel!es6-template-string?context=styles" }
  ]	
}