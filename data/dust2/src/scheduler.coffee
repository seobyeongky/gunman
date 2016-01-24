{FRAME_RATE,STATE_READY} = require './consts'

class Scheduler
	constructor : ->
		@schedule = {}
		@current_tick = 0

	add : (fn,seconds) ->
		after = (seconds * FRAME_RATE) | 0
		# log [seconds, after]
		queue = @schedule[@current_tick + after] ?= []
		queue.push fn

	next_tick : (fn) ->
		queue = @schedule[@current_tick + 1] ?= []
		queue.push fn

	tick : ->
		while @schedule[@current_tick]?.length
			list = @schedule[@current_tick]
			delete @schedule[@current_tick]
			fn() for fn in list
		@current_tick++

module.exports =
	Scheduler : Scheduler