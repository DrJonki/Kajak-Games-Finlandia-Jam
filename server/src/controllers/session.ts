import Socket from '@/util/socket';
import Session from '@/session';
import { find } from 'lodash';

export default class SessionController {
  private sessions: { [i: string]: Session } = {};
  private socketSessionMap: { [i: string]: Session } = {};

  public connect(data: any, socket: Socket) {
    let session = find(this.sessions, (value) => !value.full);

    if (!session) {
      session = new Session();
      this.sessions[session.id] = session;
    }

    this.mapSocket(socket, session);
    session.join(data, socket);
  }

  public disconnect(data: any, socket: Socket) {
    const session = this.socketSessionMap[socket.id];

    if (session) {
      session.leave(socket);
      this.unmapSocket(socket);
    }
  }

  public pong(data: any, socket: Socket) {
    this.passMessage('pong', data, socket);
  }

  private passMessage(event: string, data: any, socket: Socket) {
    const session = this.socketSessionMap[socket.id];

    if (session) {
      session[event].bind(session)(data, socket);
      return true;
    }

    return false;
  }

  private mapSocket(socket: Socket, session: Session) {
    this.socketSessionMap[socket.id] = session;
  }

  private unmapSocket(socket: Socket) {
    delete this.socketSessionMap[socket.id];
  }
}
