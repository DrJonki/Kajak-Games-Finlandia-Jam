import g from './global'

export default class Level {
    package = 'Level'
    level = {
		props: [
			{
				id: '0',
				radius: 50,
				position: [0, 0],
				type: 0, // Rock, fence, house, etc...
				angle: 0
			},
		]
	}

    constructor() {
    }
}
console.log('Loaded Level');