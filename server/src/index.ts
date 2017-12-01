import * as commander from 'commander'
import * as dgram from 'dgram'
import Player from './player'
import g from './global'

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

console.log('Server run(ning) at port', commander.port);

const server = dgram.createSocket('udp4');
let jsonString = '';

server.on('listening', function () {
    const address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

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

});

server.bind(commander.port);