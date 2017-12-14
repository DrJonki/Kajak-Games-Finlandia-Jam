import Vec from '@/util/vec';

export default class Randomizer {
  public static range(min: number, max: number) {
    return Math.random() * (max - min) + min;
  }

  public static insideCircle(radius: number) {
    const point = new Vec(Randomizer.range(-1, 1), Randomizer.range(-1, 1)).normalized;

    return new Vec(
      point.x * Math.sqrt(Randomizer.range(0, 1)) * radius,
      point.y * Math.sqrt(Randomizer.range(0, 1)) * radius,
    );
  }
}
