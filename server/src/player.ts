import g from './global'
import Vec from './vec'
export default class Player {
    id
    name
    ip
    port
    side = g.simoIsInDaHouse
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
        g.simoIsInDaHouse = 1;
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
        this.respawn(3000);
    }
    respawn(time) {
        setTimeout(()=>
        {
            this.hp = 100;
            this.position.x = Math.random()*1024
            this.position.y =  Math.random()*600
            this.send({package:'respawn', data:{position: [this.position.x, this.position.y]}})
        }
        ,time)
    }
}
console.log('Loaded player');