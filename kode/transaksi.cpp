#include "sipus.h"
using namespace std;

void inisialisasi(DaftarTransaksi& t, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    t.data = new Transaksi[kapasitasAwal];
    t.n = 0;
    t.kapasitas = kapasitasAwal;
}

void bebaskan(DaftarTransaksi& t) {
    delete[] t.data;
    t.data = nullptr;
    t.n = 0;
    t.kapasitas = 0;
}

bool masihDipinjam(const DaftarTransaksi& t, const string& kodeBk) {
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].kodeBuku == kodeBk && !t.data[i].selesai) {
            return true;
        }
    }
    return false;
}
