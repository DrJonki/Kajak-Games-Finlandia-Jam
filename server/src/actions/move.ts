import Vec from '@/util/vec'
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
    console.log('DOOOING MOVE')
    if(has(g.players, this.playerID)) {
      g.packageManager.getSession(this.playerID).players[this.playerID].position = this.position
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
