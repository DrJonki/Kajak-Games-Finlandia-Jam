import Vec from '@/util/vec';
import Socket from '@/util/socket';
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
  public readonly socket: Socket;
  public readonly id = uuid();
  public readonly faction: Faction;
  public readonly name: string;
  private mHealth: number;
  private mOnDisconnect: TOnDisconnectFunc;
  private mPinger: NodeJS.Timer;
  private mTimeout: NodeJS.Timer;

  constructor(faction: Faction, data: any, socket: Socket, onDisconnect: TOnDisconnectFunc) {
    this.faction = faction;
    this.name = data.name;
    this.socket = socket;
    this.mOnDisconnect = onDisconnect;

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

  public get health() {
    return this.mHealth;
  }
}
