#include <iostream>
#include <cmath>
#include <windows.h>
using namespace std;

class Matrix{

private:
    int n;
    int m;
    int rank;
    int *base;
    int *nez;
    double** matrix;
    double** F;
    bool expanded;
    double **CreateMatrix(int n, int m) {
        double **matrix = new double *[n];
        for (int count = 0; count < n; count++)
            matrix[count] = new double[m];
        cout << "Элементы нерасширенной матрицы через пробел построчно: " << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> matrix[i][j];
            }
        }
        return matrix;
    }
    double** Expand(double *matrix[], int n, int m){
        double **Ematrix = new double *[n];
        for (int count = 0; count < n; count++)
            Ematrix[count] = new double[m+1];
        cout<<"Введите солбец свободных членов: "<<endl;
        this->m++;
        for(int i = 0; i<n; i++){
            for(int j = 0; j<m; j++){
                Ematrix[i][j]=matrix[i][j];
            }
            cin>>Ematrix[i][m];
        }

        expanded=true;
        return Ematrix;
    }
    void PrintMatrix(double *matrix[], int n, int m) {
        cout<<endl;
        if(!expanded) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
        }else{
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m-1; j++) {
                    cout << matrix[i][j] << " ";
                }
                cout<<"|"<<matrix[i][m-1];
                cout << endl;
            }
        }
        cout<<endl;
    }
    void Replace(double *matrix[], int n, int m, int i, int j) {        //перестановка строк i j
        double a = 0;
        if(n>i&&n>j) {
            for (int k = 0; k < m; k++) {
                a = matrix[i][k];
                matrix[i][k] = matrix[j][k];
                matrix[j][k] = a;
            }
        }
    }
    void Multiply(double*matrix[], int m, int i, double l){   //умножение строки на число
        for(int k=0; k<m; k++){
            matrix[i][k]*=l;
        }
    }
    void Gauss(double*matrix[], int n, int m){
        rank=0;
        int *tmpb= new int[m];
        int *tmpn = new int[m];
        int num=0;
        int u = 0;
        for(int col=0; col<m; col++){
            bool a = true;
            for(int row=num; row<n; row++){

                if(this->matrix[row][col]!=0){
                    rank++;
                    Multiply(this->matrix, m, num, 1/this->matrix[num][col]);

                    num++;
                    for(int r=num; r<n; r++){
                        double c=-1*this->matrix[r][col]/this->matrix[num-1][col];
                        // cout<<c<<endl;
                        for(int q=col; q<m; q++){
                            this->matrix[r][q]+=c*this->matrix[num-1][q];
                        }
                    }
                    for(int r=num-2; r>=0; r--){
                        double c=-1*this->matrix[r][col]/this->matrix[num-1][col];
                        // cout<<c<<endl;
                        for(int q=col; q<m; q++){
                            this->matrix[r][q]+=c*this->matrix[num-1][q];
                        }
                    }
                    Replace(this->matrix, n, m, row, num-1);
                    tmpb[rank-1]=col;
                    a=false;
                    break;
                }
            }
            if(a) {
                tmpn[u] = col;
                u++;
            }
        }
        base = new int[rank];
        for(int i = 0; i<rank; i++){
            base[i]=tmpb[i];
        }
        delete [] tmpb;
        if(isSolvable(matrix, n, m, expanded)){
            nez=new int[m-rank-1];
            for(int i = 0; i<m-rank-1; i++){
                nez[i]=tmpn[i];
            }
            delete[] tmpn;
        }

        for(int row = 0; row<n; row++){
            for(int col=0; col<m; col++){
                if(matrix[row][col]==-0||abs(matrix[row][col])<1e-16) matrix[row][col]=0;
            }
        }
    }
bool isSolvable(double *matrix[], int n, int m, bool expanded){
        if(expanded){
            if(base[rank-1]==m-1){
               cout<<"Матрица несовместна"<<endl;
                return false;
            }else{
                cout<<"Матрица совместна"<<endl;
                return true;
            }
        }else{
            cout<<"Расширьте матрицу, чтобы решить. (expand())"<<endl;
            return false;
        }
    }

public:
    Matrix(){

        cout<<"Введите размер нерасширенной матрицы: "<<endl;
        cin>>this->n;
        cin>>this->m;
        matrix=CreateMatrix(this->n, this->m);
        cout<<"Расширить матрицу? (Y/N)"<<endl;
        char Answer;
        cin>>Answer;
        if(Answer=='Y'||Answer=='y'){
            matrix=Expand(matrix, n, m);
        }else{
            expanded = false;
        }


    };
    ~Matrix(){
        for (int count = 0; count < n; count++)
            delete []matrix[count];
    };
    void expand(){
        matrix=Expand(matrix, n, m);
        expanded=true;
    }
    void print(){
        PrintMatrix(this->matrix, n, m);
    }
    void replace(int i, int j){
        Replace(this->matrix, n, m, i, j);
    }
    void multiply(int row, double alpha){
        Multiply(this->matrix, m, row, alpha);
    }
    void gauss(){
        Gauss(matrix, n, m);
    }

    void solve(){
        gauss();
        if(isSolvable(matrix, n , m, expanded)){
        cout<<"частное решение: "<<endl;
        for(int i = 0; i<rank; i++){
            cout<<"x"<<base[i]<<"="<<matrix[i][m-1]<<endl;

        }
        for(int i = 0; i<m-rank-1; i++){
            cout<<"x"<<nez[i]<<"=0 "<<endl;
        }
        F=new double *[m];
        cout<<"Фундаментальная матрица:"<<endl;
        for (int count = 0; count < m; count++)
            F[count] = new double[m-rank-1];
        for(int i = 0; i<rank; i++) {
            for (int j = 0; j < m - 1 - rank; j++) {
                F[i][j]=-1*matrix[base[i]][nez[j]];
                cout<<F[i][j]<<" ";
            }
            cout<<endl;
        }
        for(int i = 0; i<m-1-rank; i++){
            for(int j = 0; j<m-rank-1; j++){
                if(i == j){ cout<<"1 ";
                }else{ cout<< "0 ";
                }
            }
            cout<<endl;
        }

    }
    }

};

int main() {

    SetConsoleOutputCP(CP_UTF8);

    Matrix *A= new Matrix();

    A->print();
    A->solve();
    A->print();
    return 0;
}