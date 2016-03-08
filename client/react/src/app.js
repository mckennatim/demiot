var env = require('json!../../../env.json')
var cfg= env[process.env.NODE_ENV||'development']
console.log(cfg)
console.log(env)

const app = document.getElementById('app')

app.innerHTML = '<h1>I am a ded demiot</h1>'