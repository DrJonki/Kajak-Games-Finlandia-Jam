import { each as eachAsync } from 'async';
import * as uuid from 'uuid/v4';
import Player, { Faction } from './player';
import Config from '@/services/config';
import Socket from '@/util/socket';
import Vec from '@/util/vec';
import Level from './level';
import { each, map, size } from 'lodash';

const maxPlayers = 10;

export default class Session {
  public readonly id = uuid();
  private mLevel = new Level();
  private mPlayers: { [i: string]: Player } = {};

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
      level: this.mLevel.level,
      view: [ Config.viewSize.x, Config.viewSize.y ],
      players: map(this.mPlayers, (value) => {
        return {
          id: value.id,
          health: value.id,
          faction: value.faction,
        };
      }),
    }, true);

    this.broadcast('join', {
      id: player.id,
      health: player.health,
      faction: player.faction,
    }, true);

    this.mPlayers[socket.id] = player;

    console.log(player.id, 'joined session', this.id);
  }

  public leave(socket: Socket, reason = 0) {
    const player = this.mPlayers[socket.id];

    if (player) {
      this.broadcast('leave', {
        id: player.id,
        reason,
      }, true, player);

      player.destroy();
      delete this.mPlayers[socket.id];

      console.log(player.id, 'left session', this.id, '-', reason);
    }
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

  // Events
  public pong(data: any, socket: Socket) {
    const player = this.mPlayers[socket.id];

    if (player) {
      player.pong();
    }
  }

  public move(data: any, socket: Socket) {
    const player = this.mPlayers[socket.id];

    if (player) {
      player.position = new Vec(data.position);

      this.broadcast(`updateMovement:${player.id}`, {
        position: player.position,
      }, false, player);
    }
  }

  public shoot(data: any, socket: Socket) {
    const player = this.mPlayers[socket.id];

    if (player) {
      const pos = player.shootPosition(new Vec(data.crosshairPosition));

      this.broadcast(`shoot:${player.id}`, {
        position: pos,
        weaponType: data.weaponType,
      }, false, player);

      eachAsync(this.mPlayers, (value) => {
        const dist = value.position.distanceToVector(pos);
        const headshot = dist < (Player.radius / 3);

        if (dist < Player.radius) {
          this.broadcast(`damage:${value.id}`, {
            health: player.damage(headshot ? 100 : 50) * data.weaponType === 0 ? 1.0 : 0.2,
            headshot,
            respawnTime: player.respawnTime,
          }, true);
        }
      });
    }
  }
}
