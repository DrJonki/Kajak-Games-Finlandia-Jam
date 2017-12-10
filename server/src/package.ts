import g from './global'
import {includes} from 'lodash'
class PackageManager {
    packages = {}
    constructor() {
    }

    create(name, method) {
        if(!includes(this.packages, name)) {
            //console.log('creating package')
            this.packages[name] = method
        }
    }
    apply(obj, remote) {
        try {
            // console.log('ASDASD >>> ' + typeof obj.data.name)
            this.packages[obj.package](obj, remote)
        }
        catch(err){console.log(err)}
    }

}

g.packageManager = new PackageManager()
