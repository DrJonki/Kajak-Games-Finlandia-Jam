import * as commander from 'commander'
import Player from './player'
import Move from './move'
import Shoot from './shoot'
import g from './global'
import { Server } from 'https';
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

    // g.server.send("reply", remote.port, remote.address);
    console.log(obj)
    switch(obj.package) {
        case 'ping':
            g.server.send(JSON.stringify({ package: 'pong', data: {} }), remote.port, remote.address);
            break;

        case 'connect':
                const player = new Player(obj.data.name, remote.address, remote.port)
                console.log(remote.address, 'connected!')
                //g.sendAll({asd:'LOL'})
                g.players[remote.address+':'+remote.port].send({
                    package: 'connected',
                    data:{
                        message:'GLHF',
                        side: g.players[remote.address+':'+remote.port].side
                    }
                })

                g.sendAllExcept(
                    {
                    package: 'join', 
                    data:{ 
                        id: g.players[remote.address+':'+remote.port].ip + ':' + g.players[remote.address+':'+remote.port].port,
                        side: g.players[remote.address+':'+remote.port].side
                    }
                }, remote.address+':'+remote.port)

                for(let key in g.players) {
                    if(key !== remote.address+':'+remote.port) {
                        g.players[remote.address+':'+remote.port].send(
                            {
                                package: 'join', 
                                data:{ 
                                    id: g.players[key].ip + ':' + g.players[key].port,
                                    side: g.players[key].side
                                }
                            }
                        )
                    }
                }

            break
        case 'disconnect': 
                delete g.players[remote.address+':'+remote.port]
                console.log(remote.address, 'disconnected!');
                console.log(g.players);
                g.sendAllExcept(
                    {
                    package: 'leave', 
                    data:{ 
                        id: g.players[remote.address+':'+remote.port].ip + ':' + g.players[remote.address+':'+remote.port].port,
                        side: g.players[remote.address+':'+remote.port].side
                    }
                }, remote.address+':'+remote.port)

            break;
        case 'updateMovement':
            console.log('making the move')
            const move = new Move(
                remote.address+':'+remote.port,
                obj.data.position
            )
            break
        case 'boomHeadshot':
            new Shoot(remote.address+':'+remote.port, obj.data.playerPosition, obj.data.croshairPosition)
            break
    }
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

