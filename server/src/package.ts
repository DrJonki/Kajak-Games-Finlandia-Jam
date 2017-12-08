import g from './global'
import {includes} from 'lodash'
class PackageManager {
    name
    aply
    packages = {}
    constructor() {
    }

    create(name, method) {
        this.name = name
        if(!includes(this.packages, name)) {
            this.packages['name'] = method
        }
    }
    apply(obj, remote) {
        try {
            this.packages[obj.data.name](obj, remote)
        }
        catch(err){console.log(err)}
    }

}

g.packageManager = new PackageManager()
