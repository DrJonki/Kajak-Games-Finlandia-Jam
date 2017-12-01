import * as commander from 'commander';
import * as dgram from 'dgram';
import Player from './player';

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

console.log('Server run(ning) at port', commander.port);

const server = dgram.createSocket('udp4');
let jsonString = '';

server.on('listening', () => {
  const address = server.address();
  console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', (message, remote) => {
  jsonString = message.toString();
	
	const action = JSON.parse(jsonString);
  console.log(message.toString());
});

server.bind(commander.port);

process.on('SIGTERM', () => {
  server.close();
  console.log('Closing...');
});