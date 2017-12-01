export default class Connection {
    package = 'Connection'
    connection
    name

    constructor(name, onoff) {
        this.connection = onoff
        this.name = name;
    }
}
console.log('Loaded connection');