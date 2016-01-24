module.exports =
	make_sprite : (texfile,width,height) ->
		sp = new Sprite
		sp.originX = 0
		sp.originY = 0

		tex = new Texture
		tex.loadFromFile "textures/#{texfile}"
		sp.texture = tex

		if width? && height?
			sp.scaleX = width / tex.width
			sp.scaleY = height / tex.height
		else if width?
			sp.scaleX = sp.scaleY = width / tex.width
		else if height?
			sp.scaleX = sp.scaleY = height / tex.height

		sp