'use strict'

const express = require('express');
const bodyParser= require('body-parser');
const mongoose = require('mongoose');



const app = express();
const port = process.env.PORT || 3001;

app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

//Ejemplo para mostrar el nombre mediante lo tecleado en la barra buscadora
/*app.get('/hola/:name',(req,res) => {
    res.send ({message: `Hola ${req.params.name}!`});
});*/

const routes = require('./routes.js')(app);

mongoose.connect('mongodb://localhost:27017/shop',(err,res) =>{
    if (err) {
           return console.log(`Error al conectar a la base de datos: ${err}`);
}
    app.listen(port, () => {
    console.log(`API REST corriendo en http://localhost:${port}`);
});
});

