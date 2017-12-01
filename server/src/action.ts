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
}
console.log('Loaded action');