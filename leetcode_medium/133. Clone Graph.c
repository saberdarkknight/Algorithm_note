void dfs ( struct Node *new_node, struct Node *s,  struct Node **visit){
    new_node->val = s->val;
    new_node->numNeighbors = s->numNeighbors;
    visit[s->val] = new_node;
    if ( new_node->numNeighbors == 0 ) return;
    new_node->neighbors = (struct Node ** ) malloc(sizeof(struct Node * )* new_node->numNeighbors);

    for ( int i =0; i< s->numNeighbors; i++ ){
        if ( visit[ s->neighbors[i]->val ] == NULL ){
            new_node->neighbors[i] = (struct Node * ) malloc(sizeof(struct Node ));
            dfs( new_node->neighbors[i], s->neighbors[i], visit );
        } else {
            new_node->neighbors[i] = visit[ s->neighbors[i]->val ] ;
        }
    }
    return;
}


struct Node *cloneGraph(struct Node *s) {
    if ( s==NULL ){
        return s;
    }
    struct Node * output = (struct Node *) malloc(sizeof(struct Node ));
    struct Node *visit[101] = {NULL};

    dfs(output, s, visit);
    return output;   
}