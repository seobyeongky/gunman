{STATE_READY,STATE_PLAY,STATE_RESULT} = require './consts'

FIRST_STATE = STATE_PLAY

scenes = {}
env =
	tick : 0
	state : FIRST_STATE
	players : Player.all()

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