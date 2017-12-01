export default class Action {
	package = 'Action'
	pos
	mousePos
	action
	constructor(pos, mousePos, actionType) {
		this.pos = pos
		this.mousePos = mousePos
		this.action = actionType
	}
	
	serialize() {
		return JSON.stringify(this)
	}
	
}
console.log('Loaded action');