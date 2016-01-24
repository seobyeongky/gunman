{PADDING,SMALL_PADDING,TXT_HEIGHT} = require './consts'
{make_sprite} = require './utils/sprite'

module.exports =
	make_selbox : ->
		bg = make_sprite "frame0.png", 0.4 * UI.width, 0.2 * UI.height
		bg.x = PADDING
		bg.y = UI.height - PADDING - bg.scaleY * bg.texture.height

		make_txt = (i) ->
			txt = new Text
			txt.color = rgba(255,255,255,255)
			txt.characterSize = TXT_HEIGHT
			txt.x = bg.x + SMALL_PADDING
			txt.y = bg.y + SMALL_PADDING + i * (txt.characterSize + SMALL_PADDING)
			txt.string = "#{i + 1}. 선택지 #{i + 1}!!"
			txt

		txtlist = [0...3].map make_txt

		set_player : ->

		render : ->
			UI.draw bg
			txtlist.forEach (txt) ->
				UI.draw txt