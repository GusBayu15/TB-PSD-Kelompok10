#include <iostream>
#include "sipus.h"
using namespace std;

bool inputHabis = false;

bool spasi(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}

string bacaBaris(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) {
        inputHabis = true;
        return "";
    }
    int awal = 0, akhir = (int)s.length() - 1;
    while (awal <= akhir && spasi(s[awal])) awal++;
    while (akhir >= awal && spasi(s[akhir])) akhir--;
    return s.substr(awal, akhir - awal + 1);
}

string bacaTeks(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (inputHabis || s != "") return s;
        cout << ">> Masukan tidak boleh kosong.\n";
    }
}

int bacaAngka(const string& prompt, int minimum, int maksimum) {
    while (true) {
        string s = bacaBaris(prompt);
        if (inputHabis) return minimum;
        bool valid = (s != "" && s.length() <= 9);
        for (int i = 0; i < (int)s.length() && valid; i++) {
            if (s[i] < '0' || s[i] > '9') valid = false;
        }
        if (valid) {
            int nilai = 0;
            for (int i = 0; i < (int)s.length(); i++) {
                nilai = nilai * 10 + (s[i] - '0');
            }
            if (nilai >= minimum && nilai <= maksimum) return nilai;
        }
        cout << ">> Masukkan angka " << minimum << " sampai "
             << maksimum << ".\n";
    }
}

void tampilPilihanKategori() {
    for (int i = 0; i < JML_KATEGORI; i++) {
        cout << "   " << i << " = " << namaKategoriBuku(i) << "\n";
    }
}

void isiDataBuku(Buku& b) {
    b.judul = bacaTeks("Judul      : ");
    b.penulis = bacaTeks("Penulis    : ");
    cout << "Kategori:\n";
    tampilPilihanKategori();
    b.kategori = bacaAngka("Kategori   : ", 0, JML_KATEGORI - 1);
    b.tahun = bacaAngka("Tahun      : ", 1900, 2026);
    b.stok = bacaAngka("Stok       : ", 0, 999);
}

void menuTambahBuku(DaftarBuku& buku) {
    Buku b;
    b.kode = bacaTeks("Kode buku  : ");
    if (inputHabis) return;
    if (!kodeBukuValid(b.kode)) {
        cout << ">> Gagal. Format kode harus BK-xxx, contoh BK-001.\n";
        return;
    }
    if (cariKode(buku, b.kode) != nullptr) {
        cout << ">> Gagal. Kode " << b.kode
             << " sudah dipakai buku lain.\n";
        return;
    }
    isiDataBuku(b);
    if (inputHabis) return;
    b.totalDipinjam = 0;
    int kapasitasLama = buku.kapasitas;
    if (tambahBuku(buku, b)) {
        cout << ">> Buku berhasil ditambahkan. Isi array: " << buku.n
             << "/" << buku.kapasitas << "\n";
        if (buku.kapasitas > kapasitasLama) {
            cout << ">> Kapasitas array diperbesar dari " << kapasitasLama
                 << " menjadi " << buku.kapasitas << "\n";
        }
    } else {
        cout << ">> Gagal menambahkan buku.\n";
    }
}

