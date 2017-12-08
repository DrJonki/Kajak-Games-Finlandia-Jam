import g from './global'
import * as commander from 'commander'
import './packages'
import Player from './player'
import Move from './move'
import Shoot from './shoot'
import { Server } from 'https';
import {has} from 'lodash'
//import Tick from './tick'

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

g.server.on('error', (err) => {
  console.log(`server error:\n${err.message}`);
  g.server.close();
});

g.server.on('message', function (message, remote) {
    // console.log(message.toString());

    let obj;
    try {
      obj = JSON.parse(message.toString());
      // console.log(obj);
    } catch (err) {
      console.error(err);
      return;
    }

    /// testing new package manager
    g.packageManager.apply(obj, remote)
    
    ///////////////////////////////

    // g.server.send("reply", remote.port, remote.address);
    console.log(obj)
});


g.server.on('listening', () => {
    const address = g.server.address();
    console.log('UDP Server listening on', `${address.address}:${address.port}`);
    setTimeout(() => {
        for(let i=0, j=g.actionBuffer.length; i<j; i++) {
            console.log(g.actionBuffer[i]);
        }
    }, 10);
});

g.server.bind(commander.port);

process.on('SIGTERM', () => {
    g.server.close();
    console.log('Closing...');
});

