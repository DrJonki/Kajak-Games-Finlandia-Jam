import g from './global'
import {includes} from 'lodash'
import {has} from 'lodash'
import Session from './session'
export default class PackageManager {
    packages = {}
    sessions = {}
    constructor() {
    }

    create(name, method) {
        if(!has(this.packages, name)) {
            this.packages[name] = method
        }
    }

    apply(obj, remote, protocol) {
        try {
            const sender = remote.address + ':' + remote.port
            // console.log(' >>>>>>> ')
            // console.log( obj.package )
            if(has(this.packages, obj.package)) {
                return this.packages[obj.package](obj, remote, protocol)
            }
        }
        catch(err){console.log(err)}
    }

    getSession(id) {
        console.log(id)
        for(let key in g.packageManager.sessions) {
            console.log('session '+ key)
            for (let player in g.packageManager.sessions[key].players) {
                if (id === player) {
                    return g.packageManager.sessions[key]
                }
            }
        }
    }

    joinFree(player) {
        for (let key in this.sessions) {
            if (this.sessions[key].status === 'not-full') {
                this.sessions[key].addPlayer(player)
                console.log('added to excisting session: ' + key)
                return this.sessions[key]
            }
        }
        const newSes = new Session()
        newSes.addPlayer(player)
        return newSes
    }

    createSession() {

    }

}
console.log('loaded packageManager')