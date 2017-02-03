#!/bin/sh
for t in Test*; do
  echo "Running maze $t..."
  ./maze "$t"
  echo

  if [ $? -ne 0 ]; then
    echo "Failed to run maze $t"
    exit 1
  fi
done

echo "All mazes completed!"
