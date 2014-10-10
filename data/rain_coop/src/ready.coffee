{STATE_PLAY,BG_KIND,MAX_NR_PLAYER} = require './consts'
{Scheduler} = require './scheduler'
{shuffle} = require './utils'

module.exports = (env) ->
	{lv} = env

	players = Player.all()
	if players.length > MAX_NR_PLAYER
		players = shuffle players
		players = players.splice(0, MAX_NR_PLAYER)
	env.players = players

	count = 0
	scheduler = new Scheduler

	text = new Text
	text.string = if players.length == 1 then "고독한 산성비 막기!" else "#{player.length}인 협동 산성비 막기!!"
	text.x = 0.3 * UI.width
	text.y = 0.3 * UI.height
	text.characterSize = 25

	scheduler.add ->
		text.string = "... Day #{Math.floor(lv / BG_KIND)}"
	, 3

	scheduler.add ->
		text.string = players.join(', ')
		text.string += "\n준비하세요!"
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