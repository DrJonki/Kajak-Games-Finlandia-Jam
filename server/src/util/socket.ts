import * as net from 'net';
import * as dgram from 'dgram';
import { each, filter } from 'lodash';

export interface ISocketContainer {
  [i: string]: Socket;
}

export default class Socket {
  public readonly id: string;
  public readonly address: string;
  private mUdpPort: number;
  private readonly mContainer: ISocketContainer;
  private readonly mTcpSocket: net.Socket;
  private mUdpSocket: dgram.Socket;

  public constructor(id: string, container: ISocketContainer, tcp: net.Socket) {
    this.id = id;
    this.mContainer = container;
    this.mTcpSocket = tcp;
    this.address = tcp.remoteAddress;
  }

  public setupUdp(port: number, udpSocket: dgram.Socket) {
    this.mUdpPort = port;
    this.mUdpSocket = udpSocket;
  }

  public send(event: string, data: any, tcp: boolean) {
    try {
      const obj = JSON.stringify({
        data: data || {},
        package: event,
      });

      if (tcp) {
        this.mTcpSocket.write(obj);
      } else {
        this.mUdpSocket.send(obj, this.mUdpPort, this.address);
      }
    } catch (err) {
      console.error(err);
    }
  }

  public broadcast(event: string, data: any, tcp: boolean) {
    try {
      each(filter(this.mContainer, (value) => value.id !== this.id), (value) => {
        value.send(event, data, tcp);
      });
    } catch (err) {
      console.error(err);
    }
  }

  public emit(event: string, data: any, tcp: boolean) {
    try {
      each(this.mContainer, (value) => {
        value.send(event, data, tcp);
      });
    } catch (err) {
      console.error(err);
    }
  }
}
