export class Time {
  private readonly mSince: number;

  public constructor(since: number) {
    this.mSince = since;
  }

  public get seconds() {
    return this.milliseconds / 1000;
  }

  public get milliseconds() {
    return Date.now() - this.mSince;
  }
}

export default class Timer {
  private mTimepoint = Date.now();

  public restart() {
    const time = this.elapsed;

    this.mTimepoint = Date.now();

    return time;
  }

  public get elapsed() {
    return new Time(this.mTimepoint);
  }
}
