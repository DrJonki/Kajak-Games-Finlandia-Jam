import g from './global'

export default class Vec {
    package = 'Vec'
    x
    y

    constructor(x, y) {
        this.x = x
        this.y = y
    }

    distanceToVector(vec) {
        return Math.sqrt((vec.x - this.x)*(vec.x - this.x) + (vec.y - this.y)*(vec.y - this.y))
    }
}
console.log('Loaded tick');