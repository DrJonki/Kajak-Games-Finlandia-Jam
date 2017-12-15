import { each as eachAsync } from 'async';
import * as uuid from 'uuid/v4';
import Player, { Faction } from './player';
import Config from '@/services/config';
import Socket from '@/util/socket';
import Vec from '@/util/vec';
import Level from './level';
import { each, map, size, has } from 'lodash';

const maxPlayers = 10;

export default class Session {
  public readonly id = uuid();
  private mLevel = new Level();
  private mPlayers: { [i: string]: Player } = {};
  private mEvents: { [i: string]: any } = {};

  public join(data: any, socket: Socket) {
    const player = new Player(
      size(this.mPlayers) > 0 ? Faction.Russian : Faction.Simo,
      data,
      socket,
      this,
      (reason) => {
        this.leave(socket, reason);
      },
    );

    socket.send('connect', {
      id: player.id,
      faction: player.faction,
      radius: Player.radius,
      health: player.health,
      name: player.name,
      level: this.mLevel.level,
      view: [ Config.viewSize.x, Config.viewSize.y ],
      players: map(this.mPlayers, (value) => {
        return {
          id: value.id,
          health: value.health,
          name: value.name,
          radius: Player.radius,
          faction: value.faction,
        };
      }),
    }, true);

    this.broadcast('join', {
      id: player.id,
      health: player.health,
      name: player.name,
      faction: player.faction,
      radius: Player.radius,
    }, true);

    this.mPlayers[socket.id] = player;

    console.log(player.name, 'joined session', this.id);
  }

  public leave(socket: Socket, reason = 0) {
    const player = this.mPlayers[socket.id];

    if (player) {
      if (has(this.mEvents, 'leave')) {
        this.mEvents.leave(player);
      }

      this.broadcast('leave', {
        id: player.id,
        reason,
      }, true, player);

      console.log(player.name, 'left session', this.id, '-', reason);

      delete this.mPlayers[socket.id];
    }
  }

  public on(event: 'leave', func: any) {
    this.mEvents[event] = func;
  }

  public broadcast(event: string, data: any, tcp: boolean, except?: Player) {
    each(this.mPlayers, (player, key) => {
      if (!except || except.socket.id !== key) {
        player.socket.send(event, data, tcp);
      }
    });
  }

  public get full() {
    return size(this.mPlayers) >= maxPlayers;
  }

  public get empty() {
    return size(this.mPlayers) === 0;
  }

  public move(data: any, socket: Socket) {
    const player = this.mPlayers[socket.id];

    if (player) {
      player.position = new Vec(data.position);
      player.angle = data.angle;

      this.broadcast(`updateMovement:${player.id}`, {
        position: player.position.array,
        angle: player.angle,
      }, false, player);
    }
  }

  public shoot(data: any, socket: Socket) {
    const player = this.mPlayers[socket.id];

    if (player) {
      const pos = player.shootPosition(new Vec(data.crosshairPosition));

      this.broadcast(`shoot:${player.id}`, {
        position: pos.array,
        weaponType: data.weaponType,
      }, false, player);

      each(this.mPlayers, (value) => {
        const dist = value.position.distanceToVector(pos);
        const headshot = dist < (Player.radius / 3);

        if (dist < Player.radius) {
          this.broadcast(`damage:${value.id}`, {
            health: Math.floor(player.damage((headshot ? 100 : 50) / (data.weaponType === 0 ? 1 : 5))),
            headshot,
            respawnTime: player.respawnTime,
          }, true);
        }
      });
    }
  }
}
