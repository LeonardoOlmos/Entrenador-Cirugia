'use strict'

const controllers = require('./cirujano.controllers.js');

module.exports =(app) => {

app
    .post('/api/cirujano', controllers.newCirujano)
    .get('/api/cirujano/',controllers.getCirujanos)
    .get('/api/cirujano/:cirujanoId', controllers.getCirujanoById)
    .put('/api/cirujano/:cirujanoId',controllers.updateCirujano)
    .delete('/api/cirujano/:cirujanoId',controllers.deleteCirujano)
    .get('/api/programa/:cirujanoId', controllers.getPrograma)
    .post('/api/logIn/', controllers.logIn);
}