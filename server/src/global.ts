import * as dgram from 'dgram'
const g = {
	tickCount: -1,
	players: {},
	server: dgram.createSocket('udp4'),
	actionBuffer: [],
	maxSpeed: 100,
	simoIsInDaHouse: 0,

	sendAll(obj){
		for(let key in g.players) {
			g.players[key].send(obj)
		}
	},

	sendAllExcept(obj, exceptionID){
		for(let key in g.players) {
			if(exceptionID !== g.players[key].id) {
				g.players[key].send(obj)
			}
		}
	},
}
export default g
console.log('Loaded global');