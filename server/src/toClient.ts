// not yet used
import g from './global'

export default class ToClient {
    package
    data
    constructor(name, data) {
        this.package = name
        this.data = data
    }
    stringify() {
        return JSON.stringify(this)
    }
}

console.log('Loaded tick');