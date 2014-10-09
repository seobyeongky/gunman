{STATE_PLAY} = require './consts'

module.exports = (env) ->
	{players} = env

	count = 0
	when_ = {}

	text = new Text
	text.string = "산성비를 막아라!!"
	text.x = 0.3 * UI.width
	text.y = 0.3 * UI.height
	text.characterSize = 25

	# 5초에
	when_[5 * 60] = ->
		text.string = "준비하세요!"

	[3..1].forEach (i) ->
		# 6, 7, 8초에
		when_[6 * 60 + (3 - i) * 60] = ->
			text.string += "...#{i}"

	# 9초에 시작
	when_[9 * 60] = ->
		env.state = STATE_PLAY

	on_frame_move : ->
		the_event = when_[count++]
		the_event?()

		UI.draw text


	on_player_input : ->