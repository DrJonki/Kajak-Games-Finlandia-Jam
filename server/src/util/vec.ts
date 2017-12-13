export default class Vec {
  public x: number;
  public y: number;

  constructor(x: number, y: number) {
    this.x = x;
    this.y = y;
  }

  public get length() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  }

  public minus(vec: Vec) {
    return new Vec(this.x - vec.x, this.y - vec.y);
  }

  public plus(vec: Vec) {
    return new Vec(this.x + vec.x, this.y + vec.y);
  }

  public distanceToVector(vec) {
    return Math.sqrt((vec.x - this.x) * (vec.x - this.x) + (vec.y - this.y) * (vec.y - this.y));
  }
}
