'use strict'

const mongoose = require('mongoose');
const Schema = mongoose.Schema;
const bcrypt = require ('bcrypt-nodejs');

const CirujanoSchema = new Schema ({
    nombre: {type: String, required: true},
    apellidoPaterno: {type: String, required: true},
    apellidoMaterno: {type: String, required: true},
    iniciales: {type: String, required: true},
    usuario: {type: String, required: true, index: {unique: true}},
    contrasena: {type: String, required: true },
    edad: {type: Number, required: true},
    sexo: {type: String, required: true, enum: ['Masculino','Femenino']},
    especialidad: {type: String, required: true},
    anioRes: {type: Number, required: true},
    manoDominante: {type: String, required: true, enum: ['Izquierda','Derecha','Ambidiestro']},
    pregunta1: {type: String, enum: ['Si','No']},
    pregunta2: {type: String, enum: ['0-10 veces','11-50 veces','51-100 veces','Más de 100 veces']},
    pregunta3: {type: String, enum: ['0-10 veces','11-50 veces','51-100 veces','Más de 100 veces']},
    pregunta4: {type: String, enum: ['0-10 veces','11-50 veces','51-100 veces','Más de 100 veces']}
    });

module.exports = mongoose.model('Cirujano', CirujanoSchema);

