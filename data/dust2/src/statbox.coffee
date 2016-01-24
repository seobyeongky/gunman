{PADDING,SMALL_PADDING,TXT_HEIGHT} = require './consts'
{make_sprite} = require './utils/sprite'

module.exports =
	make_statbox : (i) ->
		player = null

		W = 0.4 * UI.width
		H = 0.2 * UI.height
		X = UI.width - W - PADDING
		Y = PADDING + i * (SMALL_PADDING + 0.2 * UI.height)

		bg = make_sprite "frame0.png", W, H
		bg.x = X
		bg.y = Y

		AX = X + SMALL_PADDING
		avatar = make_sprite "avatar#{i}.png", null, H - 2 * SMALL_PADDING
		avatar.x = AX
		avatar.y = Y + SMALL_PADDING
		AX += avatar.scaleX * avatar.texture.width

		name_txt = new Text
		name_txt.x = AX + PADDING
		name_txt.y = Y + SMALL_PADDING
		name_txt.characterSize = TXT_HEIGHT

		hp_txt = new Text
		hp_txt.x = AX + PADDING
		hp_txt.y = Y + SMALL_PADDING + TXT_HEIGHT + SMALL_PADDING
		hp_txt.characterSize = TXT_HEIGHT
		hp_txt.color = rgba(255, 255, 255, 255)

		bloody = make_sprite "bloody.png", W, H
		bloody.x = X
		bloody.y = Y

		set_player : (player_) ->
			player = player_
			name_txt.string = player.name
			if player == Player.me().id
				name_txt.color = rgba(0,255,0,255)
			else
				name_txt.color = rgba(255,0,0,255)

		render : ->
			unless player?
				return

			UI.draw bg

			hp_txt.string = "HP : #{player.hp}"

			if player.visible
				UI.draw avatar
				UI.draw name_txt
				UI.draw hp_txt