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
			if(exceptionID !== key) {
				g.players[key].send(obj)
				console.log('sent:' )
				console.log(obj)
				console.log('to all except' + exceptionID)
			}
			console.log('sent to: '+ key)
		}
	},
}
export default g
console.log('Loaded global');