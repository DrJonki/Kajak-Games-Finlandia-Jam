import Router from '@/services/router';
import SessionController from '@/controllers/session';

export default (router: Router) => {
  router.registerController('SessionController', new SessionController());

  router.register('ping', (data, socket) => {
    socket.send('pong', undefined, false);
  });

  router.register('connect', 'SessionController@connect');
  router.register('disconnect', 'SessionController@disconnect');
  router.register('pong', 'SessionController@pong');
};
