import * as commander from 'commander'
import Player from './player'
import g from './global'
import Tick from './tick'

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

    // g.server.send("reply", remote.port, remote.address);

    switch(obj.package) {
        case 'connection':
            if(obj.connection === 'connect') {
                new Player(obj.name, remote.address, remote.port)
                console.log(remote.address, 'connected!');
                g.sendAll({asd:'LOL'})
                new Tick()

            } else if(obj.connection === 'disconnect') {
              delete g.players[obj.ip]
              console.log(remote.address, 'disconnected!');
            }
            console.log(g.players);
            break;
    }
});

g.server.on('listening', () => {
  const address = g.server.address();
  console.log('UDP Server listening on', `${address.address}:${address.port}`);
});

g.server.bind(commander.port);

process.on('SIGTERM', () => {
    g.server.close();
    console.log('Closing...');
});

