{INPUT_CHAT_MESSAGE} = require './system/consts'
{STATE_READY,STATE_RESULT,BG_KIND,SEA_SEVERTY_LEVELS,MAX_HP,FRAME_RATE} = require './consts'
{Scheduler} = require './scheduler'
stages = require './stages'
dict = require './dict.json'
_ = require 'underscore'

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

[0...BG_KIND].forEach (i) ->
	t = new Texture
	t.loadFromFile "textures/bg_#{i}.png"
	textures["bg_#{i}"] = t

[0...SEA_SEVERTY_LEVELS].forEach (i) ->
	t = new Texture
	t.loadFromFile "textures/sea_#{i}.png"
	textures["sea_#{i}"] = t

ACID = 'audio/acid.wav'
POP = 'audio/word.wav'
SUCCEEDED = 'audio/FFT.wav'
WORSE = 'audio/oil.wav'

the_seed = 123
RANDOM = (require './random') the_seed

play_hit = ->
	Audio.playEffect "audio/hit_#{Math.floor(Math.random() * 8)}.wav"

play_miss = ->
	Audio.playEffect "audio/miss_#{Math.floor(Math.random() * 2)}.wav"

make_message_text = (msg,color) ->
	t = new Text
	t.characterSize = 50
	t.string = msg
	t.x = 0.5 * UI.width
	t.y = 0.5 * UI.height
	t.originX = 0.5 * t.width
	t.originY = 0.5 * t.height
	t.color = color
	t

