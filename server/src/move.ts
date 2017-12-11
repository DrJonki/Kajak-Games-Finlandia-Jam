import g from './global'
import Vec from './vec'
import Player from './player'
import {has} from 'lodash'

export default class Move {
    package = 'Move'
    playerID
    position
    session
    time
    constructor(id, pos) {
        this.playerID = id
        this.session = g.packageManager.getSession(this.playerID)
        this.position = new Vec(pos[0], pos[1])
        this.do()
    }

    do(){
        if(has(g.players, this.playerID)) {
            //console.log('Cykaaaa!')
            /*
            console.log({
                package: 'updateMovement',
                data: {
                    id: this.playerID,
                    position: [this.position.x, this.position.y]
                },
            })
            */
            g.packageManager.sessions
            // finding the correct session and doing the movement
            for(let key in g.packageManager.sessions) {
                for (let player in g.packageManager.sessions[key].players) {
                    if (this.playerID === player) {
                        g.packageManager.sessions[key].players[player].position = this.position
                    }
                }
            }
            
            // players[this.playerID].position = this.position
            g.sendAllExcept (
                {
                    package: 'updateMovement',
                    data: {
                        id: this.playerID,
                        position: [this.position.x, this.position.y]
                    },
                },
                this.playerID
            )
            // console.log(g.players[this.playerID].position)
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