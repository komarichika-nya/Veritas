while :; do
    python b.py || break
    ./a < a.in | awk '
        NR==FNR { a[NR]=$1; next }
        { if ((a[FNR]-$1)^2 > 1e-9) exit 1 }
    ' a.out - && echo "ac" || { echo "wa"; break; }
done
