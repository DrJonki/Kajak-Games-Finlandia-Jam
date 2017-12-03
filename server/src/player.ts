import g from './global'
import Vec from './vec'
export default class Player {
    id
    name
    ip
    port
    side
    hp = 100
    ammo = 50
    kills = 0
    deaths = 0
    position = new Vec(0,0)
    speed = new Vec(0,0)
    acceleration = new Vec(0,0)

    constructor(name, ip, port) {
        this.id = ip+':'+port
        this.name = name
        this.ip = ip
        this.port = port
        g.players[this.id] = this
    }
    send(obj) {
        const json = JSON.stringify(obj)
        g.server.send(json, this.port, this.ip);
        console.log('sent' + json + this.ip + ':' + this.port)
    }
    die(killer){
        this.hp = 0
        ++this.deaths
        this.send({package:'Die', data:{message: killer.name + ' made you his bitch'}})
    }
}
console.log('Loaded player');