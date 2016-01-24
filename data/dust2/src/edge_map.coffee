module.exports =
	make_edgemap : (nodes,edges) ->
		edge_map = {}
		set = (X,Y) ->
			detail_map = edge_map[X] ?= {}
			detail_map[Y] = 1
			list = detail_map.edge_list ?= []
			list.push Y

		for edge in edges
			set edge[0], edge[1]
			set edge[1], edge[0]

		edge_list : (node_id) ->
			edge_map[node_id].edge_list

		check_can_go : (node_id_a,node_id_b) ->
			edge_map[node_id_a][node_id_b]?