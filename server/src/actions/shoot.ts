import Vec from '@/util/vec'
import Player from './player'

export default class Shoot {
  package = 'boomHeadshot'
  playerID
  session
  crosshairPosition
  time
  constructor(id, crosshairPos) {
    this.playerID = id
    this.session = g.packageManager.getSession(this.playerID)
    this.crosshairPosition = new Vec(crosshairPos[0], crosshairPos[1])
    for(let key in this.session.players) {
      if(this.detectHitWithPlayer(key)) {
        this.session.players[key].die(this.session.players[this.playerID])
      }
    }
  }

  detectClearLineOfSight(id) {
    return true
  }

  detectHitWithPlayer(id){
    console.log('detecting hit with '+ id)
    console.log('target position: ')
    console.log(this.session.players[id].position)
    console.log('crosshair position: ')
    console.log(this.crosshairPosition)
    console.log(this.session.players[id].position.distanceToVector(this.crosshairPosition))
    console.log('-----------------------------')
    return this.session.players[id].position.distanceToVector(this.crosshairPosition) < 20 ? true : false
  }

}
