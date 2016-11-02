'use strict'

const express = require('express');
const bodyParser= require('body-parser');
const mongoose = require('mongoose');



const app = express();
const port = process.env.PORT || 3001;

app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

const cirujanosRoutes = require ('./cirujano/cirujano.routes.js')(app);

mongoose.connect('mongodb://localhost:27017/cirugia',(err,res) =>{
    if (err) {
           return console.log(`Error al conectar a la base de datos: ${err}`);
}
    app.listen(port, () => {
    console.log(`API CIRUGÍA corriendo en http://localhost:${port}`);
});
});