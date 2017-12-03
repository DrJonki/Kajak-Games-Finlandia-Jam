import g from './global'
import Vec from './vec'
import Player from './player'

export default class Shoot {
    package = 'boomHeadshot'
    playerID
    crosshairPosition
    time
    constructor(id, crosshairPos) {
        this.playerID = id
        this.crosshairPosition = new Vec(crosshairPos[0], crosshairPos[1])
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
        console.log('target position: ')
        console.log(g.players[id].position)
        console.log('crosshair position: ')
        console.log(this.crosshairPosition)
        console.log(g.players[id].position.distanceToVector(this.crosshairPosition))
        console.log('-----------------------------')
        return g.players[id].position.distanceToVector(this.crosshairPosition) < 20 ? true : false
    }
}



console.log('Loaded action');