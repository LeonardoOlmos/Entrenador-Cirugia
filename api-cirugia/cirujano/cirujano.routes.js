'use strict'

const controllers = require('./cirujano.controllers.js');

module.exports =(app) => {

app
    .get('/api/cirujano/',controllers.getCirujanos)
    .get('/api/cirujano/:cirujanoId', controllers.getCirujanoById)
    .post('/api/cirujano', controllers.newCirujano)
    .put('/api/cirujano/:cirujanoId',controllers.updateCirujano)
    .delete('/api/cirujano/:cirujanoId',controllers.deleteCirujano);
}