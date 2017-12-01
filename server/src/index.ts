<<<<<<< HEAD
import * as commander from 'commander'
import * as dgram from 'dgram'
import Player from './player'
import g from './global'
=======
import * as commander from 'commander';
import * as dgram from 'dgram';
import Player from './player';
import * as net from 'net';
>>>>>>> f3c232db567ce33ccdbe5bec13cb20172b90ff6f

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

console.log('Server run(ning) at port', commander.port);

const server = dgram.createSocket('udp4');
let jsonString = '';

server.on('error', (err) => {
  console.log(`server error:\n${err.message}`);
  server.close();
});

<<<<<<< HEAD
server.on('message', function (message, remote) {
    const obj = JSON.parse(message.toString());
    console.log(obj);

    switch(obj.package) {
        case 'connection':
            if(obj.connection = 'connect') {
                g.players[obj.name] = new Player(obj.name, remote.address, remote.port)
                console.log(obj.name + ' connected!');
            } else if(obj.connection = 'disconnect') {
              delete g.players[obj.name]
              console.log(obj.name + ' disconnected!');
            }
            console.log(g.players);
            break;
    }

=======
server.on('listening', () => {
  const address = server.address();
  console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', (message, remote) => {
  jsonString = message.toString();
	
	const action = JSON.parse(jsonString);
  console.log(message.toString());
>>>>>>> f3c232db567ce33ccdbe5bec13cb20172b90ff6f
});

server.bind(commander.port);

process.on('SIGTERM', () => {
  server.close();
  console.log('Closing...');
});
