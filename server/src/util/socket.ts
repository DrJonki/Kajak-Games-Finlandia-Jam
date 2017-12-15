import * as net from 'net';
import * as dgram from 'dgram';
import * as uuid from 'uuid/v4';
import { each, filter } from 'lodash';

export interface ISocketContainer {
  [i: string]: Socket;
}

export default class Socket {
  public readonly id: string;
  public readonly udpId = uuid();
  public readonly address: string;
  private mPort: number;
  private readonly mContainer: ISocketContainer;
  private readonly mTcpSocket: net.Socket;
  private readonly mUdpSocket: dgram.Socket;

  public constructor(id: string, container: ISocketContainer, tcp: net.Socket, udp: dgram.Socket) {
    this.id = id;
    this.mContainer = container;
    this.mTcpSocket = tcp;
    this.address = tcp.remoteAddress;
    this.mUdpSocket = udp;
  }

  public setUdpPort(port: number) {
    this.mPort = port;
  }

  public send(event: string, data: any, tcp: boolean) {
    try {
      const obj = JSON.stringify({
        data: data || {},
        package: event,
      });

      if (tcp) {
        this.mTcpSocket.write(`${obj};end;`);
      } else {
        console.log('UDP message TO -', this.id, ':', obj);
        this.mUdpSocket.send(obj, this.mPort, this.address, (err) => {
          console.log('UDP ERROR', err);
        });
      }
    } catch (err) {
      console.log(err);
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