void menuUbahBuku(DaftarBuku& buku) {
    string kode = bacaTeks("Kode buku yang diubah : ");
    if (inputHabis) return;
    Buku* p = cariKode(buku, kode);
    if (p == nullptr) {
        cout << ">> Buku dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }
    cout << "Data lama: " << p->judul << " | " << p->penulis
         << " | stok " << p->stok << "\n";
    Buku baru;
    isiDataBuku(baru);
    if (inputHabis) return;
    if (ubahBuku(buku, kode, baru)) {
        cout << ">> Data buku berhasil diubah.\n";
    } else {
        cout << ">> Gagal mengubah data buku.\n";
    }
}

void menuHapusBuku(DaftarBuku& buku, const DaftarTransaksi& transaksi) {
    string kode = bacaTeks("Kode buku yang dihapus : ");
    if (inputHabis) return;
    if (cariKode(buku, kode) == nullptr) {
        cout << ">> Buku dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }
    if (masihDipinjam(transaksi, kode)) {
        cout << ">> Gagal. Buku masih dipinjam, tidak boleh dihapus.\n";
        return;
    }
    if (hapusBuku(buku, kode)) {
        cout << ">> Buku " << kode << " berhasil dihapus.\n";
    }
}

void menuBuku(DaftarBuku& buku, const DaftarTransaksi& transaksi) {
    int pilih;
    do {
        cout << "\n--- Kelola Buku ---\n"
             << "1. Tambah Buku\n2. Ubah Buku\n3. Hapus Buku\n"
             << "4. Tampilkan Buku\n0. Kembali\n";
        pilih = bacaAngka("Pilihan: ", 0, 4);
        if (inputHabis) return;
        switch (pilih) {
            case 1: menuTambahBuku(buku); break;
            case 2: menuUbahBuku(buku); break;
            case 3: menuHapusBuku(buku, transaksi); break;
            case 4: tampilBuku(buku); break;
        }
    } while (pilih != 0);
}

void menuTambahAnggota(DaftarAnggota& anggota) {
    Anggota a;
    a.id = bacaTeks("ID anggota : ");
    if (inputHabis) return;
    if (!idAnggotaValid(a.id)) {
        cout << ">> Gagal. Format ID harus AG-xxx, contoh AG-001.\n";
        return;
    }
    if (cariAnggota(anggota, a.id) != nullptr) {
        cout << ">> Gagal. ID " << a.id << " sudah terdaftar.\n";
        return;
    }
    a.nama = bacaTeks("Nama       : ");
    a.prodi = bacaTeks("Prodi      : ");
    if (inputHabis) return;
    a.sedangPinjam = 0;
    int kapasitasLama = anggota.kapasitas;
    if (tambahAnggota(anggota, a)) {
        cout << ">> Anggota berhasil ditambahkan. Isi array: "
             << anggota.n
             << "/" << anggota.kapasitas << "\n";
        if (anggota.kapasitas > kapasitasLama) {
            cout << ">> Kapasitas array diperbesar dari " << kapasitasLama
                 << " menjadi " << anggota.kapasitas << "\n";
        }
    } else {
        cout << ">> Gagal menambahkan anggota.\n";
    }
}

void menuUbahAnggota(DaftarAnggota& anggota) {
    string id = bacaTeks("ID anggota yang diubah : ");
    if (inputHabis) return;
    Anggota* p = cariAnggota(anggota, id);
    if (p == nullptr) {
        cout << ">> Anggota dengan ID " << id << " tidak ditemukan.\n";
        return;
    }
    cout << "Data lama: " << p->nama << " | " << p->prodi << "\n";
    Anggota baru;
    baru.nama = bacaTeks("Nama baru  : ");
    baru.prodi = bacaTeks("Prodi baru : ");
    if (inputHabis) return;
    if (ubahAnggota(anggota, id, baru)) {
        cout << ">> Data anggota berhasil diubah.\n";
    }
}

void menuAnggota(DaftarAnggota& anggota) {
    int pilih;
    do {
        cout << "\n--- Kelola Anggota ---\n"
             << "1. Tambah Anggota\n2. Ubah Anggota\n"
             << "3. Tampilkan Anggota\n0. Kembali\n";
        pilih = bacaAngka("Pilihan: ", 0, 3);
        if (inputHabis) return;
        switch (pilih) {
            case 1: menuTambahAnggota(anggota); break;
            case 2: menuUbahAnggota(anggota); break;
            case 3: tampilAnggota(anggota); break;
        }
    } while (pilih != 0);
}

int main() {
    DaftarBuku buku;
    DaftarAnggota anggota;
    DaftarTransaksi transaksi;
    inisialisasi(buku, 4);        
    inisialisasi(anggota, 4);     
    inisialisasi(transaksi, 4);

    int pilihan;
    do {
        cout << "\n===== SiPus: Perpustakaan Mini =====\n"
             << "1. Kelola Buku       5. Kembalikan Buku\n"
             << "2. Kelola Anggota    6. Cari Buku\n"
             << "3. Daftar Buku       7. Laporan Bulanan\n"
             << "4. Pinjam Buku       0. Keluar\n";
        pilihan = bacaAngka("Pilihan: ", 0, 7);
        if (inputHabis) break;
        switch (pilihan) {
            case 1: menuBuku(buku, transaksi); break;
            case 2: menuAnggota(anggota); break;
            case 3: tampilBuku(buku); break;
            case 4: case 5: case 6: case 7:
                cout << ">> Menu ini sedang dikerjakan "
                     << "(Milestone 3 dan 4).\n";
                break;
        }
    } while (pilihan != 0 && !inputHabis);

    bebaskan(buku);              
    bebaskan(anggota);
    bebaskan(transaksi);
    cout << "Terima kasih telah menggunakan SiPus.\n";
    return 0;
}
