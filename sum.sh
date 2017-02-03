#!/bin/bash
SCORE=$(grep 'You solved' "$1"  | awk '{sum += $6} END {print sum}')
MAZES=$(grep 'You solved' "$1" | wc -l)

echo "Report '$1':"
echo "  Solved $MAZES mazes with total score of $SCORE"
