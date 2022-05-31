#! /bin/bash

#por cada directorio dentro del directorio actual
for dir in */ ; do
	# chequear que no sea un symlink, si lo es saltarlo
	[ -L "${dir%/}" ] && continue
	#abrir code para ese directorio
	code "$dir"
done
