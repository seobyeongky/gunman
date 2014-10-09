module.exports = (env) ->
	{stats} = env

	text = new Text
	text.string = "결과!\n"

	count = 0

	on_frame_move : ->
		if 
		count++
	on_player_input : ->