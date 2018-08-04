type node = {
    id: string,
    neighbours: list(string),
};
type returnResult = {
    level: Hashtbl.t(string, int), 
    parent: Hashtbl.t(string, string)
};
let breadthFirstSearch: (list(node), string) => returnResult;