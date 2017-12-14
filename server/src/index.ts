import './alias';
import * as net from 'net';
import * as dgram from 'dgram';
import * as commander from 'commander';
import { each, has } from 'lodash';

import Socket, { ISocketContainer } from '@/util/socket';
import Router from '@/services/router';
import registerRoutes from '@/routes';

const router = new Router();
registerRoutes(router);

commander
  .option('--udp-port <n>', '', parseInt)
  .option('--tcp-port <n>', '', parseInt)
  .parse(process.argv);

const hostname = '0.0.0.0';
let udpServer: dgram.Socket;
const socketContainer: ISocketContainer = {};

const handleMessage = (data: Buffer, socket: Socket, tcp = true, udpPort?: number) => {
  try {
    const obj = JSON.parse(data.toString());

    if (!has(obj, 'package')) {
      throw new Error(`Invalid socket message: no 'package' property`);
    }

    if (!tcp && obj.package === 'handshake') {
      socket.setupUdp(udpPort, udpServer);
      socket.send('handshake', undefined, false);
      return;
    }

    router.invoke(obj.package, obj.data, socket);
  } catch (err) {
    console.error(err);
  }
};

const tcpServer = net.createServer((sock) => {
  console.log('TCP connected:', `${sock.remoteAddress}:${sock.remotePort}`);

  const sockInstance = socketContainer[sock.remoteAddress] = new Socket(sock.remoteAddress, socketContainer, sock);

  sock.on('error', (err) => {
    console.error(err);
  }).on('data', (data) => {
    handleMessage(data, sockInstance);
  }).on('close', () => {
    handleMessage(new Buffer(JSON.stringify({ package: 'disconnect' })), sockInstance);

    delete socketContainer[sock.remoteAddress];
  });
}).on('listening', () => {
  console.log('TCP Server listening on', `${tcpServer.address().address}:${tcpServer.address().port}`);
}).on('error', (err) => {
  console.error(err);
}).listen(commander.tcpPort, hostname);

udpServer = dgram.createSocket('udp4').on('error', (err) => {
  console.error(err);
}).on('message', (message, remote) => {
  if (has(socketContainer, remote.address)) {
    handleMessage(message, socketContainer[remote.address], false, remote.port);
  }
}).on('listening', () => {
  const address = udpServer.address();
  console.log('UDP Server listening on', `${address.address}:${address.port}`);
});

udpServer.bind(commander.udpPort, hostname);

const handleClose = () => {
  each(socketContainer, (socket) => {
    socket.send('restart', undefined, false);
  });

  udpServer.close();
  tcpServer.close();
  console.log('Closing...');
};

const handleCrash = () => {
  each(socketContainer, (socket) => {
    socket.send('crash', undefined, false);
  });
};

process.on('uncaughtException', () => {
  handleCrash();
});

process.on('unhandledRejection', () => {
  handleCrash();
});

process.on('SIGTERM', () => {
  handleClose();
});
