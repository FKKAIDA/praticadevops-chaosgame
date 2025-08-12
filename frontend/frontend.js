const express = require('express');
//https://stackoverflow.com/questions/69041454/error-require-of-es-modules-is-not-supported-when-importing-node-fetch
const fetch = (...args) => import('node-fetch').then(({default: fetch}) => fetch(...args)); 
const cors = require('cors')

const app = express();

app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));
app.use(express.json());
app.use(cors());

const PORT = process.env.PORT;
const FRACTAL = process.env.FRACTAL;
const DATA = process.env.DATA;

//recebe código input para gerar um fractal
//se conseguir, cria uma entrada no histórico (código e imagem)
//se falhar, retona uma mensagem de erro
app.post('/fractal', async (req, res) => {
  const { input } = req.body;
  const response = await fetch(`http://${FRACTAL}/fractal`, {method: 'POST', body: input});

  if(response.ok){
    const buffer = await response.buffer();

    await fetch(`http://${DATA}/add-text-history`, {method: 'POST', body: input});
    await fetch(`http://${DATA}/add-image-history`, {method: 'POST', body: buffer});

    const type = response.headers.get('content-type');
    res.setHeader('Content-Type', type);
    res.status(200).send(buffer);
  }else{
    const error = await response.text()
    res.status(500).send(error);
  }

});

//lista os exemplos (códigos prontos)
app.get('/list-examples', async(req, res) => {
  const response = await fetch(`http://${DATA}/list-examples`);

  const r = await response.json();
  res.setHeader('Content-Type', 'application/json');
  res.status(200).send(r);
});

//carrega um código pronto
app.post('/load-example', async (req, res) => {
  const { input } = req.body;
  const response = await fetch(`http://${DATA}/load-example`, {method: 'POST', body: input});

  const output = await response.text();
  res.status(200).send(output);
});

//lista o histórico (código e imagem)
app.get('/list-history', async(req, res) => {
  const response = await fetch(`http://${DATA}/list-history`);

  const r = await response.json();
  res.setHeader('Content-Type', 'application/json');
  res.status(200).send(r);
});

//carrega um código pronto
app.post('/load-text-history', async (req, res) => {
  const { input } = req.body;
  const response = await fetch(`http://${DATA}/load-text-history`, {method: 'POST', body: input});

  const output = await response.text();
  res.status(200).send(output);
});

//carrega imagem pronta
app.post('/load-image-history', async (req, res) => {
  const { input } = req.body;
  const response = await fetch(`http://${DATA}/load-image-history`, {method: 'POST', body: input});

  const buffer = await response.buffer();
  const type = response.headers.get('content-type');
  res.setHeader('Content-Type', type);
  res.status(200).send(buffer);
});

app.listen(PORT, () => {
  console.log(`Node server running at http://localhost:${PORT}`);
});
