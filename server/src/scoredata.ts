import g from './global'
import Vec from './vec'

export default class ScoreData {
    id
    session
    faction = 0
    hp = 100
    ammo = 50
    kills = 0
    deaths = 0
    position = new Vec(0,0)

    constructor(sessionId, id) {
        this.session = sessionId
        this.id = id
    }

    die(killer){
        this.hp = 0
        ++this.deaths
        g.sendAllInSession({package:'dead', data:{
            id: this.id,
            message: killer.name + ' made you his bitch'}}, this.session)
        this.respawn(3000);
    }

    respawn(time) {
        setTimeout(() =>
        {
            this.hp = 100;
            this.position.x = Math.random()*1024
            this.position.y =  Math.random()*600
            g.sendAllInSession({package:'respawn', data:{
                id: this.id,
                position: [this.position.x, this.position.y]}}, this.session)
        }
        ,time)
    }
}
console.log('Loaded ScoreData');