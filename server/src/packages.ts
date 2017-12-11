import g from './global'
import Player from './player'
import Move from './move'
import Shoot from './shoot'
import {has} from 'lodash'
import Level from './level'

g.packageManager.create(
    'ping', 
    (obj, remote) => {
        g.server.send(JSON.stringify({ package: 'pong', data: {} }), remote.port, remote.address)
    }
)

g.packageManager.create(
    'connect', 
    (obj, remote, isTcp) => {
        if(!isTcp) {
            let session
            const player = new Player(obj.data.name, remote.address, remote.port)
            // find session with open player slots
            for (let key in g.packageManager.sessions) {
                if (g.packageManager.sessions[key].status === 'not-full') {
                    g.packageManager.sessions[key].addPlayer(player.name)
                    session = g.packageManager.sessions[key]
                }
            }
            console.log(remote.address, 'connected!')
            g.players[remote.address+':'+remote.port].send({ // bug: this sends join to all not just session players
                package: 'connected',
                data:{
                    message:'GLHF',
                    id: remote.address+':'+remote.port,
                    side: session.players[remote.address+':'+remote.port].side
                }
            })

            g.sendAllExcept(
                {
                package: 'join', 
                data:{ 
                    id: remote.address+':'+remote.port,
                    side: session.players[remote.address+':'+remote.port].side
                }
            }, remote.address+':'+remote.port)

            for(let key in session.players) {
                if(key !== remote.address+':'+remote.port) {
                    console.log(remote.address+':'+remote.port )
                    session.players[remote.address+':'+remote.port].send(
                        {
                            package: 'join', 
                            data:{ 
                                id: key,
                                side: session.players[key].side
                            }
                        }
                    )
                }
            }
        } else {
            const level = JSON.stringify(new Level())
            remote.write(level)
        }
    }
)

g.packageManager.create(
    'disconnect',
    (obj, remote) => {
        if(has(g.players, remote.address+':'+remote.port)){
            console.log(remote.address, 'disconnected!');
            console.log(g.players);
            g.sendAllExcept(
                {
                package: 'leave', 
                data:{ 
                    id: remote.address + ':' + remote.port,
                    side: g.players[remote.address+':'+remote.port].side
                }
            }, remote.address+':'+remote.port)
            delete g.players[remote.address+':'+remote.port]
        }
    }
)

g.packageManager.create(
    'updateMovement', 
    (obj, remote, sessionId) => {
        console.log('making the move')
        const move = new Move(
            remote.address+':'+remote.port,
            obj.data.position,
        )
    }
)

g.packageManager.create(
    'shoot', 
    (obj, remote) => {
        new Shoot(remote.address+':'+remote.port, obj.data.crosshairPosition)
    }
)

