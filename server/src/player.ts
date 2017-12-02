import g from './global'
export default class Player {
    id
    name
    ip
    port
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
}
console.log('Loaded player');