import Socket from '@/util/socket';
import Session from '@/session';
import Player from '@/session/player';
import { find } from 'lodash';

export default class SessionController {
  private mSessions: { [i: string]: Session } = {};
  private mSocketSessionMap: { [i: string]: Session } = {};

  public connect(data: any, socket: Socket) {
    let session = find(this.mSessions, (value) => !value.full);

    if (!session) {
      session = new Session();

      session.on('leave', () => {
        if (session.empty) {
          delete this.mSessions[session.id];
        }
      });

      this.mSessions[session.id] = session;
    }

    this.mapSocket(socket, session);
    session.join(data, socket);
  }

  public disconnect(data: any, socket: Socket) {
    const session = this.mSocketSessionMap[socket.id];

    if (session) {
      session.leave(socket);
      this.unmapSocket(socket);
    }
  }

  public pong(data: any, socket: Socket) {
    this.passMessage('pong', data, socket);
  }

  public move(data: any, socket: Socket) {
    this.passMessage('move', data, socket);
  }

  public shoot(data: any, socket: Socket) {
    this.passMessage('shoot', data, socket);
  }

  private passMessage(event: string, data: any, socket: Socket) {
    const session = this.mSocketSessionMap[socket.id];

    if (session) {
      session[event].bind(session)(data, socket);
      return true;
    }

    return false;
  }

  private mapSocket(socket: Socket, session: Session) {
    this.mSocketSessionMap[socket.id] = session;
  }

  private unmapSocket(socket: Socket) {
    delete this.mSocketSessionMap[socket.id];
  }
}
