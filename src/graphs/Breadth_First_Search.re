type node = {
    id: string,
    neighbours: list(string),
};
type returnResult = {
    level: Hashtbl.t(string, int), 
    parent: Hashtbl.t(string, string)
};

exception NodeNotExist(string);

let toHashTbl = adj_list => {
    let adj_tbl = Hashtbl.create(List.length(adj_list));
    let insert = node => {
        Hashtbl.add(adj_tbl, node.id, node.neighbours);
    };
    let validateNeighbours = node => {
        List.iter(neighbour => {
            switch (Hashtbl.find(adj_tbl, neighbour)) {
            | exception Not_found => raise(NodeNotExist(neighbour));
            | _ => ();
            };
        }, node.neighbours);
    };

    List.iter(insert, adj_list);
    List.iter(validateNeighbours, adj_list);

    adj_tbl;
};

let visit = (node_id, ~adj_tbl, ~level, ~parent,~current_level) => {
    switch (Hashtbl.find(level, node_id)) {
    | exception Not_found => {
        Hashtbl.add(level, node_id, current_level);
        let neighbours = Hashtbl.find(adj_tbl, node_id);
        let markParent = neighbour_id => {
            switch (Hashtbl.find(parent, neighbour_id)) {
                | exception Not_found => Hashtbl.add(parent, neighbour_id, node_id);
                | _ => ();
            }
        };
        List.iter(markParent, neighbours);
        neighbours;   
    }
    | _ => ([]);
    };
};

let rec traverse = (frontier, adj_tbl, level, parent, current_level) => {
    let updateFrontier = (_, node_id) => {
        let visit = visit(
            ~adj_tbl = adj_tbl, 
            ~level = level,
            ~parent = parent,
            ~current_level = current_level);

        let neighbours = visit(node_id);
        neighbours;
    };
    let frontier_empty = List.length(frontier) == 0;
    if (!frontier_empty) {
        let prev_frontier = frontier;
        let frontier = List.fold_left(updateFrontier, [], prev_frontier);
        traverse(frontier, adj_tbl, level, parent, current_level + 1);
    };
};

let markNodeIfUnvisited = (node_id, _, ~level, ~parent) => {
    switch (Hashtbl.find(level, node_id)) {
        | exception Not_found => {
            Hashtbl.add(level, node_id, -1);
            Hashtbl.add(parent, node_id, "null");
        }
        | _ => ();
    };
};

let breadthFirstSearch = (adj_list, root_id) => {
    let num_nodes = List.length(adj_list);
    let level = Hashtbl.create(num_nodes);
    let parent = Hashtbl.create(num_nodes);
    Hashtbl.add(parent, root_id, root_id);

    let adj_tbl = toHashTbl(adj_list);
    traverse([root_id], adj_tbl, level, parent, 0);
    
    let markNodeIfUnvisited = markNodeIfUnvisited(~level = level, ~parent = parent);
    Hashtbl.iter(markNodeIfUnvisited, adj_tbl);
    {level: level, parent: parent};
};