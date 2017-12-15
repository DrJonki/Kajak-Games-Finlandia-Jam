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

const handleMessage = (data: any, socket: Socket) => {
  try {
    if (!has(data, 'package')) {
      throw new Error(`Invalid socket message: no 'package' property`);
    }

    router.invoke(data.package, data.data, socket);
  } catch (err) {
    console.error(err);
  }
};

const tcpServer = net.createServer((sock) => {
  const url = `${sock.remoteAddress}:${sock.remotePort}`;
  console.log('TCP connected:', url);

  const sockInstance = socketContainer[url] = new Socket(url, socketContainer, sock, udpServer);

  sock.on('error', (err) => {
    console.error(err);
  }).on('data', (data) => {
    const obj = JSON.parse(data.toString());

    if (obj.package === 'info') {
      sockInstance.send('info', {
        udpId: url,
      }, true);

      return;
    }

    handleMessage(obj, sockInstance);
  }).on('close', () => {
    handleMessage(new Buffer(JSON.stringify({ package: 'disconnect' })), sockInstance);
    delete socketContainer[url];

    console.log('TCP disconnected', url);
  });
}).on('listening', () => {
  console.log('TCP Server listening on', `${tcpServer.address().address}:${tcpServer.address().port}`);
}).on('error', (err) => {
  console.error(err);
}).listen(commander.tcpPort, hostname);

udpServer = dgram.createSocket('udp4').on('error', (err) => {
  console.error(err);
}).on('message', (message, remote) => {
  const url = `${remote.address}:${remote.port}`;
  console.log('UDP message -', url, ':', message.toString());

  const obj = JSON.parse(message.toString());

  if (!has(obj, 'udpId')) {
    return;
  }

  socketContainer[obj.udpId].setUdpPort(remote.port);

  handleMessage(obj, socketContainer[obj.udpId]);
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
}).on('SIGINT', () => {
  handleClose();
});
