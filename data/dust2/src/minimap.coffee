module.exports =
	make_minimap : ->
		sp = new Sprite
		sp.originX = 0
		sp.originY = 0
		tex = new Texture
		tex.loadFromFile "textures/minimap.jpg"
		sp.texture = tex
		H = 0.6 * UI.height
		padding = 0.05 * UI.height
		sp.scaleX = sp.scaleY = H / tex.height
		sp.x = padding
		sp.y = padding
		sp
