## stack <a href="http://stackoverflow.com/questions/35781579/basic-webpack-not-working-for-button-click-function-uncaught-reference-error">Basic webpack not working</a>
answer:
<<blockquote cite="http://stackoverflow.com/questions/35781579/basic-webpack-not-working-for-button-click-function-uncaught-reference-error">
    <p>uclicked references a global function, but webpack by default will encase your bundle in an iife, protecting the global scope. Use addEventListener in your code, or expose your app as a public object using the expose-loader github.com/webpack/expose-loader – Daniel_L 17 mins ago </p>
</blockquote>

I've got a page with a button

    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, minimum-scale=1, maximum-scale=1">   
        <title>webmqtt</title>
    </head>
    <body>
        <h1>basic webpack</h1>
        <button id="button" onclick="uclicked()">toggle device</button><br>
        <span id="outp">hi</span>
        <script src="./bundle.js"></script>     
    </body>
    </html>

and an app.js

    //(function(){
        var el = function(sel) {
          return document.querySelector(sel);
        }
        console.log(el('span').innerHTML)
        el('span').innerHTML='started up'
        console.log('started up')
        function uclicked(){
            var thecmd =  "{\"heat\":0,\"src\":1,\"empty\":0}"
            console.log(thecmd);
        }
    //})();

webpack is installed and `webpack --watch` succeeds the webpack.config.js is

    module.exports={
        entry: './app.js',
        output: {
            path: __dirname,
            filename: 'bundle.js'
        }
    }

When I look at the page console.log is working and `el('span').innerHTML='started up'` works but when I push the button I get `Uncaught ReferenceError: uclicked is not defined` 

If I replace `<script src="./bundle.js"></script>` with `<script src="./app.js"></script>` and bypass webpack the button clicks fine. Why doesn't this basic webpack setup work?