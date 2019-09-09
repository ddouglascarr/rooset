const express = require('express')
const app = express()
const port = 9300
const bodyParser = require('body-parser');

app.use(express.json());

app.get('/', (req, res) => res.send('Hello World!'))
app.post('/', (req, res) => {
  console.log(JSON.stringify(req.body, null, 2));
  res.send('test');
});

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
