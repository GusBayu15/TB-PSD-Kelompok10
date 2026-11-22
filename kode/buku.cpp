#include <iostream>
#include "sipus.h"
using namespace std;

void inisialisasi(DaftarBuku& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new Buku[kapasitasAwal];
    d.n = 0;
    d.kapasitas = kapasitasAwal;
}

void bebaskan(DaftarBuku& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}


void perbesar(DaftarBuku& d) {
    int kapasitasBaru = d.kapasitas * 2;
    Buku* baru = new Buku[kapasitasBaru];
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];      
    }
    delete[] d.data;               
    d.data = baru;
    d.kapasitas = kapasitasBaru;
}

bool kodeBukuValid(const string& kode) {
    if (kode.length() != 6) return false;
    if (kode.substr(0, 3) != "BK-") return false;
    for (int i = 3; i < 6; i++) {
        if (kode[i] < '0' || kode[i] > '9') return false;
    }
    return true;
}

Buku* cariKode(DaftarBuku& d, const string& kode) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].kode == kode) return &d.data[i];
    }
    return nullptr;
}

bool tambahBuku(DaftarBuku& d, const Buku& b) {
    if (!kodeBukuValid(b.kode)) return false;
    if (cariKode(d, b.kode) != nullptr) return false;   
    if (b.kategori < 0 || b.kategori >= JML_KATEGORI) return false;
    if (b.stok < 0) return false;
    if (d.n == d.kapasitas) perbesar(d);
    d.data[d.n] = b;
    d.n++;
    return true;
}

bool ubahBuku(DaftarBuku& d, const string& kode, const Buku& baru) {
    Buku* p = cariKode(d, kode);
    if (p == nullptr) return false;
    if (baru.kategori < 0 || baru.kategori >= JML_KATEGORI) return false;
    if (baru.stok < 0) return false;
    p->judul = baru.judul;
    p->penulis = baru.penulis;
    p->kategori = baru.kategori;
    p->tahun = baru.tahun;
    p->stok = baru.stok;
    return true;
}

bool hapusBuku(DaftarBuku& d, const string& kode) {
    int posisi = -1;
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].kode == kode) { posisi = i; break; }
    }
    if (posisi == -1) return false;
    for (int i = posisi; i < d.n - 1; i++) {
        d.data[i] = d.data[i + 1];
    }
    d.n--;
    return true;
}

string namaKategoriBuku(int kategori) {
    const string nama[JML_KATEGORI] = {"Pemrograman", "Basis Data",
                                       "Jaringan", "Matematika", "Umum"};
    if (kategori < 0 || kategori >= JML_KATEGORI) return "-";
    return nama[kategori];
}

static string kolom(const string& s, int lebar) {
    if ((int)s.length() >= lebar) return s.substr(0, lebar - 1) + " ";
    return s + string(lebar - s.length(), ' ');
}

void tampilBuku(const DaftarBuku& d) {
    if (d.n == 0) {
        cout << ">> Belum ada data buku.\n";
        return;
    }
    string garis(80, '-');
    cout << garis << "\n"
         << kolom("Kode", 8) << kolom("Judul", 26) << kolom("Penulis", 15)
         << kolom("Kategori", 12) << kolom("Tahun", 6) << kolom("Stok", 5)
         << "Dipinjam" << "\n" << garis << "\n";
    for (int i = 0; i < d.n; i++) {
        const Buku& b = d.data[i];
        cout << kolom(b.kode, 8) << kolom(b.judul, 26)
             << kolom(b.penulis, 15)
             << kolom(namaKategoriBuku(b.kategori), 12)
             << kolom(to_string(b.tahun), 6)
             << kolom(to_string(b.stok), 5)
             << b.totalDipinjam << "\n";
    }
    cout << garis << "\n"
         << "Jumlah buku: " << d.n
         << " | Kapasitas array: " << d.kapasitas << "\n";
}
