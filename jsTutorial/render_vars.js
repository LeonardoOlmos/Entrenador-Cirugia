var http = require("http"),
    fs = require("fs");

http.createServer(function(req,res){
    fs.readFile("./index.html",function(err,html){

     var html_string = html.toString();
     //Expresion regular que busca en el HTML donde haya {x}
     var variables = html_string.match(/[^\{\}]+(?=\})/g);

     var nombre = "Leo";

     //variable ['nombre']
     for (var i = variables.length - 1; i >= 0 ; i--)
     {
        //Lo ejecutamos como codigo de js 
        //Para poder obtener el valor de dicha variable
        var value = eval(variables[i]);

        html_string = html_string.replace("{"+variables[i]+"}",value);
     };

     res.writeHead(200, {"Content-Type": "text/html"})
     res.write(html_string);
     res.end();
});
}).listen(8080);




