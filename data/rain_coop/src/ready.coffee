{STATE_PLAY} = require './consts'
{Scheduler} = require './scheduler'

module.exports = (env) ->
	{players} = env

	count = 0
	scheduler = new Scheduler

	text = new Text
	text.string = "산성비를 막아라!!"
	text.x = 0.3 * UI.width
	text.y = 0.3 * UI.height
	text.characterSize = 25

	scheduler.add ->
		text.string = "준비하세요!"
	, 5

	[3..1].forEach (i) ->
		scheduler.add ->
			text.string += "...#{i}"
		, 6 + (3 - i)

	scheduler.add ->
		env.state = STATE_PLAY
	, 9

	on_frame_move : ->
		scheduler.tick()
		UI.draw text

	on_player_input : ->