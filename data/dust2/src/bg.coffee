{make_sprite} = require './utils/sprite'

module.exports =
	make_bg : ->
		sp = make_sprite "bg.jpg", UI.width, UI.height
		sp