import * as express from 'express';
import {validate} from './validator';

const app = express();
app.use(express.json());

app.post('/validate', (req: express.Request, res: express.Response) => {
  const resp = validate(req.body);
  res.json(resp);
});

app.listen(8080, () => {
  console.log('docvalidationsvc started ...');
});
