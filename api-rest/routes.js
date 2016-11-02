'use strict';

const controllers = require('./controllers.js');

module.exports =(app) => {

app
    .get('/api/product/',controllers.getProducts)
    .get('/api/product/:productId', controllers.getProductById)
    .post('/api/product', controllers.newProduct)
    .put('/api/product/:productId',controllers.updateProduct)
    .delete('/api/product/:productId',controllers.deleteProduct);

}