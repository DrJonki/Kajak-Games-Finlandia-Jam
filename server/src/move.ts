import g from './global'
import Vec from './vec'
import Player from './player'

export default class Move {
    package = 'Move'
    playerID
    position
    time
    constructor(id, pos) {
        this.playerID = id
        this.position = new Vec(pos[0], pos[1])
        this.time = Date.now()
        this.do()
    }

    do(){
        if(this.validate()) {
            g.players[this.playerID].position = this.position
            //g.sendAllExcept()
            // console.log(g.players[this.playerID].position)
        } else {
            this.sendPlayerBackToPreviousPosition()
        }
    }

    validate(){
        return g.players[this.playerID].position.distanceToVector(this.position) < g.maxSpeed ? 
            true : false
    }

    sendPlayerBackToPreviousPosition() {
    }
}



console.log('Loaded action');

/*
example of what client sends to server
{
    package: 'Action',
    data:{
        postion: {x:0, y:0},
        croshair {x:0, y:0}
        action: 'mouse1-down'
    }
}
*/