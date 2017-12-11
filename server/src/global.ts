import * as dgram from 'dgram'
import PackageManager from './package'
const g = {
	players: {},
	server: dgram.createSocket('udp4'),
	actionBuffer: [],
	packageManager: null,
	maxSpeed: 100,
	simoIsInDaHouse: 0,

	sendAll(obj){
		for(let key in g.players) {
			g.players[key].send(obj)
		}
	},

	sendAllInSession(obj, sessionID){
		for(let key in g.packageManager.sessions[sessionID].players) {
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