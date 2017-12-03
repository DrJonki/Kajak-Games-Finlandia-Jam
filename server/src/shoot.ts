import g from './global'
import Vec from './vec'
import Player from './player'

export default class Shoot {
    package = 'boomHeadshot'
    playerID
    crosshairPosition
    time
    constructor(id, croshairPos) {
        this.playerID = id
        this.crosshairPosition = new Vec(croshairPos[0], croshairPos[1])
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
        console.log(g.players[id].position)
        console.log(this.crosshairPosition)
        console.log(g.players[id].position.distanceToVector(this.crosshairPosition))
        return g.players[id].position.distanceToVector(this.crosshairPosition) < 10 ? true : false
    }
}



console.log('Loaded action');