#include <iostream> //Untuk input/output seperti cin, cout. Contoh: cout << "Hello";
#include <string> //Agar bisa pakai tipe data string. Misal: string nama = "Roihan";
#include <fstream> //Untuk file I/O (input/output ke file), seperti ifstream, ofstream.
#include <sstream> //Untuk manipulasi string, seperti stringstream untuk parsing.
#include <ctime> //Untuk mendapatkan tanggal dan waktu saat ini, seperti time_t, localtime.
#include <limits> //Untuk batasan input, seperti numeric_limits<int>::max() untuk input integer.
#include <algorithm> //Untuk fungsi-fungsi algoritma seperti sort, find, dll.
#include <iomanip> //Untuk manipulasi format output, seperti setprecision, setw, dll.
#include <chrono> //Untuk fungsi waktu, seperti this_thread::sleep_for untuk delay.
#include <thread> // Untuk fungsi multithreading, seperti this_thread::sleep_for untuk delay.
using namespace std;

// Konstanta untuk ukuran maksimum array
const int MAX_DATA = 100;

// --- Deklarasi Struct Data ---
struct User {
    string username;
    string password;
    string role;
    string phoneNumber;
};

struct Paket {
    int id;
    string nama;
    string deskripsi;
    int kapasitas;
    int harga;
};

struct Reservasi {
    int reservasiId;
    string username;
    int paketId;
    int jumlahOrang;
    double totalBiaya;
    string tanggalPemesanan;
    string tanggalReservasi;
    string status;
    string refundRekening;
};

// --- Deklarasi Prototipe Fungsi ---
// Fungsi utilitas & UI
void clearScreen();
void enterToContinue();
void printHeader(const string& title);
void printMainMenuHeader();
void printSubMenuHeader(const string& menuName, const string& username);
int getIntegerInput();
string getStringInput(const string& prompt, bool allowEmpty = false);
string getCurrentDate();
bool isValidDate(const string& dateString);
bool isValidPhoneNumber(const string& phoneNum);
bool isValidRekeningNumber(const string& rekeningNum);

// Fungsi File I/O
void bacaDataUser(User users[], int& userCount);
void tulisDataUser(const User daftarUser[], int userCount);
void bacaDataPaket(Paket pakets[], int& paketCount);
void tulisDataPaket(const Paket daftarPaket[], int paketCount);
void bacaDataReservasi(Reservasi reservations[], int& reservasiCount);
void tulisDataReservasi(const Reservasi daftarReservasi[], int reservasiCount);

// Fungsi Login & Registrasi
User* prosesLogin(User daftarUser[], int userCount);
void prosesRegistrasi(User daftarUser[], int& userCount);

// Fungsi Menu Utama
void tampilkanMenuAdmin(User& admin, Paket daftarPaket[], int& paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount);
void tampilkanMenuReservator(User& reservator, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount);

// Fungsi lainnya
void tambahPaketBaru(Paket daftarPaket[], int& paketCount);
void hapusPaket(Paket daftarPaket[], int& paketCount);
int generateUniquePaketId(const Paket daftarPaket[], int paketCount);
void editPaket(Paket daftarPaket[], int paketCount);
void lihatSemuaReservasi(const Reservasi daftarReservasi[], int reservasiCount, Paket daftarPaket[], int paketCount, User daftarUser[], int userCount);
void kelolaStatusReservasi(Reservasi daftarReservasi[], int& reservasiCount, Paket daftarPaket[], int paketCount, User daftarUser[], int userCount);
void buatReservasiBaru(User& currentUser, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount);
void kelolaReservasiUser(User& currentUser, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount);
void editNomorTelepon(User& currentUser, User daftarUser[], int userCount);
Paket* findPaketById(Paket daftarPaket[], int paketCount, int id);
Reservasi* findReservasiById(Reservasi daftarReservasi[], int reservasiCount, int id);
User* findUserByUsername(User daftarUser[], int userCount, const string& username);
int generateUniqueId(const Reservasi daftarReservasi[], int reservasiCount, const string& tanggalReservasi);
void cetakNota(const Reservasi& reservasi, const User& user, const Paket& paket);
void printNotaKeFile(const Reservasi& reservasi, const User& user, const Paket& paket);


