export default class Player {
    name
    ip
    port
    constructor(name, ip, port) {
        this.name = name
        this.ip = ip
        this.port = port
    }
    
}
console.log('Loaded player');