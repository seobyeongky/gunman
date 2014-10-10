{STATE_READY,STATE_PLAY,STATE_RESULT,MAX_PLAYER} = require './consts'

FIRST_STATE = STATE_READY

scenes = {}
env =
	lv : 0
	tick : 0
	state : FIRST_STATE
	players : Player.all()
	stage_cleared : false

scenes[STATE_READY] = require './ready'
scenes[STATE_PLAY] = require './play'
scenes[STATE_RESULT] = require './result'

current_stage = FIRST_STATE
scene = null

next_scene = ->
	scene = scenes[current_stage] env
next_scene()

onPlayerInput (args...) ->
	scene.on_player_input args...
	if current_stage != env.state
		current_stage = env.state
		next_scene()

onFrameMove (args...) ->
	scene.on_frame_move args...
	if current_stage != env.state
		current_stage = env.state
		next_scene()
	env.tick++

Audio.playMusic 'audio/main_bgm.mp3'