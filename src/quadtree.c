#include "config.h"
#include "quadtree.h"


QTree* init_qtree( Pos p1, Pos p2 ){
    QTree* qtree = ( QTree* ) malloc ( sizeof( QTree ) );

    copy_pos( &qtree -> p1, &p1 );
    copy_pos( &qtree -> p2, &p2 );
    
    qtree -> node = NULL;

    qtree -> nw = NULL;
    qtree -> ne = NULL;
    qtree -> sw = NULL;
    qtree -> se = NULL;

    qtree -> empty = true;
    
    return qtree;
}


QNode* init_qnode( Pos pos, Cell cell){
    QNode* qnode = ( QNode* ) malloc ( sizeof ( QNode ) );

    copy_pos( &qnode -> pos, &pos );
    copy_cell( &qnode -> cell, &cell );
        
    return qnode;
}


void insert_node( QTree* qtree, QNode* node ){

    Pos p1 = qtree -> p1;
    Pos p2 = qtree -> p2;
    Pos pos = node -> pos;
    
    if( !VALID_RANGE( pos, p1, p2 ) )
        return;
    
    qtree -> empty = false;
    
    if( IN_RANGE( p1, p2 ) ){
        if( qtree -> node == NULL )
            qtree -> node = node;
        
        
        return;
    }
    
    QTree** branch = select_branch( qtree, pos );

    Pos q1, q2;
    init_pos( &q1,
              WESTSIDE( pos, p1, p2 )  ? p1.x : HALFX( p1, p2 ),
              NORTHSIDE( pos, p1, p2 ) ? p1.y : HALFY( p1, p2 ) );
    
    init_pos( &q2,
              EASTSIDE( pos, p1, p2 )  ? p2.x : HALFX( p1, p2 ),
              SOUTHSIDE( pos, p1, p2 ) ? p2.y : HALFY( p1, p2 ) );

    if( *branch == NULL )
        *branch = init_qtree( q1, q2 );

        
    insert_node( *branch, node );
    
    
    return;
}


QNode* get_node( QTree* qtree, Pos pos ){

    if( qtree == NULL )
        return NULL;
    
    Pos p1 = qtree -> p1;
    Pos p2 = qtree -> p2;
    
    if( !VALID_RANGE( pos, p1, p2 ) )
        return NULL;

    QNode* node = qtree -> node;
    
    if( node != NULL ){
        
        if( node -> pos.x == pos.x && node -> pos.y == pos.y )
            return qtree -> node;
        else
            return NULL;
    }
    
    QTree** branch = select_branch( qtree, pos );

    if( *branch == NULL )
        return NULL;
    
    return get_node( *branch, pos );
}


QTree** select_branch( QTree* qtree, Pos pos ){
    
    Pos p1 = qtree -> p1;
    Pos p2 = qtree -> p2;
    
    if( NORTHWEST( pos, p1, p2 ) )
        return &qtree -> nw;
    
    else if( NORTHEAST( pos, p1, p2 ) )
        return &qtree -> ne;
    
    else if( SOUTHWEST( pos, p1, p2 ) )
        return &qtree -> sw;
    
    else if( SOUTHEAST( pos, p1, p2 ) )
        return &qtree -> se;
    

    return NULL;
}

    
void print_qtree( QTree* qtree ){
    
    if( qtree -> node != NULL )
        printf( "qtree -> node -> pos = (%hhu, %hhu)\n" \
                "qtree -> node -> cell.ship,state = %hhu, %hhu\n",
                qtree -> node -> pos.x,
                qtree -> node -> pos.y,
                qtree -> node -> cell.ship,
                qtree -> node -> cell.state );

    if( qtree -> nw != NULL )
        print_qtree( qtree -> nw );
    
    if( qtree -> ne != NULL )
        print_qtree( qtree -> ne );
    
    if( qtree -> sw != NULL )
        print_qtree( qtree -> sw );
    
    if( qtree -> se != NULL )
        print_qtree( qtree -> se );
    
    return;
}


void free_qtree( QTree* qtree ){

    if( qtree -> node != NULL )
        free( qtree -> node );

    
    if( qtree -> nw != NULL )
        free_qtree( qtree -> nw );
    
    if( qtree -> ne != NULL )
        free_qtree( qtree -> ne );
    
    if( qtree -> sw != NULL )
        free_qtree( qtree -> sw );
    
    if( qtree -> se != NULL )
        free_qtree( qtree -> se );

    free( qtree );
        
    return;
}

