_ = require 'underscore'
{STATE_READY} = require './consts'
{Scheduler} = require './scheduler'
stages = require './stages'

textures = {}
['gameover','gameclear','stageclear'].forEach (name) ->
	t = new Texture
	t.loadFromFile "textures/bg_#{name}.png"
	textures["bg_#{name}"] = t

module.exports = (env) ->
	{players,stats,stage_cleared} = env

	scheduler = new Scheduler
	result_text = null
	bg = null

	all_cleared = ->
		env.lv == stages.length - 1

	make_result_text = ->
		text = new Text
		text.characterSize = 25
		text.string = "결과!"
		text.x = 0.3 * UI.width
		text.y = 0.3 * UI.height

		wrapped = stats.map (a,i) -> _.extend {pid:i}, a
		sorted = _.sortBy wrapped, (a) -> a.nr_destroy
		sorted.reverse()
		sorted.forEach (a,i) ->
			p = players[a.pid]
			scheduler.add ->
				text.string += "\n"
				text.string += "#{p.name} : #{a.nr_destroy}개 파괴!"
			, i + 2
		text
	result_text = make_result_text()

	make_bg = ->
		t = null
		if stage_cleared
			if all_cleared()
				t = textures.bg_gameclear
			else
				t = textures.bg_stageclear
		else
			t = textures.bg_gameover
		s = new Sprite
		s.texture = t
		s.x = 0
		s.y = 0
		s.originX = 0
		s.originY = 0
		s.scaleX = UI.width / t.width
		s.scaleY = UI.height / t.height
		s
	bg = make_bg()

	scheduler.add ->
		if all_cleared()
			result_text.string += "\n축하합니다. 모든 스테이지를 클리어하였습니다!"
			return
		if stage_cleared
			env.lv++
		else
			env.lv = 0
		env.state = STATE_READY
	, players.length + 6

	on_frame_move : ->
		scheduler.tick()
		UI.draw bg
		UI.draw result_text

	on_player_input : ->