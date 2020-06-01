#include "config.h"
#include "quadtree.h"


QTree* init_qtree( Pos p1, Pos p2 ){
    QTree* qtree = ( QTree* ) malloc ( sizeof( QTree ) );
    
    copy_pos( &qtree -> p1, &p1 );
    copy_pos( &qtree -> p2, &p2 );
    
    qtree -> list = init_qnode_list( NULL );
    qtree -> size = 0;
    
    qtree -> nw = NULL;
    qtree -> ne = NULL;
    qtree -> sw = NULL;
    qtree -> se = NULL;
    
    return qtree;
}


QNode* init_qnode( Pos pos, Cell cell){
    QNode* qnode = ( QNode* ) malloc ( sizeof ( QNode ) );

    copy_pos( &qnode -> pos, &pos );
    copy_cell( &qnode -> cell, &cell );
        
    return qnode;
}

QNodeList* init_qnode_list( QNode* node ){
    QNodeList* list = ( QNodeList* ) malloc ( sizeof ( QNodeList ) );

    list -> node = node;
    list -> next = NULL;

    return list;
}

void insert_node( QTree* qtree, QNode* node ){

    Pos p1 = qtree -> p1;
    Pos p2 = qtree -> p2;
    Pos pos = node -> pos;
    
    if( !VALID_RANGE( pos, p1, p2 ) )
        return;

    qtree -> size++;
    
    if( IN_RANGE( p1, p2 ) ){
	
	if( qtree -> list -> node == NULL ){
	    qtree -> list -> node = node;
	    
	}else{
	    QNodeList* list = qtree -> list;
	    while( list -> next != NULL )
	        list = list -> next;
	    
	    list -> next = init_qnode_list( node );
	}
	
        return;
    }
    
    Pos q1, q2;
    init_pos( &q1,
              WESTSIDE( pos, p1, p2 )  ? p1.x : HALFX( p1, p2 ),
              NORTHSIDE( pos, p1, p2 ) ? p1.y : HALFY( p1, p2 ) );
    
    init_pos( &q2,
              EASTSIDE( pos, p1, p2 )  ? p2.x : HALFX( p1, p2 ),
              SOUTHSIDE( pos, p1, p2 ) ? p2.y : HALFY( p1, p2 ) );
    
    QTree** branch = select_branch( qtree, pos );
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

    QNodeList* list = qtree -> list;
    
    if( list -> node != NULL ){
        
        if( equal_pos( &list -> node -> pos, &pos ) ){
	    
            return list -> node;
	    
	}else{
            
	    list = list -> next;
            
	    while( list != NULL ){

		if( equal_pos( &list -> node -> pos, &pos ) ){
		    return list -> node;
		}

		list = list -> next;
	    }

            return NULL;
	}
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

    QNodeList* list = qtree -> list;
	    
    while( list != NULL ){
	if( list -> node != NULL ){
	    printf( "qtree -> node = pos: (%hhu, %hhu) | ship: %hhu | state: %hhu\n",        
		    list -> node -> pos.x,
		    list -> node -> pos.y,
		    list -> node -> cell.ship,
		    list -> node -> cell.state );
	}
	
	list = list -> next;
    }
    

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

    free_qnode_list( qtree -> list );
    
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

void free_qnode_list( QNodeList* list ){

    if( list -> next != NULL )
	free_qnode_list( list -> next );
   
    free( list );
    return;
}
