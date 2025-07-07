awk '$1 ~ /^[0-9]+$/ && $1 > 50000 {x += $9; n++} END {if (n > 0) printf "variable lfin equal %.6f\n", x/n; else print "No data"}' equi.log > in.lfin
