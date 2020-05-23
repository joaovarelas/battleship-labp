#!/bin/sh

(cat $1; cat) | ./run_client_quadtree.sh
