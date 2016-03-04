#vanilla client

Client with basic router and css.
##questions

### on turning script type="text/content" to es6 template string
ans:
<a href="https://gist.github.com/bryanerayner/68e1498d4b1b09a30ef6">bryanerayner/generateTemplateString.js</a>
<blockquote>
No, there is not a way to do this without dynamic code generation.
However, I have created a function which will turn a regular string into a function which can be provided with a map of values, using template strings internally.   
</blockquote>

Template strings seem like a good idea for some small projects and this works fine.

    var dogname='Ulysses';
    let ttt = (
            `<div>
            <h1>read and change device</h1>
            <h4>my dog is ${dogname} </h4>
            <button id="button">toggle device</button>
        </div>`
    )   

But it is almost as ugly as jsx:) What I would like to do is have this...

    <script type="text/content" id="templ-adj">
        <div >
            <h1>read and change device</h1>
            <h4>my dog is ${dogname} </h4>
            <span  id="temp1"></span><br>
            roof heater on  ? <span id='lue'></span><br>
            <button id="button">toggle device</button>
        </div>      
    </script>

and then turn it into a template string and render it somehow like

    const el=(sel)=>document.querySelector(sel)
    var dogname='Ulysses';
    let tx = `${el('#templ-adj').innerHTML}`

But alas that doesn't work nor does any of the goofy other stuff I've tried (try to make a closure, returning a function...). Is this possible?    