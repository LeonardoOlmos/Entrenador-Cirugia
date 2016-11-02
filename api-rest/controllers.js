
const Product = require('./models/product');


exports.getProducts = (req, res) => {
    Product.find({}, (err, products) => {
        if (err) return res.status(500).send({message: `Error al realizar la peticion: ${err}`});
        if (!products) return res.status(404).send({message:'No existen productos' });

        res.status(200).send({products: products});
    });
}

exports.getProductById = (req, res) => {
    let productId= req.params.productId;

    Product.findById(productId, (err,product) =>
    {
        if (err) return res.status(500).send({message: `Error al realizar la peticion: ${err}`});
        if (!product) return res.status(404).send({message:'El producto no existe' });

        res.status(200).send({product: product});
    });
}

exports.newProduct = (req,res) =>
{
   console.log('POST /api/product');
   console.log(req.body);

   let product = new Product();
   product.name = req.body.name;
   product.picture = req.body.picture;
   product.price = req.body.price;
   product.category = req.body.category;
   product.description = req.body.description;

   product.save((err,productStored) =>{ 
    if (err) 
        res.status(500).send ({message: `Error al salvar en la base de datos: ${err}`});

    res.status(200).send({product: productStored});
});
}

exports.updateProduct = (req, res) =>
{
    let productId = req.params.productId;
    let update = req.body;

    Product.findByIdAndUpdate(productId, update, (err, productUpdated) =>
    {
        if (err) res.status(500).send({message: `Error al tratar de actualizar el registro: ${err}`});

    res.status(200).send({ product: productUpdated});
}
)}

exports.deleteProduct = (req, res) =>
{
    let productId = req.params.productId;

    Product.findById (productId, (err, product) => {
        if (err) res.status(500).send({message: `Error al tratar de eliminar el registro: ${err}`});

        product.remove (err => {
        if (err) res.status(500).send({message: `Error al tratar de eliminar el registro: ${err}`})
        res.status(200).send({message: `Se elimino el registro.`});
        });
    });
}

