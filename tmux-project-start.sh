#! /bin/sh
tmux new-session -s "kiss" -d
tmux split-window -v
tmux split-window -h
tmux select-pane -t 0
tmux split-window -h
tmux select-pane -t 0
tmux send-keys 'cd memoria_swap && make clean && make start' C-m
sleep 1.5
tmux select-pane -t 1
tmux send-keys 'cd cpu && make clean && make start' C-m
sleep 1.5
tmux select-pane -t 2
tmux send-keys 'cd kernel && make clean && make start' C-m
sleep 1.5
tmux select-pane -t 3
tmux send-keys 'cd consola' C-m
tmux send-keys 'make clean && make start'
tmux a -t kiss
