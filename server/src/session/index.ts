import Player, { Faction } from './player';
import * as uuid from 'uuid/v4';
import Socket from '@/util/socket';
import Level from './level';
import { each, map, size } from 'lodash';

const maxPlayers = 10;

export default class Session {
  public readonly id = uuid();
  private level = new Level();
  private players: { [i: string]: Player } = {};

  public join(data: any, socket: Socket) {
    const player = new Player(
      size(this.players) > 0 ? Faction.Russian : Faction.Simo,
      data,
      socket,
      (reason) => {
        this.leave(socket, reason);
      },
    );

    socket.send('connect', {
      id: player.id,
      faction: player.faction,
      health: player.health,
      level: this.level,
      players: map(this.players, (value) => {
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

    this.players[socket.id] = player;

    console.log(player.id, 'joined session', this.id);
  }

  public leave(socket: Socket, reason = 0) {
    const player = this.players[socket.id];

    if (player) {
      this.broadcast('leave', {
        id: player.id,
        reason,
      }, true, player);

      player.destroy();
      delete this.players[socket.id];

      console.log(player.id, 'left session', this.id, '-', reason);
    }
  }

  public broadcast(event: string, data: any, tcp: boolean, except?: Player) {
    each(this.players, (player, key) => {
      if (!except || except.socket.id !== key) {
        player.socket.send(event, data, tcp);
      }
    });
  }

  public get full() {
    return size(this.players) >= maxPlayers;
  }

  // Events
  public pong(data: any, socket: Socket) {
    const player = this.players[socket.id];

    if (player) {
      player.pong();
    }
  }
}
