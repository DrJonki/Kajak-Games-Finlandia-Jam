import g from './global'

export default class Vec {
    package = 'Vec'
    x
    y

    constructor(x, y) {
        this.x = x
        this.y = y
    }

    length() {
        return Math.sqrt(this.x * this.x + this.y * this.y)
    }

    minus(vec) {
        return new Vec(this.x-vec.x, this.y-vec.y)
    }

    plus(vec) {
        return new Vec(this.x + vec.x, this.y + vec.y)
    }

    distanceToVector(vec) {
        return Math.sqrt((vec.x - this.x)*(vec.x - this.x) + (vec.y - this.y)*(vec.y - this.y))
    }
}
console.log('Loaded tick');