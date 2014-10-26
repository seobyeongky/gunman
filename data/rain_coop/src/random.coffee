RNG = require './libs/RNG'

module.exports = (seed) ->
	rng = new RNG(seed)

	number : (N) -> rng.random 0, N
	shuffle : (obj) ->
		index = 0
		shuffled = []
		obj.forEach (value) -> 
			rand = rng.random(0,++index)
			shuffled[index - 1] = shuffled[rand]
			shuffled[rand] = value
		shuffled