import g from './global'
import vec from './vec'

export default class Tick {
    package = 'Tick'
    number = ++g.tickCount
    status
    action
    position

    // movePlayers(){
    //     for(let key in g.players) {
    //         if(this.validateMovement([key])) {

    //         }
    //     }
    // }

    updateStatus(){

    }

    doActions(){

    }

    pushToSequence(){

    }

    // validateMovement(playerId) {
    //     if(g.tickCount > 1) {
    //         newPos = g.actionBuffer.pos
    //         if(newPos.distanceToVector())
    //     }
    // }

    // constructor() {
    //     this.pos = this.movePlayers()
    //     this.status = this.updateStatus()
    // }
}
console.log('Loaded tick');

/* Example of tick that server sends to client

{
    package: 'tick',
    number: 1337,
    status: {player1ID: {health: 35, ammo: 34, clip: 12}, ... },
    action: {player1ID: 'fire', player2Id: 'idle' ... },
    position: {player1ID: {x:0, y:0}, ... }

}
*/