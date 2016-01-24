{STATE_PLAY,BG_KIND,MAX_NR_PLAYER} = require './consts'
{Scheduler} = require './scheduler'
{shuffle} = require './utils'

make_bg = ->
	t = new Texture
	t.loadFromFile 'textures/bg_intro.png'
	s = new Sprite
	s.texture = t
	s.x = 0
	s.y = 0
	s.originX = 0
	s.originY = 0
	s.scaleX = UI.width / t.width
	s.scaleY = UI.height / t.height
	s

module.exports = (env) ->
	{lv} = env

	players = Player.all()
	if players.length > MAX_NR_PLAYER
		players = shuffle players
		players = players.splice(0, MAX_NR_PLAYER)
	env.players = players

	# count = 0
	# scheduler = new Scheduler

	# text = new Text
	# text.color = {r:0,g:0,g:0,a:255}
	# text.string = if players.length == 1 then "산성비를 막아라!" else "#{players.length}인 협동 산성비 막기!!"
	# text.x = 0.3 * UI.width
	# text.y = 0.3 * UI.height
	# text.characterSize = 25
	# bg = make_bg()

	# scheduler.add ->
	# 	Audio.playEffect FOGHORN
	# 	text.string = "... Day #{Math.floor(lv / BG_KIND)}"
	# , 3

	# scheduler.add ->
	# 	text.string = players.map((p)->p.name).join(', ')
	# 	text.string += "\n준비하십시오!"
	# , 5

	# [3..1].forEach (i) ->
	# 	scheduler.add ->
	# 		text.string += "...#{i}"
	# 	, 6 + (3 - i)

	# scheduler.add ->
	# 	env.state = STATE_PLAY
	# , 9

	on_frame_move : ->
		# scheduler.tick()
		# UI.draw bg
		# UI.draw text

	on_player_input : (args...) ->
		print "#{JSON.stringify(args)}"