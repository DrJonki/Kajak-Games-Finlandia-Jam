import * as net from 'net';
import * as dgram from 'dgram';
import { each, filter } from 'lodash';

export interface ISocketContainer {
  [i: string]: Socket;
}

export default class Socket {
  public readonly id: string;
  public readonly address: string;
  private udpPort: number;
  private readonly container: ISocketContainer;
  private readonly tcpSocket?: net.Socket;
  private udpSocket: dgram.Socket;

  public constructor(id: string, container: ISocketContainer, tcp: net.Socket) {
    this.id = id;
    this.container = container;
    this.tcpSocket = tcp;
    this.address = tcp.remoteAddress;
  }

  public setupUdp(port: number, udpSocket: dgram.Socket) {
    this.udpPort = port;
    this.udpSocket = udpSocket;
  }

  public send(event: string, data: any, tcp: boolean) {
    try {
      const obj = JSON.stringify({
        data: data || {},
        package: event,
      });

      if (tcp) {
        this.tcpSocket.write(obj);
      } else {
        this.udpSocket.send(obj, this.udpPort, this.address);
      }
    } catch (err) {
      console.error(err);
    }
  }

  public broadcast(event: string, data: any, tcp: boolean) {
    try {
      each(filter(this.container, (value) => value.id !== this.id), (value) => {
        value.send(event, data, tcp);
      });
    } catch (err) {
      console.error(err);
    }
  }

  public emit(event: string, data: any, tcp: boolean) {
    try {
      each(this.container, (value) => {
        value.send(event, data, tcp);
      });
    } catch (err) {
      console.error(err);
    }
  }
}