module.exports = (env) ->
	{players} = env

	count = 0
	fallen_texts = []
	bg = null
	sea = null
	hp_text = null
	timeleft_text = null
	stage = stages[env.lv]
	effects = []
	scheduler = new Scheduler
	nr_fallen = 0
	playing = true
	ranking_text = null
	env.stats = players.map ->
		nr_destroy : 0

	make_background = ->
		{lv} = env
		texture_index = lv % BG_KIND
		s = new Sprite
		s.x = 0
		s.y = 0
		s.originX = 0
		s.originY = 0
		update_texture = ->
			t = textures["bg_#{texture_index}"]
			s.scaleX = UI.width / t.width
			s.scaleY = UI.height / t.height
			s.texture = t
		# s.change = ->
		# 	texture_index = 0 if ++texture_index >= BG_KIND
		# 	update_texture()

		update_texture()
		s
	bg = make_background()

	make_sea = ->
		severty = 0
		s = new Sprite
		s.x = 0
		s.y = UI.height
		s.originX = 0
		update_texture = ->
			t = textures["sea_#{severty}"]
			s.originY = t.height
			s.scaleX = UI.width / t.width
			s.scaleY = s.scaleX
			s.texture = t
		s.update = ->
			ratio = Math.min(nr_fallen / MAX_HP,1)
			old_severty = severty
			severty = Math.floor(ratio * (SEA_SEVERTY_LEVELS - 1))
			Audio.playEffect WORSE if old_severty != severty
			update_texture()

		update_texture()
		s
	sea = make_sea()

	make_hp_text = ->
		self = new Text
		self.characterSize = 45
		self.update = ->
			self.string = "#{Math.max(MAX_HP-nr_fallen,0)}/#{MAX_HP}"
			self.originX = self.width
		self.update()
		self.x = UI.width - 20
		self.y = UI.height - 100
		self.color = sub_key_color(env.lv)
		self.originY = 0
		self
	hp_text = make_hp_text()

	make_timeleft_text = ->
		self = new Text
		self.characterSize = 35
		{STAGE_TIME} = stage
		begin_at = count
		self.update = ->
			self.string = "남은 시간 : #{Math.max(0,Math.floor((begin_at - count) / FRAME_RATE) + STAGE_TIME)}초"
			self.originX = self.width
		self.update()
		self.x = UI.width - 20
		self.y = 20
		self.color = sub_key_color(env.lv)
		self
	timeleft_text = make_timeleft_text()

	make_ranking_text = ->
		t = new Text
		t.characterSize = 22
		t.color = sub_key_color(env.lv)
		t.update = ->
			{stats} = env
			wrapped = stats.map (a,i) -> _.extend {pid:i}, a
			sorted = _.sortBy wrapped, (a) -> a.nr_destroy
			sorted.reverse()
			t.string = ""
			for a in sorted
				t.string += '\n'
				t.string += "#{players[a.pid].name} : #{a.nr_destroy}개 파괴"
			max_len = _.reduce t.string.split('\n'), ((memo,text) -> Math.max(memo,text.length)), 0
			t.string = ([0...max_len].map -> '=').join('') + t.string

			t.originX = t.width
			return
		t.update()
		t.x = UI.width - 20
		t.y = 0.2 * UI.height
		t
	ranking_text = make_ranking_text()

	make_fallen_text = ->
		self = new Text
		self.characterSize = 25
		self.string = dict[RANDOM.number(dict.length)]
		self.originX = 0.5 * self.width
		self.originY = 0.5 * self.height
		self.x = RANDOM.number(0.5 * UI.width - 0.5 * self.width) + 0.5 * self.width
		self.y = -self.height
		self.color = main_key_color(env.lv)
		born_time = count
		self.update = ->
			{WORD_LIVING_TICKS} = stage
			self.y = -self.height + ((count - born_time) / WORD_LIVING_TICKS) * UI.height
			if count - born_time > WORD_LIVING_TICKS
				self.should_dead = true
			return
		self.run_disappear_action = ->
			effects.push self
			index = 0
			the_func = ->
				if index++ > 10
					effects.splice effects.indexOf(self), 1
				else
					self.scaleX *= 0.9
					self.scaleY *= 0.9
					scheduler.next_tick the_func
			scheduler.next_tick the_func

		self

	update_fallen_texts = ->
		{WORD_GENERATION_PERIOD} = stage
		if count % WORD_GENERATION_PERIOD == 0
			fallen_texts.push make_fallen_text()
		text.update() for text in fallen_texts
		fallen_texts.forEach (t) ->
			if t.should_dead
				Audio.playEffect ACID
				nr_fallen++
		fallen_texts = _.reject fallen_texts, (t) -> t.should_dead

	checks = ->
		if playing
			if nr_fallen >= MAX_HP
				t = make_message_text("게임 오버!",{r:255,g:0,b:0,a:255})
				effects.push t
				scheduler.add ->
					effects.splice effects.indexOf(t), 1
					env.stage_cleared = false
					env.state = STATE_RESULT
				, 3
				playing = false

	update = ->
		scheduler.tick()

		if playing
			update_fallen_texts()
			hp_text.update()
			timeleft_text.update()
			ranking_text.update()
			sea.update()

		checks()

	render = ->
		UI.draw bg
		UI.draw sea
		UI.draw text for text in fallen_texts
		UI.draw effect for effect in effects
		UI.draw hp_text
		UI.draw timeleft_text
		UI.draw ranking_text

	chat = (pid,msg) ->
		matched = _.find fallen_texts, (t) -> t.string == msg
		if matched?
			env.stats[pid].nr_destroy++
			matched.color = players[pid].color
			matched.run_disappear_action()
			fallen_texts.splice fallen_texts.indexOf(matched), 1
			play_hit()
		else
			play_miss()

	stage_start = ->
		{STAGE_TIME} = stage
		scheduler.add ->
			playing = false
			Audio.playEffect SUCCEEDED
			t = make_message_text("스테이지 성공!",{r:100,g:225,b:50,a:255})
			effects.push t
			scheduler.add ->
				effects.splice effects.indexOf(t), 1
				env.stage_cleared = true
				env.state = STATE_RESULT
			, 3
		, STAGE_TIME

	stage_start()


	on_frame_move : ->
		update()
		render()

		count++

		return

	on_player_input : (pid,type,args...) ->
		if type == INPUT_CHAT_MESSAGE
			chat pid, args[0]