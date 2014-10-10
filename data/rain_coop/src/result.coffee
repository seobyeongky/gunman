_ = require 'underscore'
{Scheduler} = require './scheduler'

module.exports = (env) ->
	{players,stats} = env

	scheduler = new Scheduler

	text = new Text
	text.string = "결과!"

	wrapped = stats.map (a,i) -> _.extend {pid:i}, a
	sorted = _.sortBy wrapped, (a) -> a.nr_destroy
	sorted.reverse()
	sorted.forEach (a,i) ->
		p = players[a.pid]
		scheduler.add ->
			text.string += "\n"
			text.string += "#{p.name} : #{a.nr_destroy}개 파괴!"
		, i + 2

	scheduler.add ->
		env.state = STATE_READY
	, sorted.length + 6

	on_frame_move : ->
		scheduler.tick() 

	on_player_input : ->