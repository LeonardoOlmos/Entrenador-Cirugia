'use strict'

const Cirujano = require('./Cirujano.js');

exports.getCirujanos = (req, res) => {
    Cirujano.find({}, (err, cirujanos) => {
        if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`});
        if (!cirujanos) return res.status(404).send({message:'No existen cirujanos registrados.' });

        res.status(200).send({cirujanos: cirujanos});
    });
}

exports.getCirujanoById = (req, res) => {
    let cirujanoId= req.params.cirujanoId;

    Cirujano.findById(cirujanoId, (err,cirujano) =>
    {
        if (err) return res.status(500).send({message: `Error al realizar la peticion: ${err}`});
        if (!cirujano) return res.status(404).send({message:'El cirujano no existe' });

        res.status(200).send({cirujano: cirujano});
    });
}

exports.newCirujano = (req,res) =>
{
   console.log('POST /api/cirujano');
   console.log(req.body);

   let cirujano = new Cirujano(req.body);

   cirujano.save((err,cirujanostored) =>{ 
    if (err) 
        res.status(500).send ({message: `Error al guardar en la base de datos: ${err}`});

    res.status(200).send({cirujano: cirujanostored});
});
}

exports.updateCirujano = (req, res) =>
{
    let cirujanoId = req.params.cirujanoId;
    let update = req.body;

    Cirujano.findByIdAndUpdate(cirujanoId, update, (err, cirujanoUpdated) =>
    {
        if (err) res.status(500).send({message: `Error al intentar actualizar el registro: ${err}`});

    res.status(200).send({ cirujano: cirujanoUpdated});
}
)}

exports.deleteCirujano = (req, res) =>
{
    let cirujanoId = req.params.cirujanoId;

    Cirujano.findById (cirujanoId, (err, cirujano) => {
        if (err) res.status(500).send({message: `Error al tratar de eliminar el registro: ${err}`});

        cirujano.remove (err => {
        if (err) res.status(500).send({message: `Error al tratar de eliminar el registro: ${err}`})
        res.status(200).send({message: `Se elimino el registro.`});
        });
    });
}