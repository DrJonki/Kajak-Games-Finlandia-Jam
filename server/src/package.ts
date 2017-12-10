import g from './global'
import {includes} from 'lodash'
class PackageManager {
    packages = {}
    sessions = {}
    constructor() {
    }

    create(name, method) {
        if(!includes(this.packages, name)) {
            this.packages[name] = method
        }
    }
    apply(obj, remote) {
        try {
            const sender = remote.address + ':' + remote.port
            this.packages[obj.package](obj, remote)
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

g.packageManager = new PackageManager()
import './packages'