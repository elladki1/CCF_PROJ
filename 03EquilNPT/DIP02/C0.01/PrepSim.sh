#prepare the files for the simulation
TT=(0.45 0.65 0.85 1.05 1.20) #array of temperatures you'll run the simulation for

for tt in ${TT[*]}
do
    echo T$tt #prints Ttt in terminal ex. T0.30...
    mkdir T$tt #in current directory create new directory for that temperature, named Ttt
    cd T$tt #step into that newly created directory

        cp ../in.NPT ./ #from the directory above it, copy the in.NPT file

    cd ../ #go back up one directory to do the same for the next tt in array TT
done