int main() {
    User daftarUser[MAX_DATA];
    Paket daftarPaket[MAX_DATA];
    Reservasi daftarReservasi[MAX_DATA];

    int userCount = 0;
    int paketCount = 0;
    int reservasiCount = 0;

    // Membaca data dari file ke dalam array
    bacaDataUser(daftarUser, userCount);
    bacaDataPaket(daftarPaket, paketCount);
    bacaDataReservasi(daftarReservasi, reservasiCount);

    while (true) {
        clearScreen();
        printMainMenuHeader();
        cout << " [1] Login Akun\n";
        cout << " [2] Registrasi Akun Baru\n";
        cout << " [3] Keluar\n";
        cout << " +--------------------------------------+\n";
        cout << "   Pilihan Anda: ";
        int pilihan = getIntegerInput();

        switch (pilihan) {
            case 1: { // Login
                User* currentUser = prosesLogin(daftarUser, userCount);
                if (currentUser != nullptr) {
                    if (currentUser->role == "admin") {
                        tampilkanMenuAdmin(*currentUser, daftarPaket, paketCount, daftarReservasi, reservasiCount, daftarUser, userCount);
                    } else if (currentUser->role == "reservator") {
                        tampilkanMenuReservator(*currentUser, daftarPaket, paketCount, daftarReservasi, reservasiCount, daftarUser, userCount);
                    }
                } else {
                    cout << "\n   [!] Login gagal! Username atau password salah.\n";
                    enterToContinue();
                }
                break;
            }
            case 2: // Register
                prosesRegistrasi(daftarUser, userCount);
                break;
            case 3: // Keluar
                cout << "\n   Terima kasih telah menggunakan sistem reservasi kami. Sampai jumpa!\n";
                this_thread::sleep_for(chrono::seconds(2));
                return 0;
            default:
                cout << "\n   [!] Pilihan tidak valid. Silakan coba lagi.\n";
                enterToContinue();
        }
    }

    return 0;
}
User* prosesLogin(User daftarUser[], int userCount) {
    clearScreen();
    printHeader("LOGIN");
    string username, password;
    cout << "   -> Username: ";
    getline(cin, username);
    cout << "   -> Password: ";
    getline(cin, password);

    for (int i = 0; i < userCount; ++i) {
        if (daftarUser[i].username == username && daftarUser[i].password == password) {
            cout << "\n   Login berhasil! Selamat datang, " << username << ".\n";
            this_thread::sleep_for(chrono::seconds(1));
            return &daftarUser[i];
        }
    }
    return nullptr;
}
void tampilkanMenuAdmin(User& admin, Paket daftarPaket[], int& paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount) {
    while (true) {
        clearScreen();
        printSubMenuHeader("MENU ADMIN", admin.username);
        cout << " [1] Tambah Paket Baru\n";
        cout << " [2] Edit Data Paket\n";
        cout << " [3] Hapus Paket\n";
        cout << " [4] Lihat Semua Reservasi\n";
        cout << " [5] Kelola Status Reservasi\n";
        cout << " [6] Kembali ke Menu Utama\n";
        cout << " [7] Keluar Aplikasi\n";
        cout << " +--------------------------------------+\n";
        cout << "   Pilihan Anda: ";
        int pilihan = getIntegerInput();

        switch (pilihan) {
            case 1:
                tambahPaketBaru(daftarPaket, paketCount);
                break;
            case 2:
                editPaket(daftarPaket, paketCount);
                break;
            case 3:
                hapusPaket(daftarPaket, paketCount);
                break;
            case 4:
                lihatSemuaReservasi(daftarReservasi, reservasiCount, daftarPaket, paketCount, daftarUser, userCount);
                break;
            case 5:
                kelolaStatusReservasi(daftarReservasi, reservasiCount, daftarPaket, paketCount, daftarUser, userCount);
                break;
            case 6:
                return;
            case 7:
                cout << "\n   Keluar dari aplikasi...\n";
                exit(0);
            default:
                cout << "\n   [!] Pilihan tidak valid.\n";
                enterToContinue();
        }
    }
}
void tampilkanMenuReservator(User& reservator, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount) {
    while (true) {
        clearScreen();
        printSubMenuHeader("MENU RESERVATOR", reservator.username);
        cout << " [1] Buat Reservasi Baru\n";
        cout << " [2] Cek & Kelola Reservasi Saya\n";
        cout << " [3] Edit Nomor Telepon Saya\n";
        cout << " [4] Kembali ke Menu Utama\n";
        cout << " [5] Keluar Aplikasi\n";
        cout << " +--------------------------------------+\n";
        cout << "   Pilihan Anda: ";
        int pilihan = getIntegerInput();

        switch (pilihan) {
            case 1:
                buatReservasiBaru(reservator, daftarPaket, paketCount, daftarReservasi, reservasiCount, daftarUser, userCount);
                break;
            case 2:
                kelolaReservasiUser(reservator, daftarPaket, paketCount, daftarReservasi, reservasiCount, daftarUser, userCount);
                break;
            case 3:
                editNomorTelepon(reservator, daftarUser, userCount);
                break;
            case 4:
                return;
            case 5:
                cout << "\n   Keluar dari aplikasi...\n";
                exit(0);
            default:
                cout << "\n   [!] Pilihan tidak valid.\n";
                enterToContinue();
        }
    }
}
void printMainMenuHeader() {
    cout << "==========================================================\n";
    cout << "||                                                      ||\n";
    cout << "||              Selamat Datang di Program               ||\n";
    cout << "||                Reservasi Tiket Wisata                ||\n";
    cout << "||                         dan                          ||\n";
    cout << "||                   Penyewaan Tenda                    ||\n";
    cout << "||                                                      ||\n";
    cout << "==========================================================\n\n";
    cout << " +--------------------------------------+\n";
}
void printHeader(const string& title) {
    cout << "==========================================================\n";
    cout << "             " << title << "\n";
    cout << "==========================================================\n\n";
}
void printSubMenuHeader(const string& menuName, const string& username) {
    cout << "==========================================================\n";
    cout << "   " << menuName << " - (User: " << username << ")\n";
    cout << "==========================================================\n\n";
}
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "-"
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
       << (1900 + ltm->tm_year);
    return ss.str();
}
bool isValidDate(const string& dateString) {
    if (dateString.length() != 10 || dateString[2] != '-' || dateString[5] != '-') {
        return false;
    }
    try {
        int day = stoi(dateString.substr(0, 2));
        int month = stoi(dateString.substr(3, 2));
        int year = stoi(dateString.substr(6, 4));

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;

        if (month == 2) {
            bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            if (day > (isLeap ? 29 : 28)) return false;
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}
string getStringInput(const string& prompt, bool allowEmpty) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (allowEmpty || !input.empty()) {
            break;
        }
        cout << "      [!] Input tidak boleh kosong. Silakan masukkan lagi.\n";
    }
    return input;
}
bool isValidPhoneNumber(const string& phoneNum) {
    if (phoneNum.length() < 10 || phoneNum.length() > 13) return false;
    if (phoneNum.substr(0, 2) != "08") return false;
    for (char c : phoneNum) {
        if (!isdigit(c)) return false;
    }
    return true;
}
bool isValidRekeningNumber(const string& rekeningNum) {
    if (rekeningNum.length() < 10) return false; // Adjusted for more realistic length
    for (char c : rekeningNum) {
        if (!isdigit(c)) return false;
    }
    return true;
}
int generateUniqueId(const Reservasi daftarReservasi[], int reservasiCount, const string& tanggalReservasi) {
    int day = stoi(tanggalReservasi.substr(0, 2));
    int month = stoi(tanggalReservasi.substr(3, 2));
    int year = stoi(tanggalReservasi.substr(8, 2));

    int baseId = day * 1000000 + month * 10000 + year * 100;

    int maxSequence = 0;
    for (int i = 0; i < reservasiCount; ++i) {
        if (daftarReservasi[i].tanggalReservasi == tanggalReservasi) {
            int currentId = daftarReservasi[i].reservasiId;
            if (currentId / 100 == baseId / 100) {
                int currentSequence = currentId % 100;
                if (currentSequence > maxSequence) {
                    maxSequence = currentSequence;
                }
            }
        }
    }
    return baseId + maxSequence + 1;
}
int generateUniquePaketId(const Paket daftarPaket[], int paketCount) {
    if (paketCount == 0) {
        return 1;
    } else {
        int maxId = 0;
        for (int i = 0; i < paketCount; ++i) {
            if (daftarPaket[i].id > maxId) {
                maxId = daftarPaket[i].id;
            }
        }
        return maxId + 1;
    }
}
Paket* findPaketById(Paket daftarPaket[], int paketCount, int id) {
    for (int i = 0; i < paketCount; ++i) {
        if (daftarPaket[i].id == id) return &daftarPaket[i];
    }
    return nullptr;
}
Reservasi* findReservasiById(Reservasi daftarReservasi[], int reservasiCount, int id) {
    for (int i = 0; i < reservasiCount; ++i) {
        if (daftarReservasi[i].reservasiId == id) {
            return &daftarReservasi[i];
        }
    }
    return nullptr;
}
User* findUserByUsername(User daftarUser[], int userCount, const string& username) {
    for (int i = 0; i < userCount; ++i) {
        if (daftarUser[i].username == username) {
            return &daftarUser[i];
        }
    }
    return nullptr;
}
void cetakDanPrintNota(ostream& stream, const Reservasi& reservasi, const User& user, const Paket& paket) {
    // Semua logika "cout <<" atau "file <<" diganti menjadi "stream <<"
    stream << "+------------------------------------------+\n";
    stream << "|              NOTA RESERVASI              |\n";
    stream << "+------------------------------------------+\n";
    stream << "| ID Reservasi      : " << left << setw(18) << reservasi.reservasiId << " |\n";
    stream << "| Nama Pelanggan    : " << left << setw(18) << user.username << " |\n";
    stream << "| Nomor Telepon     : " << left << setw(18) << user.phoneNumber << " |\n";
    stream << "| Tgl. Pemesanan    : " << left << setw(18) << reservasi.tanggalPemesanan << " |\n";
    stream << "| Tgl. Reservasi    : " << left << setw(18) << reservasi.tanggalReservasi << " |\n";
    stream << "| Status            : " << left << setw(18) << reservasi.status << " |\n";
    stream << "|------------------------------------------|\n";
    stream << "| Detail Biaya                             |\n";
    stream << "|------------------------------------------|\n";
    string paketStr = paket.nama + " (Rp" + to_string(paket.harga) + ")";
    stream << "| Paket             : " << left << setw(18) << paketStr << " |\n";
    string htmStr = to_string(reservasi.jumlahOrang) + " x Rp10000";
    stream << "| HTM               : " << left << setw(18) << htmStr << " |\n";
    stream << "+------------------------------------------+\n";
    string totalStr = "Rp" + to_string(static_cast<long long>(reservasi.totalBiaya));
    stream << "| Total Biaya       : " << left << setw(18) << totalStr << " |\n";
    stream << "+------------------------------------------+\n";
    stream << "|              TERIMA KASIH                |\n";
    stream << "+------------------------------------------+\n";
}
void cetakNota(const Reservasi& reservasi, const User& user, const Paket& paket) {
    cout << "\n"; // Beri spasi sebelum mencetak ke konsol
    cetakDanPrintNota(cout, reservasi, user, paket);
}
void printNotaKeFile(const Reservasi& reservasi, const User& user, const Paket& paket){
    string folder = "nota/";
    // (Pastikan folder "nota/" ada, atau buat jika belum ada)
    string filename = folder + "NOTA-" + to_string(reservasi.reservasiId) + ".txt";
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "   [!] Error: Tidak bisa membuat file nota.\n";
        return;
    }
    cetakDanPrintNota(file, reservasi, user, paket);
    file.close();
}
int getIntegerInput() {
    int value;
    string line;
    while (true) {
        getline(cin, line);
        stringstream ss(line);
        if (ss >> value && ss.eof() && !line.empty()) {
            break;
        }
        cout << "   [!] Input tidak valid. Masukkan angka yang benar: ";
    }
    return value;
}
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void enterToContinue() {
    cout << "\n   Tekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//Fahmi
void prosesRegistrasi(User daftarUser[], int& userCount) {
    clearScreen();
    printHeader("REGISTRASI AKUN BARU");

    if (userCount >= MAX_DATA) {
        cout << "   [!] Database user penuh. Tidak bisa mendaftar saat ini.\n";
        enterToContinue();
        return;
    }

    string usernameBaru;
    while (true) {
        cout << "   -> Masukkan username baru: ";
        getline(cin, usernameBaru);
        
        if (usernameBaru.empty()) {
            cout << "      [!] Username tidak boleh kosong. Silakan masukkan lagi.\n";
            continue;
        }

        bool found = false;
        for (int i = 0; i < userCount; ++i) {
            if (daftarUser[i].username == usernameBaru) {
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        } else {
            cout << "      [!] Username '" << usernameBaru << "' sudah digunakan. Silakan pilih username lain.\n";
        }
    }

    string passwordBaru;
    cout << "   -> Masukkan password baru: ";
    while (true) {
        getline(cin, passwordBaru);
        if (!passwordBaru.empty()) {
            break;
        }
        cout << "      [!] Password tidak boleh kosong. Silakan masukkan lagi: ";
    }
    
    string phoneNumberBaru;
    while (true) {
        phoneNumberBaru = getStringInput("   -> Masukkan nomor telepon (cth: 08123456789): ");
        if (isValidPhoneNumber(phoneNumberBaru)) {
            break;
        }
        cout << "      [!] Nomor telepon tidak valid. Harus dimulai dengan '08' dan 10-13 digit.\n";
    }

    daftarUser[userCount] = {usernameBaru, passwordBaru, "reservator", phoneNumberBaru};
    userCount++;
    tulisDataUser(daftarUser, userCount);

    cout << "\n   ===============================================\n";
    cout << "   |            REGISTRASI BERHASIL!             |\n";
    cout << "   | Silakan login dengan akun yang baru dibuat. |\n";
    cout << "   ===============================================\n";
    enterToContinue();
}
void bacaDataUser(User users[], int& userCount) {
    ifstream file("reservator.csv");
    string line, username, password, role, phoneNumber;
    userCount = 0;
    if (!file.is_open()) return;
    while (getline(file, line) && userCount < MAX_DATA) {
        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, role, ',');
        getline(ss, phoneNumber);
        users[userCount] = {username, password, role, phoneNumber};
        userCount++;
    }
    file.close();
}
void tulisDataUser(const User daftarUser[], int userCount) {
    ofstream file("reservator.csv");
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file reservator.csv untuk menulis.\n";
        return;
    }
    for (int i = 0; i < userCount; ++i) {
        file << daftarUser[i].username << "," << daftarUser[i].password << "," << daftarUser[i].role << "," << daftarUser[i].phoneNumber;
        if (i < userCount - 1) {
            file << "\n";
        }
    }
    file.close();
}
void bacaDataPaket(Paket pakets[], int& paketCount) {
    ifstream file("paket.csv");
    string line, temp;
    paketCount = 0;
    if (!file.is_open()) return;
    while (getline(file, line) && paketCount < MAX_DATA) {
            stringstream ss(line);
            Paket p;
            getline(ss, temp, ','); if (!temp.empty()) p.id = stoi(temp);
            if (ss.peek() == '"') { ss.get(); getline(ss, p.nama, '"'); ss.get(); }
            else { getline(ss, p.nama, ','); }
            if (ss.peek() == '"') { ss.get(); getline(ss, p.deskripsi, '"'); ss.get(); }
            else { getline(ss, p.deskripsi, ','); }
            getline(ss, temp, ','); if (!temp.empty()) p.kapasitas = stoi(temp);
            getline(ss, temp); if (!temp.empty()) p.harga = stoi(temp);
            pakets[paketCount] = p;
            paketCount++;
    }
    file.close();
}
void tulisDataPaket(const Paket daftarPaket[], int paketCount) {
    ofstream file("paket.csv");
    for (int i = 0; i < paketCount; ++i) {
        file << daftarPaket[i].id << ",\"" << daftarPaket[i].nama << "\",\"" << daftarPaket[i].deskripsi << "\","
             << daftarPaket[i].kapasitas << "," << daftarPaket[i].harga << "\n";
    }
    file.close();
}
void bacaDataReservasi(Reservasi reservations[], int& reservasiCount) {
    ifstream file("reservasi.csv");
    string line, temp;
    reservasiCount = 0;
    if (!file.is_open()) return;
    while (getline(file, line) && reservasiCount < MAX_DATA) {
        stringstream ss(line);
        Reservasi r;
        getline(ss, temp, ','); if (!temp.empty()) r.reservasiId = stoi(temp);
        getline(ss, r.username, ',');
        getline(ss, temp, ','); if (!temp.empty()) r.paketId = stoi(temp);
        getline(ss, temp, ','); if (!temp.empty()) r.jumlahOrang = stoi(temp);
        getline(ss, temp, ','); if (!temp.empty()) r.totalBiaya = stod(temp);
        getline(ss, r.tanggalPemesanan, ',');
        getline(ss, r.tanggalReservasi, ',');
        getline(ss, r.status, ',');
        getline(ss, r.refundRekening);

        if (r.status.empty()) {
            r.status = "Pending";
        }
        reservations[reservasiCount] = r;
        reservasiCount++;
    }
    file.close();
}
void tulisDataReservasi(const Reservasi daftarReservasi[], int reservasiCount) {
    ofstream file("reservasi.csv");
    for (int i = 0; i < reservasiCount; ++i) {
        file << daftarReservasi[i].reservasiId << "," << daftarReservasi[i].username << "," << daftarReservasi[i].paketId << ","
             << daftarReservasi[i].jumlahOrang << "," << fixed << setprecision(0) << daftarReservasi[i].totalBiaya << ","
             << daftarReservasi[i].tanggalPemesanan << "," << daftarReservasi[i].tanggalReservasi << ","
             << daftarReservasi[i].status << "," << daftarReservasi[i].refundRekening << "\n";
    }
    file.close();
}


