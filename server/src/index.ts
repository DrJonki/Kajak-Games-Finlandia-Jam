import * as commander from 'commander';
import * as dgram from 'dgram';
import Player from './player';

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
    jsonString = message.toString();
	
	const action = JSON.parse(jsonString);
	console.log(action);

});

server.bind(commander.port, '127.0.0.1');