#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>


using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");


struct masina {
    int pozLin, pozCol;
    int ocupat; //indicele ride-ului
    int nrRute;
    vector<int> rute;
};

masina masini[1001];

struct ride {
    int startT, endT; //start timp, finish timp
    int xStart, yStart; //pozitie initiala
    int xStop, yStop; //pozitie finala
    int distanta; //distanta cursei
    bool taken;
};

ride curse[10005];

int calculDistanta(int x, int y, int a, int b) {
    return abs(a - x) + abs(b - y);
}

int rutaOptima(int **mat, int F, int N, int m) {
    int MIN = 2e9, index=-1;
    for(int i = 0; i < N; ++i) {
        if(curse[i].taken == 0 && mat[m][i] < MIN) {
            MIN = mat[m][i];
            index = i;
        }
    }

    if(index!=-1)
        curse[index].taken = 1;

    return index;
}

int main() {
    int R, C, F, N, B, T, a, b, x, y, s, f;

    fin >> R >> C >> F >> N >> B >> T;

    for(int i = 0; i < N; ++i) {
        fin >> curse[i].xStart >> curse[i].yStart >> curse[i].xStop >> curse[i].yStop >> curse[i].startT >> curse[i].endT;
        curse[i].distanta = calculDistanta(curse[i].xStart, curse[i].yStart, curse[i].xStop,  curse[i].yStop);
    }




    int **matDist = new int*[F];
    for(int i = 0; i < F; ++i) {
        masini[i].ocupat = -1;

        matDist[i] = new int[N];
        for(int j = 0; j < N ; ++j) {
            matDist[i][j] = calculDistanta(masini[i].pozLin, masini[i].pozCol, curse[j].xStart, curse[j].yStart);
        }
    }


    int timp = 0;
    while(timp < T) {
        for(int m = 0; m < F; ++m) {
            if(masini[m].ocupat != -1) {
                if(timp >= curse[masini[m].ocupat].startT) {
                    curse[masini[m].ocupat].distanta--;
                    if (curse[masini[m].ocupat].distanta == 0) {
                        masini[m].nrRute++;
                        masini[m].rute.push_back(masini[m].ocupat);
                        masini[m].ocupat = -1;

                        for (int j = 0; j < N; ++j) {
                            if(curse[j].taken == 0) {
                                matDist[m][j] = calculDistanta(curse[masini[m].ocupat].xStop, curse[masini[m].ocupat].yStop,
                                                               curse[j].xStart, curse[j].yStart);
                            }
                        }
                        masini[m].ocupat = rutaOptima(matDist, F, N, m);

                    }
                }
            }
            else if(masini[m].ocupat == -1) {
                masini[m].ocupat = rutaOptima(matDist, F, N, m);

            }
        }

        timp++;
    }

    for(int i = 0; i < F; ++i) {
        fout<<masini[i].nrRute<<' ';
        for(int j = 0; j < masini[i].rute.size(); ++j)
            fout<<masini[i].rute[j]<<' ';
        fout << '\n';
    }


    for(int i = 0; i <= F; ++i) {
        delete[] matDist[i];
    }

    delete[] matDist;
    return 0;
}
