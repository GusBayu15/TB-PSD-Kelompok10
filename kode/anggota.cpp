#include <iostream>
#include "sipus.h"
using namespace std;

void inisialisasi(DaftarAnggota& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new Anggota[kapasitasAwal];
    d.n = 0;
    d.kapasitas = kapasitasAwal;
}

void bebaskan(DaftarAnggota& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

void perbesar(DaftarAnggota& d) {
    int kapasitasBaru = d.kapasitas * 2;
    Anggota* baru = new Anggota[kapasitasBaru];
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
}

bool idAnggotaValid(const string& id) {
    if (id.length() != 6) return false;
    if (id.substr(0, 3) != "AG-") return false;
    for (int i = 3; i < 6; i++) {
        if (id[i] < '0' || id[i] > '9') return false;
    }
    return true;
}

Anggota* cariAnggota(DaftarAnggota& d, const string& id) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].id == id) return &d.data[i];
    }
    return nullptr;
}

bool tambahAnggota(DaftarAnggota& d, const Anggota& a) {
    if (!idAnggotaValid(a.id)) return false;
    if (cariAnggota(d, a.id) != nullptr) return false;   
    if (d.n == d.kapasitas) perbesar(d);
    d.data[d.n] = a;
    d.data[d.n].sedangPinjam = 0;
    d.n++;
    return true;
}

bool ubahAnggota(DaftarAnggota& d, const string& id,
                 const Anggota& baru) {
    Anggota* p = cariAnggota(d, id);
    if (p == nullptr) return false;
    p->nama = baru.nama;
    p->prodi = baru.prodi;
    return true;
}

static string kolom(const string& s, int lebar) {
    if ((int)s.length() >= lebar) return s.substr(0, lebar - 1) + " ";
    return s + string(lebar - s.length(), ' ');
}

void tampilAnggota(const DaftarAnggota& d) {
    if (d.n == 0) {
        cout << ">> Belum ada data anggota.\n";
        return;
    }
    string garis(61, '-');
    cout << garis << "\n"
         << kolom("ID", 8) << kolom("Nama", 24) << kolom("Prodi", 21)
         << "Dipinjam" << "\n" << garis << "\n";
    for (int i = 0; i < d.n; i++) {
        const Anggota& a = d.data[i];
        cout << kolom(a.id, 8) << kolom(a.nama, 24) << kolom(a.prodi, 21)
             << a.sedangPinjam << "\n";
    }
    cout << garis << "\n"
         << "Jumlah anggota: " << d.n
         << " | Kapasitas array: " << d.kapasitas << "\n";
}
