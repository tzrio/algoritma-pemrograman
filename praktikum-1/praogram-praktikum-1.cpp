#include <iostream> 
using namespace std; 
 
int main(){ 
    float volume, luas, ps, ts, tp; 
 
    cout << "Kalkulator Penghitung Volume Prisma Segitiga Siku-Siku (cm)\n\n"; 
    cout << "Masukan Nilai Panjang Segitiga = "; 
    cin >> ps; 
    cout << "Masukan Nilai Tinggi Segitiga = "; 
    cin >> ts; 
    cout << "\n"; 
 
    luas = 0.5 * ps * ts; 
 
    cout << "Didapatkan Luas Alas = " << luas << " cm^2\n" << endl; 
 
    cout << "Masukan Nilai Tinggi Prisma = "; 
    cin >> tp; 
    cout << "\n"; 
 
    volume = luas * tp; 
 
    cout << "Maka Volume Prisma = " << volume << " cm^3\n" << endl; 
 
    return 0; 
}; 
