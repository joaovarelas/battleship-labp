#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "board.h"
#include "ship.h"


typedef struct _QNode {

    Pos pos;
    Cell cell;
    
} QNode;


typedef struct _QNodeList {
    
    QNode* node;
    struct _QNodeList* next;
    
} QNodeList;

typedef struct _QTree {

    Pos p1, p2;

    QNodeList* list;

    unsigned short size;
       
    QTree* nw;
    QTree* ne;
    QTree* sw;
    QTree* se;
    
} QTree;


QTree* init_qtree( Pos p1, Pos p2 );
QNode* init_qnode( Pos pos, Cell cell );
QNodeList* init_qnode_list( );

void insert_node( QTree* qtree, QNode* node );
QNode* get_node( QTree* qtree, Pos pos );

QTree** select_branch( QTree* qtree, Pos pos );

void print_qtree( QTree* qtree );

void free_qtree( QTree* qtree );
void free_qnode_list( QNodeList* list );

#endif
