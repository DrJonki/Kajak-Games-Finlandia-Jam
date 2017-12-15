import Config from '@/services/config';
import Randomizer from '@/util/randomizer';

const enum PropType {
  RockCircular,
  HouseRectangular,
  Last,
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
  private readonly mLevel = {
    props: [] as IProp[],
  };
  private mIdCounter = 0;

  public constructor() {
    const numProps = Math.floor(Randomizer.range(Config.mapObjects.min, Config.mapObjects.max));

    for (let i = 0; i < numProps; ++i) {
      const type = Math.floor(Math.random() * (PropType.Last - 0.01));
      let attribs: any;

      switch (type) {
        case PropType.RockCircular: {
          attribs = {
            radius: Randomizer.range(8 * Config.objectRadiusMult, 25 * Config.objectRadiusMult),
          };
          break;
        }
        case PropType.HouseRectangular: {
          attribs = {
            size: [
              Randomizer.range(25 * Config.objectRadiusMult, 50 * Config.objectRadiusMult),
              Randomizer.range(25 * Config.objectRadiusMult, 50 * Config.objectRadiusMult),
            ],
          };
          break;
        }
        default: {
          throw new Error('Invalid prop generated');
        }
      }

      this.mLevel.props.push(Object.assign(attribs, {
        id: String(this.mIdCounter++),
        type,
        position: Randomizer.insideCircle(Config.mapRadius).array,
        angle: Math.random() * 359,
      }));
    }
  }

  public get level() {
    return this.mLevel;
  }
}
