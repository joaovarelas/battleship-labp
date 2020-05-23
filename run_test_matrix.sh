#!/bin/sh

(cat $1; cat) | ./run_client_matrix.sh
