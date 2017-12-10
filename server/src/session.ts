import g from './global'
import ScoreData from './scoredata'

export default class Session {

    status = 'not-full'
    players = {}
    maxSpeed = 100
	simoIsInDaHouse = 0
    constructor() {

    }
    addPlayer(player) {
        this.players[player.name] = new ScoreData(this)
    }
}
console.log('Loaded Session');