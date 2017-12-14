import Vec from '@/util/vec';
import Socket from '@/util/socket';
import Session from '@/session';
import * as uuid from 'uuid/v4';
import { size } from 'lodash';
import { setTimeout } from 'timers';

type TOnDisconnectFunc = (reason: number) => void;

export const enum Faction {
  Simo,
  Russian,
}

const initialHealth = [
  /* Simo */    1000,
  /* Russian */ 100,
];

export default class Player {
  public static readonly radius = 30.0;

  public readonly socket: Socket;
  public readonly id = uuid();
  public readonly faction: Faction;
  public readonly name: string;
  private readonly mSession: Session;
  private readonly mOnDisconnect: TOnDisconnectFunc;
  private mHealth: number;
  private mPinger: NodeJS.Timer;
  private mTimeout: NodeJS.Timer;
  private mPosition = new Vec(0, 0);

  constructor(faction: Faction, data: any, socket: Socket, session: Session, onDisconnect: TOnDisconnectFunc) {
    this.faction = faction;
    this.name = data.name;
    this.socket = socket;
    this.mOnDisconnect = onDisconnect;
    this.mSession = session;

    this.mHealth = initialHealth[faction];

    this.mPinger = setInterval(() => {
      this.socket.send('ping', undefined, false);
    }, 5000);
    this.pong();
  }

  public pong() {
    if (this.mTimeout) {
      clearTimeout(this.mTimeout);
      this.mTimeout = undefined;
    }
    this.mTimeout = setTimeout(() => {
      this.mOnDisconnect(1);
    }, 15000);
  }

  public destroy() {
    clearInterval(this.mPinger);
    if (this.mTimeout) {
      clearTimeout(this.mTimeout);
    }
  }

  public shootPosition(vec: Vec) {
    // TODO: calculate spread
    return vec;
  }

  public damage(amount: number) {
    this.mHealth -= amount;

    setTimeout(() => {
      this.mHealth = initialHealth[this.faction];

      this.mSession.broadcast(`respawn:${this.id}`, {
        health: this.health,
        position: [
          (Math.random() + 1) / 2 * 1000,
          (Math.random() + 1) / 2 * 800,
        ],
      }, true);
    }, this.respawnTime);

    return this.mHealth;
  }

  public set position(pos: Vec) {
    this.mPosition = pos;
  }

  public get position() {
    return this.mPosition;
  }

  public get health() {
    return this.mHealth;
  }

  public get respawnTime() {
    return this.faction === Faction.Simo ? 15000 : 5000;
  }
}
