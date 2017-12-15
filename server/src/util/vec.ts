import { isArray } from 'lodash';

export default class Vec {
  public x: number;
  public y: number;

  constructor(x: number, y: number);
  constructor(xy: [number, number]);
  constructor(...args: any[]) {
    if (isArray(args[0])) {
      this.x = args[0][0];
      this.y = args[0][1];
      return;
    }
    this.x = args[0];
    this.y = args[1];
  }

  public get length() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  }

  public get normalized() {
    const len = this.length;
    return new Vec(this.x / len, this.y / len);
  }

  public get array() {
    return [this.x, this.y];
  }

  public minus(vec: Vec) {
    return new Vec(this.x - vec.x, this.y - vec.y);
  }

  public plus(vec: Vec) {
    return new Vec(this.x + vec.x, this.y + vec.y);
  }

  public distanceToVector(vec: Vec) {
    return Math.sqrt((vec.x - this.x) * (vec.x - this.x) + (vec.y - this.y) * (vec.y - this.y));
  }
}
