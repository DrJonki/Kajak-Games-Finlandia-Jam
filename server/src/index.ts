import * as commander from 'commander'
import * as dgram from 'dgram'
import Player from './player'
import g from './global'

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

const server = dgram.createSocket('udp4');

server.on('error', (err) => {
  console.log(`server error:\n${err.message}`);
  server.close();
});

server.on('message', function (message, remote) {
    console.log(message.toString());

    let obj;
    try {
      obj = JSON.parse(message.toString());
      console.log(obj);
    } catch (err) {
      console.error(err);
      return;
    }

    server.send("reply", remote.port, remote.address);

    switch(obj.package) {
        case 'connection':
            if(obj.connection === 'connect') {
                g.players[obj.name] = new Player(obj.name, remote.address, remote.port)
                console.log(obj.name, 'connected!');
            } else if(obj.connection === 'disconnect') {
              delete g.players[obj.name]
              console.log(obj.name, 'disconnected!');
            }
            console.log(g.players);
            break;
    }
});

server.on('listening', () => {
  const address = server.address();
  console.log('UDP Server listening on', `${address.address}:${address.port}`);
});

server.bind(commander.port);

process.on('SIGTERM', () => {
  server.close();
  console.log('Closing...');
});
