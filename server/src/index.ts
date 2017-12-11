import g from './global'
import * as net from 'net'
import * as commander from 'commander'
import Player from './player'
import Move from './move'
import Shoot from './shoot'
import { Server } from 'https';
import {has} from 'lodash'
import Session from './session'
import './packages'

//import Tick from './tick'

commander
  .option('--udp-port <n>', '', parseInt)
  .option('--tcp-port <n>', '', parseInt)
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
    g.packageManager.apply(obj, remote, false)
    
    ///////////////////////////////

    // g.server.send("reply", remote.port, remote.address);
});


g.server.on('listening', () => {
    const address = g.server.address();
    console.log('UDP Server listening on', `${address.address}:${address.port}`);
    /*
    setTimeout(() => {
        for(let i=0, j=g.actionBuffer.length; i<j; i++) {
            console.log(g.actionBuffer[i]);
        }
    }, 10);
    */
});

g.server.bind(commander.udpPort);

/// TCP stuff
const HOST = '127.0.0.1'
// Create a server instance, and chain the listen function to it
// The function passed to net.createServer() becomes the event handler for the 'connection' event
// The sock object the callback function receives UNIQUE for each connection
const tcpServer = net.createServer(function(sock) {
    
    // We have a connection - a socket object is assigned to the connection automatically
    console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
    
    // Add a 'data' event handler to this instance of socket
    sock.on('data', function(data) {
        
        console.log('DATA ' + sock.remoteAddress + ': ' + data);
        // Write the data back to the socket, the client will receive it as data from the server
        sock.write('You said "' + data + '"');


        ///>>>>>>>>>>>>>>>>>>>>>>
        let obj;
        try {
          obj = JSON.parse(data.toString());
          // console.log(obj);
        } catch (err) {
          console.error(err);
          return;
        }
    
        /// testing new package manager
        g.packageManager.apply(obj, sock, true) // tcp === true udp === false
        console.log("TCP-magic happened")
        //////////////////////////
        
    });
    
    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('CLOSED: ' + sock.remoteAddress +' '+ sock.remotePort);
    });
    
}).listen(commander.tcpPort);

console.log('TCP Server listening on ' + commander.tcpPort);
///////////////////////////

process.on('SIGTERM', () => {
    g.server.close()
    tcpServer.close()
    console.log('Closing...');
});

