import g from './global'
import {includes} from 'lodash'
import {has} from 'lodash'
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
            console.log(' >>>>>>> ')
            console.log( obj.package )
            if(has(this.packages, obj.package)) {
                this.packages[obj.package](obj, remote, protocol)
            }
        }
        catch(err){console.log(err)}
    }
    getSession(id) {
        for(let key in g.packageManager.sessions) {
            for (let player in g.packageManager.sessions[key].players) {
                if (id === player) {
                    return g.packageManager.sessions[key]
                }
            }
        }
    }

}
console.log('loaded packageManager')