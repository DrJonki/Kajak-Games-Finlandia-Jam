import * as dgram from 'dgram'
const g = {
	tickCount: -1,
	players: {},
	server: dgram.createSocket('udp4'),
	actionBuffer: [],
	maxSpeed: 100,
	simoIsInDaHouse: 0,

	sendAll(obj){
		const json = JSON.stringify(obj)
		for(let key in g.players) {
			g.players[key].send(json)
		}
	}
}
export default g
console.log('Loaded global');