import g from './global'
import ScoreData from './scoredata'

export default class Session {

    id
    status = 'not-full'
    players = {}
    maxSpeed = 100
	simoIsInDaHouse = 0
    constructor() {
        this.id = Math.random() * 999999999
        g.packageManager.sessions[this.id] = this
        console.log('#### SESSION CREATED ####')
        // console.log(this.id)
        // console.log(g.packageManager.sessions[this.id])
    }
    addPlayer(player) {
        this.players[player.id] = new ScoreData(this.id, player.id)
    }

    send(obj) {
        const json = JSON.stringify(obj)

        for(let key in this.players) {
            const playerObj = g.players[key]
            g.server.send(json, playerObj.port, playerObj.ip);
            console.log('sent' + json + playerObj.ip + ':' + playerObj.port)
		}
    }
}
console.log('Loaded Session');