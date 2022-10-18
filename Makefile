all:
        clang++ apriori.cpp -o apriori.out
        ./apriori.out datafile1.txt 10
        ./apriori.out datafile1.txt 20
        ./apriori.out datafile1.txt 30
        ./apriori.out datafile1.txt 50


clean:
        rm -rf apriori.out


