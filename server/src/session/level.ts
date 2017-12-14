const enum PropType {
  RockCircular,
  HouseRectangular,
}

interface IProp {
  id: string;
  type: PropType;
  breakable?: boolean;
  health?: number;
  position: [number, number];
  angle: number;
  radius?: number;
  size?: [number, number];
}

export default class Level {
  private readonly mLevel: any = {};
  private mIdCounter = 0;

  public constructor() {
    // TODO:Generate
    this.mLevel.props = [
      {
        id: '0',
        radius: 50,
        position: [0, 0],
        type: 0,
        angle: 0,
      },
    ];
  }

  public get level() {
    return this.mLevel;
  }
}
