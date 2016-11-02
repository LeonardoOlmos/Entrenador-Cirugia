'use strict'

const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const CirujanoSchema = Schema ({
    nombre: {type: String, required: true},
    apPat: {type: String, required: true},
    apMat: {type: String, required: true},
    iniciales: {type: String, required: true},
    usuario: {type: String, required: true, index: {unique: true}},
    password: {type: String, required: true},
    edad: {type: Number, required: true},
    sexo: {type: String, required: true, enum: ['M','F']},
    especialidad: {type: String, required: true},
    anioRes: {type: Number, required: true},
    manoDominante: {type: String, required: true, enum: ['I','D','A']},
    experiencia: {
        pregunta1: {type: String, enum: ['Si','No']},
        pregunta2: {type: String, enum: ['0 a 10 veces','11 a 50 veces','51 a 100 veces','Mas de 100 veces']},
        pregunta3: {type: String, enum: ['0 a 10 veces','11 a 50 veces','51 a 100 veces','Mas de 100 veces']},
        pregunta4: {type: String, enum: ['0 a 10 veces','11 a 50 veces','51 a 100 veces','Mas de 100 veces']}
    }
});

module.exports = mongoose.model('Cirujano', CirujanoSchema);