_ = require 'underscore'

{INPUT_CHAT_MESSAGE} = require './system/consts'
{STATE_RESULT,BG_KIND,SEA_SEVERTY_LEVELS,MAX_HP,FRAME_RATE} = require './consts'
{Scheduler} = require './scheduler'
{make_bg} = require './bg'
{make_minimap} = require './minimap'
{make_statbox} = require './statbox'
{make_selbox} = require './selbox'


textures = {}

main_key_color = (index) ->
	switch index % 3
		when 0
			{r:0,g:0,b:0,a:255}
		when 1
			{r:50,g:50,b:50,a:255}
		when 2
			{r:255,g:255,b:240,a:255}

sub_key_color = (index) ->
	switch index % 3
		when 0
			{r:255,g:255,b:255,a:255}
		when 1
			{r:180,g:180,b:170,a:255}
		when 2
			{r:190,g:190,b:180,a:255}

the_seed = 123
RANDOM = (require './random') the_seed

module.exports = (env) ->
	{players} = env

	scheduler = new Scheduler
	bg = make_bg()
	minimap = make_minimap()
	statboxes = [0...4].map make_statbox
	selbox = make_selbox()

	j = 1
	for player, i in players
		break if i >= 4

		player.hp = MAX_HP
		player.turn_used = false
		if player.id == Player.me().id
			player.statbox = statboxes[0]
			player.visible = true
		else
			player.statbox = statboxes[j++]
			player.visible = false
		
		player.statbox.set_player player

	update_turn = ->
		turn_player = env.turn_player = _.find players, (player) -> player.turn_used == false
		turn_player.turn_used = true
		print "#{turn_player.name}님의 턴입니다."

	scheduler.add update_turn, 0.5

	handle_chat = ->

	update = ->
		scheduler.tick()

	render = ->
		UI.draw bg
		UI.draw minimap
		for statbox in statboxes
			statbox.render()
		selbox.render()

	on_frame_move : ->
		update()
		render()
		return

	on_player_input : (pid,type,args...) ->
		if type == INPUT_CHAT_MESSAGE
			handle_chat pid, args[0]