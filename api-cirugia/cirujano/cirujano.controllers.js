'use strict'

const spawn = require('child_process').spawn;
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

   let cirujano = new Cirujano(req.body.cirujano);

   cirujano.save((err,cirujanostored) =>{ 
    if (err){ 
        console.log(err);
        res.status(500).send ({message: `Error al guardar en la base de datos: ${err}`});
    }
    else
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

exports.logIn = (req, res) => {
    let usuario = req.body.usuario;
    console.log(usuario);
    Cirujano.findOne ({'usuario' : usuario.usuario}, (err, cirujano) => {
        if (err) {
            //console.log(err);
            res.status(500).send({message: `Error al iniciar sesión: ${err}`});}
        else {
            console.log(cirujano);
            if (cirujano){
             if (usuario.contrasena == cirujano.contrasena){
                 res.json({exito:true,  message: 'Inicio exitoso',_id:cirujano._id, usuario:cirujano.usuario})
                                                             }
             else{
                res.json({exito:false, message: 'Error: Contraseña incorrecta'})
                }
                        }
            else{
                res.json({exito:false, message: 'Error: Usuario incorrecto'})
                 }
        }
    });
}  

exports.getPrograma = (req, res) =>
{    
    const camaras = spawn('Camaras2.exe',[req.params.cirujanoId]);

    res.json({message: 'Exito'})
}