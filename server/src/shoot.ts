import g from './global'
import Vec from './vec'
import Player from './player'

export default class Shoot {
    package = 'boomHeadshot'
    playerID
    croshairPosition
    time
    constructor(id, croshairPos) {
        this.playerID = id
        this.croshairPosition = new Vec(croshairPos[0], croshairPos[1])
        for(let key in g.players) {
            if(this.detectHitWithPlayer(key) && this.detectClearLineOfSight(g.players[key])) {
                g.players[key].die(g.players[this.playerID])
            }
        }
    }
    detectClearLineOfSight(id) {
        return true
    }
    detectHitWithPlayer(id){
        console.log('detecting hit with '+ id)
        return g.players[id].position.distanceToVector(this.croshairPosition) < 10 ? true : false
    }
}



console.log('Loaded action');