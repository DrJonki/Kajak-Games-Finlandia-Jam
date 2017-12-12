import g from './global'
import Player from './player'
import Move from './move'
import Shoot from './shoot'
import {has, map, filter} from 'lodash'
import Level from './level'

g.packageManager.create(
    'ping', 
    (obj, remote, isTcp) => {
        g.server.send(JSON.stringify({ package: 'pong', data: {} }), remote.port, remote.address)
    }
)

g.packageManager.create(
    'connect', 
    (obj, remote, isTcp) => {
        console.log('########################### ASDASD')
        // const ses = g.packageManager.getSession(remote.address + ':' + remote.port);
        const myId = remote.remoteAddress + ':' + remote.remotePort;
        const player = new Player(obj.data.name, remote.remoteAddress,  remote.remotePort)
        const ses = g.packageManager.joinFree(player)
        const initPack = {
            id: myId, // Unique player identifier
            faction: ses.players[myId].faction, // 0 - Simo, 1 - Russian
            health: ses.players[myId].hp, // Initial health
            level: new Level().level,
            players: filter(
                map(ses.players, (value, key) => {
                    return {
                        id: key,
                        health: value.hp,
                        faction: value.faction,
                    }
                }), value => value.id !== myId)
        }
        const sendObj = {package: 'connect', data: initPack}
        console.log(sendObj)
        return sendObj
    }
)

g.packageManager.create(
    'disconnect',
    (obj, remote) => {
        if(has(g.players, remote.address+':'+remote.port)){
            console.log(remote.address, 'disconnected!');
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

