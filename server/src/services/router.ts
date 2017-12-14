import { includes } from 'lodash';
import { has, isString, isFunction } from 'lodash';
import Socket from '@/util/socket';

type TMessageHandler = (data: any | undefined, socket: Socket) => any;

export default class Router {
  private mRoutes: { [i: string]: TMessageHandler[] } = new Proxy({}, {
    get(target, property) {
      return target[property] || (target[property] = [] as TMessageHandler[]);
    },
  });
  private mControllers: { [i: string]: object } = {};

  public register(event: string, handler: string | TMessageHandler) {
    if (isString(handler)) {
      const split = handler.split('@');
      const controller = this.mControllers[split[0]];

      if (!controller) {
        throw new Error(`Controller not found: ${split[0]}`);
      }

      const method: TMessageHandler = controller[split[1]];

      if (!method || !isFunction(method)) {
        throw new Error(`Controller method '${split[1]} not found from controller: ${split[0]}`);
      }

      this.register(event, method.bind(controller));
    } else {
      this.mRoutes[event].push(handler);
    }
  }

  public registerController(name: string, controller: object) {
    this.mControllers[name] = controller;
  }

  public invoke(event: string, data: any, socket: Socket) {
    try {
      for (const handler of this.mRoutes[event]) {
        handler(data, socket);
      }
    } catch (err) {
      console.error(err);
    }
  }
}
