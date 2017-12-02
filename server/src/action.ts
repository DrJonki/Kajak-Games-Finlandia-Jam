import g from './global'
export default class Action {
    package = 'Action'
    myId
    pos
    croshair
    action
    constructor(myId, pos, croshair, actionType) {
        this.pos = pos
        this.croshair = croshair
        this.action = actionType

        g.actionBuffer[myId] = this
    }
}
console.log('Loaded action');

/*
example of what client sends to server
{
    package: 'Action',
    data:{
        postion: {x:0, y:0},
        croshair {x:0, y:0}
        action: 'mouse1-down'
    }
}
*/