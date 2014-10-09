{INPUT_CHAT_MESSAGE} = require './system/consts'
{STATE_READY,BG_KIND,SEA_SEVERTY_LEVELS,MAX_HP,FRAME_RATE} = require './consts'
stages = require './stages'
dict = require './dict.json'
_ = require 'underscore'

textures = {}

[0...BG_KIND].forEach (i) ->
	t = new Texture
	t.loadFromFile "textures/bg_#{i}.png"
	textures["bg_#{i}"] = t

[0...SEA_SEVERTY_LEVELS].forEach (i) ->
	t = new Texture
	t.loadFromFile "textures/sea_#{i}.png"
	textures["sea_#{i}"] = t


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
	stage = stages[0]
	effects = []
	when_ = []
	nr_fallen = 0
	lv = 0
	playing = true
	ranking_text = null
	env.stats = players.map ->
		nr_destroy : 0

	make_background = ->
		texture_index = 0
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
		s.change = ->
			texture_index = 0 if ++texture_index >= BG_KIND
			update_texture()

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
			severty = Math.floor(ratio * (SEA_SEVERTY_LEVELS - 1))
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
		self.color = {r:255,g:255,b:0,a:255}
		self.originY = 0
		self
	hp_text = make_hp_text()

	make_timeleft_text = ->
		self = new Text
		self.characterSize = 35
		{STAGE_TIME} = stage
		begin_at = count
		self.update = ->
			self.string = "남은 시간 : #{Math.max(0,Math.floor((begin_at + STAGE_TIME - count) / FRAME_RATE))}초"
			self.originX = self.width
		self.update()
		self.x = UI.width - 20
		self.y = 20
		self.color = {r:0,g:255,b:0,a:255}
		self
	timeleft_text = make_timeleft_text()

	make_ranking_text = ->
		t = new Text
		t.characterSize = 22
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
		self.string = dict[Math.floor(Math.random() * dict.length)]
		self.originX = 0.5 * self.width
		self.originY = 0.5 * self.height
		self.x = Math.random() * (0.5 * UI.width - self.width) + 0.5 * self.width
		self.y = -self.height
		self.color = {r:0,g:0,b:0,a:255}
		born_time = count
		self.update = ->
			{WORD_LIVING_TICKS} = stage
			self.y = -self.height + ((count - born_time) / WORD_LIVING_TICKS) * UI.height
			if count - born_time > WORD_LIVING_TICKS
				self.should_dead = true
			return
		self.run_disappear_action = ->
			effects.push self
			[1...10].forEach (i) ->
				time = when_[count + i] ?= []
				time.push ->
					self.scaleX *= 0.9
					self.scaleY *= 0.9
			lasttime = when_[count+11] ?= []
			lasttime.push ->
				effects.splice effects.indexOf(self), 1

		self

	update_fallen_texts = ->
		{WORD_GENERATION_PERIOD} = stage
		if count % WORD_GENERATION_PERIOD == 0
			fallen_texts.push make_fallen_text()
		text.update() for text in fallen_texts
		fallen_texts.forEach (t) -> nr_fallen++ if t.should_dead
		fallen_texts = _.reject fallen_texts, (t) -> t.should_dead

	checks = ->
		if playing
			if nr_fallen >= MAX_HP
				t = make_message_text("게임 오버!",{r:255,g:0,b:0,a:255})
				effects.push t
				time = when_[count + 3 * 60] ?= []
				time.push ->
					idx = effects.indexOf t
					throw new Error("hul") if idx == -1
					effects.splice idx, 1
					env.state = STATE_READY
				playing = false

	update = ->
		while when_[count]?
			list = when_[count]
			delete when_[count]
			e() for e in list

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


	next_stage = ->
		stage = stages[++lv]
		throw new Error("all stage cleared") unless stage?

	stage_start = ->
		{STAGE_TIME} = stage
		a = when_[count + STAGE_TIME] ?= []
		a.push ->
			t = make_message_text("스테이지 성공!",{r:100,g:225,b:50,a:255})
			effects.push t
			time = when_[count + 3 * 60] ?= []
			time.push ->
				idx = effects.indexOf t
				throw new Error("hul2") if idx == -1
				effects.splice idx, 1
				env.state = STATE_RESULT
			playing = false

	on_frame_move : ->
		update()
		render()

		count++

		return

	on_player_input : (pid,type,args...) ->
		if type == INPUT_CHAT_MESSAGE
			chat pid, args[0]