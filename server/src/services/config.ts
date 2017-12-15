import Vec from '@/util/vec';

const viewSize = new Vec(1920, 1080);

export default Object.freeze({
  initialHealth: [ // Indexed by Faction
    /* Simo */    1000,
    /* Russian */ 100,
  ],
  mapObjects: {
    max: 50,
    min: 30,
  },
  mapRadius: viewSize.x * 2,
  objectRadiusMult: 1.0,
  viewSize,
});
