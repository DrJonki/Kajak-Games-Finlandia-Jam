import Vec from '@/util/vec';
import Socket from '@/util/socket';
import Config from '@/services/config';
import * as randName from 'node-random-name';
import Session from '@/session';
import * as uuid from 'uuid/v4';
import { size } from 'lodash';
import { setTimeout } from 'timers';

type TOnDisconnectFunc = (reason: number) => void;

export const enum Faction {
  Simo,
  Russian,
}

export default class Player {
  public static readonly radius = 30 * Config.objectRadiusMult;

  public readonly socket: Socket;
  public readonly id = uuid();
  public readonly faction: Faction;
  public readonly name: string;
  private readonly mSession: Session;
  private readonly mOnDisconnect: TOnDisconnectFunc;
  private mHealth: number;
  private mPosition = new Vec(0, 0);
  private mAngle = 0;

  constructor(faction: Faction, data: any, socket: Socket, session: Session, onDisconnect: TOnDisconnectFunc) {
    this.faction = faction;
    this.name = faction === Faction.Simo ? 'Simo' : randName();
    this.socket = socket;
    this.mOnDisconnect = onDisconnect;
    this.mSession = session;
    this.mHealth = Config.initialHealth[faction];
  }

  public shootPosition(vec: Vec) {
    // TODO: calculate spread
    return vec;
  }

  public damage(amount: number) {
    const wasDead = this.dead;

    this.mHealth -= amount;

    if (!wasDead && this.dead) {
      setTimeout(() => {
        this.mHealth = Config.initialHealth[this.faction];

        this.mSession.broadcast(`respawn:${this.id}`, {
          health: this.health,
          position: [
            (Math.random() + 1) / 2 * 1000,
            (Math.random() + 1) / 2 * 800,
          ],
        }, true);
      }, this.respawnTime);
    }

    return this.mHealth;
  }

  public set position(pos: Vec) {
    this.mPosition = pos;
  }

  public get position() {
    return this.mPosition;
  }

  public set angle(angle: number) {
    this.mAngle = angle;
  }

  public get angle() {
    return this.mAngle;
  }

  public get health() {
    return this.mHealth;
  }

  public get dead() {
    return this.mHealth <= 0;
  }

  public get respawnTime() {
    return this.faction === Faction.Simo ? 15000 : 5000;
  }
}
