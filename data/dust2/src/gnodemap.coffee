{nodes,edges} = require './nodes.json'
{make_sprite} = require './utils/sprite'
{SMALL_PADDING} = require './consts'
{make_edgemap} = require './edge_map'

edge_map = make_edgemap nodes, edges

module.exports =
	make_gnodemap : (X,Y,S) ->
		gnodes = Object.keys(nodes).map (node_id) ->
			node = nodes[node_id]
			sp = make_sprite "green_dot.png", SMALL_PADDING, SMALL_PADDING
			sp.x = X + node.pos[0] * S
			sp.y = Y + node.pos[1] * S

			render : ->
				UI.draw sp

		render : ->
			for gnode in gnodes
				gnode.render()
			return