//Awal
void tambahPaketBaru(Paket daftarPaket[], int& paketCount) {
    clearScreen();
    printHeader("TAMBAH PAKET BARU");

    if (paketCount >= MAX_DATA) {
        cout << "   [!] Database paket sudah penuh. Tidak bisa menambah paket baru.\n";
        enterToContinue();
        return;
    }

    Paket paketBaru;
    paketBaru.id = generateUniquePaketId(daftarPaket, paketCount);
    paketBaru.nama = getStringInput("   -> Nama Paket Baru: ");
    paketBaru.deskripsi = getStringInput("   -> Deskripsi Paket Baru: ");

    while (true) {
        cout << "   -> Kapasitas Orang: ";
        paketBaru.kapasitas = getIntegerInput();
        if (paketBaru.kapasitas > 0) break;
        cout << "      [!] Kapasitas harus lebih besar dari 0.\n";
    }

    while (true) {
        cout << "   -> Harga Paket (Rp): ";
        paketBaru.harga = getIntegerInput();
        if (paketBaru.harga >= 0) break;
        cout << "      [!] Harga tidak boleh negatif.\n";
    }

    daftarPaket[paketCount] = paketBaru;
    paketCount++;
    tulisDataPaket(daftarPaket, paketCount);

    cout << "\n   [OK] Paket baru berhasil ditambahkan!\n";
    enterToContinue();
}
void hapusPaket(Paket daftarPaket[], int& paketCount) {
    clearScreen();
    printHeader("HAPUS PAKET");
    if (paketCount == 0) {
        cout << "   Tidak ada paket untuk dihapus.\n";
        enterToContinue();
        return;
    }

    cout << "   +----+-------------------------+\n";
    cout << "   | ID | Nama Paket              |\n";
    cout << "   +----+-------------------------+\n";
    for (int i = 0; i < paketCount; ++i) {
        cout << "   | " << left << setw(2) << daftarPaket[i].id << " | "
             << setw(23) << daftarPaket[i].nama << " |\n";
    }
    cout << "   +----+-------------------------+\n\n";


    cout << "   -> Masukkan ID paket yang akan dihapus (0 untuk batal): ";
    int idToDelete = getIntegerInput();

    if (idToDelete == 0) {
        cout << "\n   [*] Penghapusan paket dibatalkan.\n";
        enterToContinue();
        return;
    }

    int indexToDelete = -1;
    for (int i = 0; i < paketCount; ++i) {
        if (daftarPaket[i].id == idToDelete) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete != -1) {
        for (int i = indexToDelete; i < paketCount - 1; ++i) {
            daftarPaket[i] = daftarPaket[i + 1];
        }
        paketCount--;
        tulisDataPaket(daftarPaket, paketCount);
        cout << "\n   [OK] Paket berhasil dihapus.\n";
    } else {
        cout << "\n   [!] ID paket tidak ditemukan.\n";
    }
    enterToContinue();
}
void editPaket(Paket daftarPaket[], int paketCount) {
    clearScreen();
    printHeader("EDIT DATA PAKET");
    if (paketCount == 0) {
        cout << "   Tidak ada paket untuk diedit.\n";
        enterToContinue();
        return;
    }

    cout << "   +----+-------------------------+\n";
    cout << "   | ID | Nama Paket              |\n";
    cout << "   +----+-------------------------+\n";
    for (int i = 0; i < paketCount; ++i) {
        cout << "   | " << left << setw(2) << daftarPaket[i].id << " | "
             << setw(23) << daftarPaket[i].nama << " |\n";
    }
    cout << "   +----+-------------------------+\n\n";

    cout << "   -> Pilih ID paket yang akan di-edit (0 untuk batal): ";
    int id = getIntegerInput();

    if (id == 0) {
        cout << "\n   [*] Edit paket dibatalkan.\n";
        enterToContinue();
        return;
    }

    Paket* paketToEdit = findPaketById(daftarPaket, paketCount, id);
    if (paketToEdit == nullptr) {
        cout << "\n   [!] Paket tidak ditemukan.\n";
        enterToContinue();
        return;
    }

    clearScreen();
    printHeader("EDIT PAKET: " + paketToEdit->nama);
    cout << "   Kosongkan input dan tekan Enter jika tidak ingin mengubah data.\n\n";

    string input;
    cout << "   -> Nama baru [" << paketToEdit->nama << "]: ";
    getline(cin, input);
    if (!input.empty()) paketToEdit->nama = input;
    
    cout << "   -> Deskripsi baru [" << paketToEdit->deskripsi << "]: ";
    getline(cin, input);
    if (!input.empty()) paketToEdit->deskripsi = input;

    cout << "   -> Kapasitas baru [" << paketToEdit->kapasitas << "]: ";
    getline(cin, input);
    if (!input.empty()) {
        try {
            int newCapacity = stoi(input);
            if (newCapacity > 0) {
                paketToEdit->kapasitas = newCapacity;
            } else {
                cout << "      [!] Kapasitas harus > 0, tidak diubah.\n";
            }
        } catch (...) {
            cout << "      [!] Input kapasitas tidak valid, tidak diubah.\n";
        }
    }

    cout << "   -> Harga baru [" << paketToEdit->harga << "]: ";
    getline(cin, input);
    if (!input.empty()) {
        try {
            int newPrice = stoi(input);
            if (newPrice >= 0) {
                paketToEdit->harga = newPrice;
            } else {
                cout << "      [!] Harga tidak boleh negatif, tidak diubah.\n";
            }
        } catch (...) {
            cout << "      [!] Input harga tidak valid, tidak diubah.\n";
        }
    }

    tulisDataPaket(daftarPaket, paketCount);
    cout << "\n   [OK] Data paket berhasil diperbarui.\n";
    enterToContinue();
}
void lihatSemuaReservasi(const Reservasi daftarReservasi[], int reservasiCount, Paket daftarPaket[], int paketCount, User daftarUser[], int userCount) {
    clearScreen();
    printHeader("SEMUA DATA RESERVASI");
    if (reservasiCount == 0) {
        cout << "   Belum ada reservasi.\n";
    } else {
        cout << "+------------+----------------+----------------+-------------------------+-------+---------------+-----------------+-----------------+----------------+" << endl;
        cout << "| ID Res     | User           | No. Telepon    | Paket                   | Orang | Total Biaya   | Tgl Reservasi   | Status          | Rek. Refund    |" << endl;
        cout << "+------------+----------------+----------------+-------------------------+-------+---------------+-----------------+-----------------+----------------+" << endl;

        for (int i = 0; i < reservasiCount; ++i) {
            const auto& r = daftarReservasi[i];
            Paket* p = findPaketById(daftarPaket, paketCount, r.paketId);
            User* u = findUserByUsername(daftarUser, userCount, r.username);
            cout << "| " << left << setw(10) << r.reservasiId
                 << "| " << setw(14) << r.username
                 << "| " << setw(14) << (u ? u->phoneNumber : "N/A")
                 << "| " << setw(23) << (p ? p->nama : "N/A")
                 << "| " << setw(5) << r.jumlahOrang
                 << "| Rp" << setw(11) << fixed << setprecision(0) << r.totalBiaya
                 << "| " << setw(15) << r.tanggalReservasi
                 << "| " << setw(15) << r.status
                 << "| " << setw(14) << (r.refundRekening.empty() ? "-" : r.refundRekening) << " |" << endl;
        }
        cout << "+------------+----------------+----------------+-------------------------+-------+---------------+-----------------+-----------------+----------------+" << endl;
    }
    enterToContinue();
}
void kelolaStatusReservasi(Reservasi daftarReservasi[], int& reservasiCount, Paket daftarPaket[], int paketCount, User daftarUser[], int userCount) {
    clearScreen();
    printHeader("KELOLA STATUS RESERVASI");
    if (reservasiCount == 0) {
        cout << "   Belum ada reservasi untuk dikelola.\n";
        enterToContinue();
        return;
    }

    cout << "+------------+----------------+----------------+-------------------------+-----------------+-----------------+" << endl;
    cout << "| ID Res     | User           | No. Telepon    | Paket                   | Tgl Reservasi   | Status          |" << endl;
    cout << "+------------+----------------+----------------+-------------------------+-----------------+-----------------+" << endl;
    for (int i = 0; i < reservasiCount; ++i) {
        const auto& r = daftarReservasi[i];
        Paket* p = findPaketById(daftarPaket, paketCount, r.paketId);
        User* u = findUserByUsername(daftarUser, userCount, r.username);
        cout << "| " << left << setw(10) << r.reservasiId
             << "| " << setw(14) << r.username
             << "| " << setw(14) << (u ? u->phoneNumber : "N/A")
             << "| " << setw(23) << (p ? p->nama : "N/A")
             << "| " << setw(15) << r.tanggalReservasi
             << "| " << setw(15) << r.status << " |" << endl;
    }
    cout << "+------------+----------------+----------------+-------------------------+-----------------+-----------------+" << endl;


    cout << "\n   -> Masukkan ID Reservasi untuk diubah statusnya (0 untuk batal): ";
    int reservasiId = getIntegerInput();

    if (reservasiId == 0) {
        cout << "\n   [*] Pengelolaan status reservasi dibatalkan.\n";
        enterToContinue();
        return;
    }

    Reservasi* reservasiToUpdate = findReservasiById(daftarReservasi, reservasiCount, reservasiId);

    if (reservasiToUpdate == nullptr) {
        cout << "\n   [!] Reservasi dengan ID " << reservasiId << " tidak ditemukan.\n";
        enterToContinue();
        return;
    }

    cout << "\n   Status saat ini: " << reservasiToUpdate->status << "\n";
    cout << "   Pilih status baru:\n";
    cout << "   [1] Checked-in\n";
    cout << "   [2] Checked-out\n";
    cout << "   [3] Pending\n";
    cout << "   [4] Batal\n";
    cout << "   Pilihan: ";
    int choice = getIntegerInput();

    string newStatus;

    switch (choice) {
        case 1: newStatus = "Checked-in"; break;
        case 2: newStatus = "Checked-out"; break;
        case 3: newStatus = "Pending"; break;
        case 4:
            cout << "\n   [*] Perubahan status dibatalkan.\n";
            enterToContinue();
            return;
        default:
            cout << "\n   [!] Pilihan tidak valid.\n";
            enterToContinue();
            return;
    }

    reservasiToUpdate->status = newStatus;
    tulisDataReservasi(daftarReservasi, reservasiCount);
    cout << "\n   [OK] Status reservasi ID " << reservasiId << " berhasil diperbarui menjadi: " << newStatus << ".\n";
    enterToContinue();
}
void buatReservasiBaru(User& currentUser, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount) {
    clearScreen();
    printHeader("BUAT RESERVASI BARU");
    
    if (reservasiCount >= MAX_DATA) {
        cout << "   [!] Database reservasi penuh. Tidak bisa membuat reservasi baru.\n";
        enterToContinue();
        return;
    }
    if (paketCount == 0) {
        cout << "   [!] Belum ada paket tersedia. Silakan hubungi admin.\n";
        enterToContinue();
        return;
    }

    cout << "   +----+-------------------------+--------------+----------+\n";
    cout << "   | ID | Nama Paket              | Kapasitas    | Harga    |\n";
    cout << "   +----+-------------------------+--------------+----------+\n";
    for (int i = 0; i < paketCount; ++i) {
        cout << "   | " << left << setw(2) << daftarPaket[i].id << " | "
             << setw(23) << daftarPaket[i].nama << " | "
             << setw(7) << daftarPaket[i].kapasitas << "orang | "
             << "Rp" << setw(6) << right << daftarPaket[i].harga << " |\n";
    }
    cout << "   +----+-------------------------+--------------+----------+\n\n";
    cout << "   -> Pilih ID paket: ";
    int id = getIntegerInput();

    Paket* paketPilihan = findPaketById(daftarPaket, paketCount, id);
    if (paketPilihan == nullptr) {
        cout << "\n   [!] Paket tidak ditemukan.\n";
        enterToContinue();
        return;
    }

    clearScreen();
    printHeader("DETAIL PAKET: " + paketPilihan->nama);
    cout << "   Deskripsi: " << paketPilihan->deskripsi << "\n";
    cout << "   Kapasitas: " << paketPilihan->kapasitas << " orang\n\n";

    int jumlahOrang;
    while (true) {
        cout << "   -> Masukkan jumlah orang (1-" << paketPilihan->kapasitas << "): ";
        jumlahOrang = getIntegerInput();
        if (jumlahOrang > 0 && jumlahOrang <= paketPilihan->kapasitas) break;
        cout << "      [!] Jumlah orang tidak valid. Silakan masukkan lagi.\n";
    }

    string tanggalReservasiStr;
    while (true) {
        cout << "   -> Masukkan tanggal reservasi (DD-MM-YYYY): ";
        getline(cin, tanggalReservasiStr);
        if (isValidDate(tanggalReservasiStr)) break;
        cout << "      [!] Format tanggal tidak valid. Gunakan format DD-MM-YYYY (Contoh: 20-06-2025)\n";
    }

    Reservasi newReservasi;
    newReservasi.tanggalPemesanan = getCurrentDate();
    newReservasi.tanggalReservasi = tanggalReservasiStr;
    newReservasi.reservasiId = generateUniqueId(daftarReservasi, reservasiCount, newReservasi.tanggalReservasi);
    newReservasi.username = currentUser.username;
    newReservasi.paketId = paketPilihan->id;
    newReservasi.jumlahOrang = jumlahOrang;
    newReservasi.totalBiaya = paketPilihan->harga + (jumlahOrang * 10000);
    newReservasi.status = "Pending";
    newReservasi.refundRekening = "";

    clearScreen();
    printHeader("KONFIRMASI PEMBAYARAN");
    cout << "   Nama Pelanggan    : " << currentUser.username << "\n";
    cout << "   Nomor Telepon     : " << currentUser.phoneNumber << "\n";
    cout << "   Paket             : " << paketPilihan->nama << "\n";
    cout << "   Jumlah Orang      : " << jumlahOrang << "\n";
    cout << "   Tanggal Reservasi : " << newReservasi.tanggalReservasi << "\n";
    cout << "   -----------------------------------------\n";
    cout << "   Total Biaya       : Rp" << fixed << setprecision(0) << newReservasi.totalBiaya << "\n\n";
    
    cout << "   Silakan transfer ke rekening ADMIN: 1234567890 (Bank XYZ)\n";
    cout << "   Lanjutkan pembayaran? (Y/N): ";
    char konfirmasi;
    cin >> konfirmasi;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(konfirmasi) == 'Y') {
        daftarReservasi[reservasiCount] = newReservasi;
        reservasiCount++;
        tulisDataReservasi(daftarReservasi, reservasiCount);
        
        cout << "\n   [OK] Pembayaran berhasil! Reservasi telah dibuat.\n";
        cetakNota(newReservasi, currentUser, *paketPilihan);
        
        cout << "\n   Cetak nota ke file .txt? (Y/N): ";
        cin >> konfirmasi;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(toupper(konfirmasi) == 'Y'){
            printNotaKeFile(newReservasi, currentUser, *paketPilihan);
            cout << "   [OK] Nota telah disimpan ke file NOTA-" << newReservasi.reservasiId << ".txt\n";
        }

    } else {
        cout << "\n   [*] Reservasi dibatalkan.\n";
    }
    enterToContinue();
}
void kelolaReservasiUser(User& currentUser, Paket daftarPaket[], int paketCount, Reservasi daftarReservasi[], int& reservasiCount, User daftarUser[], int userCount) {
    clearScreen();
    printHeader("RESERVASI SAYA");
    
    bool hasReservasi = false;
    cout << "+------------+-------------------------+-------+-----------------+---------------+-----------------+" << endl;
    cout << "| ID Res     | Paket                   | Orang | Tgl Reservasi   | Total         | Status          |" << endl;
    cout << "+------------+-------------------------+-------+-----------------+---------------+-----------------+" << endl;

    for (int i = 0; i < reservasiCount; ++i) {
        if (daftarReservasi[i].username == currentUser.username) {
            hasReservasi = true;
            Paket* p = findPaketById(daftarPaket, paketCount, daftarReservasi[i].paketId);
            cout << "| " << left << setw(10) << daftarReservasi[i].reservasiId
                 << "| " << setw(23) << (p ? p->nama : "N/A")
                 << "| " << setw(5) << daftarReservasi[i].jumlahOrang
                 << "| " << setw(15) << daftarReservasi[i].tanggalReservasi
                 << "| Rp" << setw(10) << right << fixed << setprecision(0) << daftarReservasi[i].totalBiaya << " "
                 << "| " << left << setw(15) << daftarReservasi[i].status << " |" << endl;
        }
    }

    if (!hasReservasi) {
        cout << "|                 ANDA BELUM MEMILIKI RESERVASI.                                        |" << endl;
        cout << "+------------+-------------------------+-------+-----------------+---------------+-----------------+" << endl;
        enterToContinue();
        return;
    }
    cout << "+------------+-------------------------+-------+-----------------+---------------+-----------------+" << endl;

    cout << "\n   Opsi:\n";
    cout << "   [1] Batalkan Reservasi (Refund)\n";
    cout << "   [2] Print Ulang Nota\n";
    cout << "   [3] Kembali\n";
    cout << "   Pilihan: ";
    int pilihan = getIntegerInput();

    if (pilihan == 1 || pilihan == 2) {
        cout << "   -> Masukkan ID Reservasi yang akan diproses: ";
        int idToProcess = getIntegerInput();
        
        int indexToProcess = -1;
        for(int i = 0; i < reservasiCount; ++i) {
            if (daftarReservasi[i].reservasiId == idToProcess && daftarReservasi[i].username == currentUser.username) {
                indexToProcess = i;
                break;
            }
        }

        if (indexToProcess != -1) {
            if (pilihan == 1) { // BATALKAN (HAPUS)
                if (daftarReservasi[indexToProcess].status == "Pending") {
                    string confirmRekening;
                    while (true) {
                        confirmRekening = getStringInput("   -> Masukkan nomor rekening Anda untuk proses refund: ");
                        if (isValidRekeningNumber(confirmRekening)) {
                            break;
                        }
                        cout << "      [!] Nomor rekening tidak valid. Harus terdiri dari angka.\n";
                    }
                    
                    // Simpan data refund ke reservasi yang akan dihapus untuk catatan (opsional, bisa ditulis ke log terpisah)
                    // Disini, kita langsung hapus saja.
                    
                    for (int i = indexToProcess; i < reservasiCount - 1; ++i) {
                        daftarReservasi[i] = daftarReservasi[i+1];
                    }
                    reservasiCount--;
                    tulisDataReservasi(daftarReservasi, reservasiCount);
                    cout << "\n   [OK] Reservasi berhasil dibatalkan. Proses refund ke rekening " << confirmRekening << " akan segera dilakukan.\n";
                } else {
                    cout << "\n   [!] Reservasi tidak dapat dibatalkan karena statusnya bukan 'Pending'.\n";
                }

            } else if (pilihan == 2) { // PRINT ULANG NOTA
                const Reservasi& r = daftarReservasi[indexToProcess];
                Paket* p = findPaketById(daftarPaket, paketCount, r.paketId);
                if (p != nullptr) {
                    cetakNota(r, currentUser, *p);
                    char konfirmasi;
                    cout << "\n   Cetak nota ke file .txt? (Y/N): ";
                    cin >> konfirmasi;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if(toupper(konfirmasi) == 'Y'){
                        printNotaKeFile(r, currentUser, *p);
                        cout << "   [OK] Nota telah disimpan ke file NOTA-" << r.reservasiId << ".txt\n";
                    }
                } else {
                    cout << "\n   [!] Error: Detail paket untuk reservasi ini tidak ditemukan.\n";
                }
            }
        } else {
            cout << "\n   [!] ID Reservasi tidak valid atau bukan milik Anda.\n";
        }
    }
    if (pilihan != 3) {
        enterToContinue();
    }
}
void editNomorTelepon(User& currentUser, User daftarUser[], int userCount) {
    clearScreen();
    printHeader("EDIT NOMOR TELEPON");
    cout << "   Nomor telepon Anda saat ini: " << currentUser.phoneNumber << "\n\n";

    string newPhoneNumber;
    while (true) {
        newPhoneNumber = getStringInput("   -> Masukkan nomor telepon baru (cth: 08123456789): ");
        if (isValidPhoneNumber(newPhoneNumber)) {
            break;
        }
        cout << "      [!] Nomor telepon tidak valid. Harus dimulai '08' dan 10-13 digit.\n";
    }

    currentUser.phoneNumber = newPhoneNumber;
    
    // Cari user di array utama dan update juga di sana
    User* userInArray = findUserByUsername(daftarUser, userCount, currentUser.username);
    if(userInArray != nullptr) {
        userInArray->phoneNumber = newPhoneNumber;
    }

    tulisDataUser(daftarUser, userCount);

    cout << "\n   [OK] Nomor telepon berhasil diperbarui menjadi: " << currentUser.phoneNumber << "\n";
    enterToContinue();
}
