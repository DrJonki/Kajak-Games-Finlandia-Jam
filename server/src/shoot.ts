import g from './global'
import Vec from './vec'
import Player from './player'

export default class Shoot {
    package = 'boomHeadshot'
    playerID
    playerPosition
    croshairPosition
    time
    constructor(id, playerPos, croshairPos) {
        this.playerID = id
        this.playerPosition = new Vec(playerPos[0], playerPos[1])
        this.croshairPosition = new Vec(croshairPos[0], croshairPos[1])
        this.do()
    }

    do(){
    }
    detectHitWithPlayer(id){
        return g.players[id].position.distanceToVector(this.croshairPosition) < 10 ? true : false
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