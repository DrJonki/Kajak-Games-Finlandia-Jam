import g from './global'
import {includes} from 'lodash'
export default class PackageManager {
    name
    aply
    packages = {}
    constructor() {
        g.packageManager = this
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



console.log('Loaded Package');
