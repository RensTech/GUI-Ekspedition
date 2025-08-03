#include <SFML/Graphics.hpp> //membuat aplikasi GUI grafis berbasis SFML
#include <iostream> // Untuk input/output standar
#include <string> // menggunakan tipe data
#include <vector> // Menggunakan struktur data vektor (array dinamis)
#include <map> // Struktur data map (key-value pair)
#include <queue> // antrian 
#include <stack> // untuk tumpukan
#include <algorithm> // algoritma standar seperti sorting, searching, dll
#include <sstream> // Memanipulasi string sebagai stream 
#include <iomanip> // Untuk formatting output, khususnya angka dan teks
#include <cmath> // fungsi matematika
#include <limits> //Untuk mendapatkan nilai batas maksimum/minimum
#include <ctime> //Untuk fungsi waktu dan tanggal

// ==================================================
// DEKLARASI STRUKTUR DATA DAN CLASS
// ==================================================

// 1. POINTER - Untuk manajemen memori dinamis dan referensi objek
struct Kota; // Deklarasi awal untuk penggunaan pointer

// 2. STACK - Implementasi stack untuk menyimpan riwayat transaksi
template<typename T>
class Tumpukan {
private:
    std::vector<T> data; // Menyimpan data dalam vector
public:
    void dorong(const T& item) { data.push_back(item); } // Menambahkan item ke atas stack
    void keluarkan() { if (!data.empty()) data.pop_back(); } // Mengeluarkan item dari atas stack
    T& atas() { return data.back(); } // Mengakses item teratas
    bool kosong() const { return data.empty(); } // Memeriksa apakah stack kosong
    size_t ukuran() const { return data.size(); } // Mendapatkan jumlah item dalam stack
};

// 3. QUEUE - Implementasi antrian untuk memproses pesanan pengiriman
template<typename T>
class Antrian {
private:
    std::vector<T> data; // Menyimpan data dalam vector
    size_t indeks_depan; // Indeks untuk elemen depan antrian
public:
    Antrian() : indeks_depan(0) {} // Konstruktor, inisialisasi indeks depan
    void masukkan(const T& item) { data.push_back(item); } // Menambahkan item ke belakang antrian
    void keluarkan() { if (!kosong()) indeks_depan++; } // Mengeluarkan item dari depan antrian
    T& depan() { return data[indeks_depan]; } // Mengakses item di depan antrian
    bool kosong() const { return indeks_depan >= data.size(); } // Memeriksa apakah antrian kosong
    size_t ukuran() const { return data.size() - indeks_depan; } // Mendapatkan jumlah item dalam antrian
};

// Struktur Kendaraan untuk opsi transportasi
struct Kendaraan {
    std::string jenis; // Jenis kendaraan (APV, Pickup, Box, Truck)
    float biaya_per_km; // Biaya transportasi per kilometer
    float berat_maksimal; // Berat maksimal yang dapat diangkut (kg)
    
    Kendaraan(const std::string& j, float biaya, float berat) 
        : jenis(j), biaya_per_km(biaya), berat_maksimal(berat) {}
};

// Struktur PesananPengiriman
struct PesananPengiriman {
    std::string nama_pengirim;
    std::string barang;
    float berat;
    std::string jenis_kendaraan;
    std::string kota_asal;
    std::string kota_tujuan;
    float total_biaya;
    std::string waktu_pesan;
    
    PesananPengiriman() : berat(0.0f), total_biaya(0.0f) {}
};

// 7. GRAF - Struktur graf untuk koneksi antar kota dan pencarian rute
struct Kota {
    std::string nama;
    std::map<Kota*, float> koneksi; // Daftar adjacency dengan jarak
    
    Kota(const std::string& nama_kota) : nama(nama_kota) {}
};

// ==================================================
// KELAS UTAMA SISTEM TRANSPORTASI
// ==================================================

class SistemTransportasi {
private:
    // Implementasi struktur data
    std::vector<Kendaraan> daftar_kendaraan; // Array dinamis untuk kendaraan
    std::map<std::string, Kota*> daftar_kota; // Map untuk pencarian cepat kota
    Tumpukan<PesananPengiriman> riwayat_pesanan; // Stack untuk riwayat pesanan
    Antrian<PesananPengiriman> pesanan_pending; // Queue untuk pesanan yang menunggu
    
    // Komponen GUI
    sf::RenderWindow jendela;
    sf::Font font;
    std::vector<sf::Text> label_input;
    std::vector<std::string> nilai_input;
    std::vector<sf::RectangleShape> kotak_input;
    std::vector<sf::RectangleShape> tombol;
    std::vector<sf::Text> teks_tombol;
    sf::Text teks_hasil;
    sf::Text teks_judul;
    sf::RectangleShape latar_belakang;
    
    // Variabel keadaan
    int input_aktif;
    int kendaraan_terpilih;
    bool tampilkan_hasil;
    std::string pesan_hasil;
    bool jendela_fokus;
    float waktu_animasi;
    
    // Warna untuk desain
    sf::Color warna_utama;
    sf::Color warna_sekunder;
    sf::Color warna_aksen;
    
public:
    SistemTransportasi() : 
        jendela(sf::VideoMode(1000, 700), "Sistem Informasi Transportasi Nasional"),
        input_aktif(-1), kendaraan_terpilih(-1), tampilkan_hasil(false), 
        jendela_fokus(true), waktu_animasi(0.0f) {
        
        // Inisialisasi warna
        warna_utama = sf::Color(70, 130, 180);    // Steel blue
        warna_sekunder = sf::Color(176, 196, 222); // Light steel blue
        warna_aksen = sf::Color(255, 215, 0);     // Gold
        
        inisialisasiData();
        inisialisasiGUI();
        jendela.setKeyRepeatEnabled(true); // Aktifkan pengulangan tombol untuk input teks
    }
    
    // ==================================================
    // FUNGSI INISIALISASI
    // ==================================================
    
    // Inisialisasi data transportasi
    void inisialisasiData() {
        // Inisialisasi jenis kendaraan dengan biaya per km dan berat maksimal
        daftar_kendaraan.push_back(Kendaraan("APV", 2.5f, 500.0f));
        daftar_kendaraan.push_back(Kendaraan("Pickup", 3.0f, 1000.0f));
        daftar_kendaraan.push_back(Kendaraan("Box", 4.5f, 2000.0f));
        daftar_kendaraan.push_back(Kendaraan("Truck", 6.0f, 5000.0f));
        
        // 7. GRAF - Inisialisasi jaringan kota dengan jarak
        inisialisasiJaringanKota();
    }
    
    // Inisialisasi jaringan kota menggunakan struktur graf
    void inisialisasiJaringanKota() {
        // Daftar kota besar di Indonesia
        std::vector<std::string> nama_kota = {
            "Jakarta", "Surabaya", "Bandung", "Medan", "Semarang",
            "Makassar", "Palembang", "Tangerang", "Depok", "Bekasi"
        };
        
        // Buat node kota
        for (const auto& nama : nama_kota) {
            daftar_kota[nama] = new Kota(nama);
        }
        
        // Tambahkan koneksi dengan jarak (jaringan disederhanakan)
        tambahkanKoneksiKota("Jakarta", "Bandung", 150.0f);
        tambahkanKoneksiKota("Jakarta", "Semarang", 450.0f);
        tambahkanKoneksiKota("Jakarta", "Surabaya", 800.0f);
        tambahkanKoneksiKota("Bandung", "Semarang", 350.0f);
        tambahkanKoneksiKota("Semarang", "Surabaya", 350.0f);
        tambahkanKoneksiKota("Jakarta", "Tangerang", 30.0f);
        tambahkanKoneksiKota("Jakarta", "Depok", 25.0f);
        tambahkanKoneksiKota("Jakarta", "Bekasi", 35.0f);
        tambahkanKoneksiKota("Surabaya", "Makassar", 1200.0f);
        tambahkanKoneksiKota("Jakarta", "Palembang", 550.0f);
        tambahkanKoneksiKota("Medan", "Palembang", 650.0f);
    }
    
    // Tambahkan koneksi dua arah antara kota
    void tambahkanKoneksiKota(const std::string& kota1, const std::string& kota2, float jarak) {
        if (daftar_kota.find(kota1) != daftar_kota.end() && daftar_kota.find(kota2) != daftar_kota.end()) {
            daftar_kota[kota1]->koneksi[daftar_kota[kota2]] = jarak;
            daftar_kota[kota2]->koneksi[daftar_kota[kota1]] = jarak;
        }
    }
    
    // ==================================================
    // FUNGSI GUI DAN TAMPILAN
    // ==================================================
    
    void inisialisasiGUI() {
        // Muat font - coba dari beberapa sumber
        if (!font.loadFromFile("arial.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                    std::cerr << "Peringatan: Tidak dapat memuat file font. Menggunakan font default." << std::endl;
                    font = sf::Font(); // Font dasar
                }
            }
        }
        
        // Inisialisasi latar belakang dengan gradien
        latar_belakang.setSize(sf::Vector2f(1000, 700));
        latar_belakang.setFillColor(warna_sekunder);
        
        // Inisialisasi judul
        teks_judul.setFont(font);
        teks_judul.setString("Sistem Informasi Transportasi/Pengiriman Nasional");
        teks_judul.setCharacterSize(24);
        teks_judul.setFillColor(warna_utama);
        teks_judul.setStyle(sf::Text::Bold);
        teks_judul.setPosition(150, 20);
        
        // Inisialisasi bidang input
        std::vector<std::string> label = {
            "Nama Pengirim:", "Barang:", "Berat Barang (kg):", 
            "Kota Asal:", "Kota Tujuan:"
        };
        
        nilai_input.resize(label.size(), "");
        label_input.resize(label.size());
        kotak_input.resize(label.size());
        
        for (size_t i = 0; i < label.size(); ++i) {
           
            label_input[i].setFont(font);
            label_input[i].setString(label[i]);
            label_input[i].setCharacterSize(16);
            label_input[i].setFillColor(sf::Color::Black);
            label_input[i].setPosition(50, 80 + i * 60);
            
            
            kotak_input[i].setSize(sf::Vector2f(300, 30));
            kotak_input[i].setPosition(250, 80 + i * 60);
            kotak_input[i].setFillColor(sf::Color::White);
            kotak_input[i].setOutlineThickness(1);
            kotak_input[i].setOutlineColor(sf::Color(100, 100, 100));
        }
        
        
        for (size_t i = 0; i < daftar_kendaraan.size(); ++i) {
            sf::RectangleShape tombol_kendaraan;
            tombol_kendaraan.setSize(sf::Vector2f(100, 40));
            tombol_kendaraan.setPosition(600 + (i % 2) * 120, 100 + (i / 2) * 50);
            tombol_kendaraan.setFillColor(sf::Color(200, 200, 200)); // Warna default
            tombol_kendaraan.setOutlineThickness(1);
            tombol_kendaraan.setOutlineColor(sf::Color(100, 100, 100));
            tombol.push_back(tombol_kendaraan);
            
            sf::Text teks_kendaraan;
            teks_kendaraan.setFont(font);
            std::string info_kendaraan = daftar_kendaraan[i].jenis + "\n(" + 
                                       std::to_string(static_cast<int>(daftar_kendaraan[i].berat_maksimal)) + "kg)";
            teks_kendaraan.setString(info_kendaraan);
            teks_kendaraan.setCharacterSize(12);
            teks_kendaraan.setFillColor(sf::Color::Black);
            teks_kendaraan.setPosition(605 + (i % 2) * 120, 105 + (i / 2) * 50);
            teks_tombol.push_back(teks_kendaraan);
        }
        
        // Tombol Hitung Biaya
        sf::RectangleShape tombol_hitung;
        tombol_hitung.setSize(sf::Vector2f(150, 50));
        tombol_hitung.setPosition(400, 350);
        tombol_hitung.setFillColor(warna_utama);
        tombol_hitung.setOutlineThickness(1);
        tombol_hitung.setOutlineColor(warna_aksen);
        tombol.push_back(tombol_hitung);
        
        sf::Text teks_hitung;
        teks_hitung.setFont(font);
        teks_hitung.setString("Submit");
        teks_hitung.setCharacterSize(16);
        teks_hitung.setFillColor(sf::Color::White);
        teks_hitung.setPosition(420, 365);
        teks_tombol.push_back(teks_hitung);
        
        // Tombol Bersihkan
        sf::RectangleShape tombol_bersihkan;
        tombol_bersihkan.setSize(sf::Vector2f(100, 40));
        tombol_bersihkan.setPosition(580, 350);
        tombol_bersihkan.setFillColor(sf::Color(220, 80, 80)); // Merah muda
        tombol_bersihkan.setOutlineThickness(1);
        tombol_bersihkan.setOutlineColor(sf::Color(180, 50, 50)); // Merah tua
        tombol.push_back(tombol_bersihkan);
        
        sf::Text teks_bersihkan;
        teks_bersihkan.setFont(font);
        teks_bersihkan.setString("Bersihkan");
        teks_bersihkan.setCharacterSize(14);
        teks_bersihkan.setFillColor(sf::Color::White);
        teks_bersihkan.setPosition(600, 360);
        teks_tombol.push_back(teks_bersihkan);
        
        // Tampilan hasil
        teks_hasil.setFont(font);
        teks_hasil.setCharacterSize(12);
        teks_hasil.setFillColor(sf::Color(0, 0, 139)); // Biru tua
        teks_hasil.setPosition(50, 420);
    }
    
    // ==================================================
    // FUNGSI ALGORITMA UTAMA
    // ==================================================
    
    // 6. PENCARIAN - Binary search untuk mencari kota
    bool cariKota(const std::string& nama_kota) {
        std::vector<std::string> kota_terurut;
        for (const auto& pasangan : daftar_kota) {
            kota_terurut.push_back(pasangan.first);
        }
        
        // 4. PENGURUTAN - Mengurutkan kota untuk binary search
        std::sort(kota_terurut.begin(), kota_terurut.end());
        
        // Implementasi binary search
        int kiri = 0;
        int kanan = static_cast<int>(kota_terurut.size()) - 1;
        while (kiri <= kanan) {
            int tengah = kiri + (kanan - kiri) / 2;
            if (kota_terurut[tengah] == nama_kota) return true;
            if (kota_terurut[tengah] < nama_kota) kiri = tengah + 1;
            else kanan = tengah - 1;
        }
        return false;
    }
    
    // 6. PENCARIAN - Linear search untuk kendaraan berdasarkan jenis
    Kendaraan* cariKendaraan(const std::string& jenis_kendaraan) {
        for (auto& kendaraan : daftar_kendaraan) {
            if (kendaraan.jenis == jenis_kendaraan) {
                return &kendaraan;
            }
        }
        return nullptr;
    }
    
    // 7. GRAF - Algoritma Dijkstra untuk rute terpendek
    float cariJarakTerpendek(const std::string& dari, const std::string& ke) {
        if (daftar_kota.find(dari) == daftar_kota.end() || daftar_kota.find(ke) == daftar_kota.end()) {
            return -1.0f; // Kota tidak ditemukan
        }
        
        std::map<Kota*, float> jarak;
        std::map<Kota*, bool> dikunjungi;
        
        // Inisialisasi jarak
        for (const auto& pasangan : daftar_kota) {
            jarak[pasangan.second] = std::numeric_limits<float>::infinity();
            dikunjungi[pasangan.second] = false;
        }
        
        jarak[daftar_kota[dari]] = 0.0f;
        
        // Algoritma Dijkstra
        for (size_t i = 0; i < daftar_kota.size(); ++i) {
            Kota* saat_ini = nullptr;
            float jarak_min = std::numeric_limits<float>::infinity();
            
            // Temukan vertex yang belum dikunjungi dengan jarak minimum
            for (const auto& pasangan : daftar_kota) {
                if (!dikunjungi[pasangan.second] && jarak[pasangan.second] < jarak_min) {
                    jarak_min = jarak[pasangan.second];
                    saat_ini = pasangan.second;
                }
            }
            
            if (saat_ini == nullptr) break;
            dikunjungi[saat_ini] = true;
            
            // Perbarui jarak ke tetangga
            for (const auto& koneksi : saat_ini->koneksi) {
                Kota* tetangga = koneksi.first;
                float bobot = koneksi.second;
                
                if (!dikunjungi[tetangga]) {
                    float jarak_baru = jarak[saat_ini] + bobot;
                    if (jarak_baru < jarak[tetangga]) {
                        jarak[tetangga] = jarak_baru;
                    }
                }
            }
        }
        
        return jarak[daftar_kota[ke]];
    }
    
    // Hitung biaya pengiriman berdasarkan jarak, berat, dan jenis kendaraan
    float hitungBiayaPengiriman(const std::string& asal, const std::string& tujuan, 
                               float berat, const std::string& jenis_kendaraan) {
        float jarak = cariJarakTerpendek(asal, tujuan);
        if (jarak < 0) return -1.0f; // Rute tidak valid
        
        Kendaraan* kendaraan = cariKendaraan(jenis_kendaraan);
        if (!kendaraan) return -1.0f; // Kendaraan tidak valid
        
        if (berat > kendaraan->berat_maksimal) return -2.0f; // Kelebihan berat
        
        // Perhitungan biaya: biaya dasar + biaya jarak + faktor berat
        float biaya_dasar = 50000.0f; // Biaya pengiriman dasar dalam IDR
        float biaya_jarak = jarak * kendaraan->biaya_per_km * 1000.0f; // Konversi ke IDR
        float faktor_berat = berat * 500.0f; // Biaya tambahan berat
        
        return biaya_dasar + biaya_jarak + faktor_berat;
    }
    
    // ==================================================
    // FUNGSI PROSES PESANAN
    // ==================================================
    
    // Proses pesanan pengiriman dan tambahkan ke antrian
    void prosesPesanan() {
        // Validasi pemilihan kendaraan
        if (kendaraan_terpilih < 0 || kendaraan_terpilih >= static_cast<int>(daftar_kendaraan.size())) {
            pesan_hasil = "Error: Silakan pilih jenis kendaraan!";
            tampilkan_hasil = true;
            return;
        }
        
        PesananPengiriman pesanan;
        
        // Validasi input
        if (nilai_input[0].empty() || nilai_input[1].empty() || 
            nilai_input[2].empty() || nilai_input[3].empty() || 
            nilai_input[4].empty()) {
            pesan_hasil = "Error: Harap isi semua kolom!";
            tampilkan_hasil = true;
            return;
        }
        
        // Parse berat
        try {
            pesanan.berat = std::stof(nilai_input[2]);
            if (pesanan.berat <= 0) {
                pesan_hasil = "Error: Berat harus positif!";
                tampilkan_hasil = true;
                return;
            }
        } catch (...) {
            pesan_hasil = "Error: Format berat tidak valid!";
            tampilkan_hasil = true;
            return;
        }
        
        // Validasi kota
        if (!cariKota(nilai_input[3]) || !cariKota(nilai_input[4])) {
            pesan_hasil = "Error: Nama kota tidak valid!\nKota yang tersedia: Jakarta, Surabaya, Bandung, Medan, Semarang, Makassar, Palembang, Tangerang, Depok, Bekasi";
            tampilkan_hasil = true;
            return;
        }
        
        // Isi detail pesanan
        pesanan.nama_pengirim = nilai_input[0];
        pesanan.barang = nilai_input[1];
        pesanan.kota_asal = nilai_input[3];
        pesanan.kota_tujuan = nilai_input[4];
        pesanan.jenis_kendaraan = daftar_kendaraan[kendaraan_terpilih].jenis;
        
        // Hitung biaya
        pesanan.total_biaya = hitungBiayaPengiriman(pesanan.kota_asal, pesanan.kota_tujuan, 
                                               pesanan.berat, pesanan.jenis_kendaraan);
        
        if (pesanan.total_biaya == -1.0f) {
            pesan_hasil = "Error: Tidak ada rute yang tersedia antara kota-kota tersebut!";
            tampilkan_hasil = true;
            return;
        } else if (pesanan.total_biaya == -2.0f) {
            pesan_hasil = "Error: Barang terlalu berat untuk kendaraan yang dipilih!\nBerat maksimal untuk " + 
                           pesanan.jenis_kendaraan + ": " + std::to_string(static_cast<int>(daftar_kendaraan[kendaraan_terpilih].berat_maksimal)) + " kg";
            tampilkan_hasil = true;
            return;
        }
        
        // Tambahkan waktu pesan
        std::time_t sekarang = std::time(nullptr);
        pesanan.waktu_pesan = std::ctime(&sekarang);
        
        // Tambahkan ke antrian dan riwayat stack
        pesanan_pending.masukkan(pesanan);
        riwayat_pesanan.dorong(pesanan);
        
        // Tampilkan hasil
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0);
        ss << "=== HASIL PERHITUNGAN PENGIRIMAN ===\n";
        ss << "Pengirim: " << pesanan.nama_pengirim << "\n";
        ss << "Barang: " << pesanan.barang << " (" << pesanan.berat << " kg)\n";
        ss << "Rute: " << pesanan.kota_asal << " -> " << pesanan.kota_tujuan << "\n";
        ss << "Kendaraan: " << pesanan.jenis_kendaraan << "\n";
        ss << "Jarak: " << cariJarakTerpendek(pesanan.kota_asal, pesanan.kota_tujuan) << " km\n";
        ss << "Total Biaya: Rp " << static_cast<int>(pesanan.total_biaya) << "\n";
        ss << "Waktu Pesan: " << pesanan.waktu_pesan;
        ss << "Pesanan dalam Antrian: " << pesanan_pending.ukuran() << "\n";
        ss << "Jumlah Riwayat: " << riwayat_pesanan.ukuran();
        
        pesan_hasil = ss.str();
        tampilkan_hasil = true;
    }
    
    // ==================================================
    // FUNGSI EVENT DAN RENDER
    // ==================================================
    
    void handleEvents() {
        sf::Event event;
        while (jendela.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                jendela.close();
            }
            
            // Handle fokus jendela
            if (event.type == sf::Event::LostFocus) {
                jendela_fokus = false;
            }
            if (event.type == sf::Event::GainedFocus) {
                jendela_fokus = true;
            }
            
            // Handle klik mouse hanya ketika jendela memiliki fokus
            if (jendela_fokus && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i posisi_mouse = sf::Mouse::getPosition(jendela);
                
                // Periksa klik kotak input
                input_aktif = -1;
                for (size_t i = 0; i < kotak_input.size(); ++i) {
                    if (kotak_input[i].getGlobalBounds().contains(static_cast<float>(posisi_mouse.x), static_cast<float>(posisi_mouse.y))) {
                        input_aktif = static_cast<int>(i);
                        break;
                    }
                }
                
                // Periksa klik tombol kendaraan
                for (size_t i = 0; i < daftar_kendaraan.size(); ++i) {
                    if (tombol[i].getGlobalBounds().contains(static_cast<float>(posisi_mouse.x), static_cast<float>(posisi_mouse.y))) {
                        if (kendaraan_terpilih == static_cast<int>(i)) {
                            // Unselect jika sudah terpilih
                            kendaraan_terpilih = -1;
                            tombol[i].setFillColor(sf::Color(200, 200, 200));
                        } else {
                            // Select kendaraan baru
                            if (kendaraan_terpilih >= 0) {
                                tombol[kendaraan_terpilih].setFillColor(sf::Color(200, 200, 200));
                            }
                            kendaraan_terpilih = static_cast<int>(i);
                            tombol[i].setFillColor(warna_aksen);
                        }
                        break;
                    }
                }
                
                // Periksa klik tombol hitung (tombol kedua dari belakang)
                if (tombol[tombol.size()-2].getGlobalBounds().contains(static_cast<float>(posisi_mouse.x), static_cast<float>(posisi_mouse.y))) {
                    prosesPesanan();
                }
                
                // Periksa klik tombol bersihkan (tombol terakhir)
                if (tombol[tombol.size()-1].getGlobalBounds().contains(static_cast<float>(posisi_mouse.x), static_cast<float>(posisi_mouse.y))) {
                    // Bersihkan semua bidang input
                    for (auto& nilai : nilai_input) {
                        nilai.clear();
                    }
                    tampilkan_hasil = false;
                    input_aktif = -1;
                    kendaraan_terpilih = -1;
                    // Reset warna tombol kendaraan
                    for (size_t j = 0; j < daftar_kendaraan.size(); ++j) {
                        tombol[j].setFillColor(sf::Color(200, 200, 200));
                    }
                }
            }
            
            // Handle input teks hanya ketika jendela memiliki fokus dan input aktif
            if (jendela_fokus && input_aktif >= 0 && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!nilai_input[input_aktif].empty()) {
                        nilai_input[input_aktif].pop_back();
                    }
                } 
                else if (event.text.unicode >= 32 && event.text.unicode < 128) { // Karakter ASCII yang bisa dicetak
                    if (nilai_input[input_aktif].length() < 50) {
                        nilai_input[input_aktif] += static_cast<char>(event.text.unicode);
                    }
                }
            }
            
            // Handle tombol Enter untuk pindah ke field berikutnya
            if (jendela_fokus && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (input_aktif >= 0 && input_aktif < static_cast<int>(nilai_input.size()) - 1) {
                    // Pindah ke field input berikutnya
                    input_aktif++;
                }
            }
        }
    }
    
    void update(float deltaTime) {
        waktu_animasi += deltaTime;
        
        // Animasi tombol hitung
        if (tombol.size() > daftar_kendaraan.size()) {
            float scale = 1.0f + 0.05f * sin(waktu_animasi * 3.0f);
            tombol[tombol.size()-2].setScale(scale, scale);
        }
    }
    
    void render() {
        jendela.clear(sf::Color(240, 248, 255)); // Latar biru muda
        
        // Gambar latar belakang
        jendela.draw(latar_belakang);
        
        // Gambar judul
        jendela.draw(teks_judul);
        
        // Gambar bidang input
        for (size_t i = 0; i < label_input.size(); ++i) {
            // Set warna outline berdasarkan status aktif
            if (input_aktif == static_cast<int>(i)) {
                kotak_input[i].setOutlineThickness(2);
                kotak_input[i].setOutlineColor(warna_aksen);
            } else {
                kotak_input[i].setOutlineThickness(1);
                kotak_input[i].setOutlineColor(sf::Color(100, 100, 100));
            }
            
            jendela.draw(kotak_input[i]);
            jendela.draw(label_input[i]);
            
            // Gambar teks input
            sf::Text teks_input;
            teks_input.setFont(font);
            teks_input.setString(nilai_input[i]);
            teks_input.setCharacterSize(14);
            teks_input.setFillColor(sf::Color::Black);
            teks_input.setPosition(255, 85 + i * 60);
            jendela.draw(teks_input);
            
            // Gambar kursor untuk input aktif
            if (input_aktif == static_cast<int>(i)) {
                sf::RectangleShape kursor;
                kursor.setSize(sf::Vector2f(2, 20));
                kursor.setFillColor(sf::Color::Black);
                
                // Hitung posisi kursor berdasarkan lebar teks
                sf::Text teks_sementara;
                teks_sementara.setFont(font);
                teks_sementara.setString(nilai_input[i]);
                teks_sementara.setCharacterSize(14);
                float lebar_teks = teks_sementara.getGlobalBounds().width;
                
                kursor.setPosition(255 + lebar_teks + 2, 85 + i * 60);
                jendela.draw(kursor);
            }
        }
        
        // Gambar label jenis kendaraan dan info pemilihan
        sf::Text label_kendaraan;
        label_kendaraan.setFont(font);
        label_kendaraan.setString("Jenis Kendaraan (Klik untuk Memilih):");
        label_kendaraan.setCharacterSize(16);
        label_kendaraan.setFillColor(sf::Color::Black);
        label_kendaraan.setPosition(600, 70);
        jendela.draw(label_kendaraan);
        
        // Tampilkan info kendaraan terpilih
        sf::Text info_terpilih;
        info_terpilih.setFont(font);
        if (kendaraan_terpilih >= 0) {
            info_terpilih.setString("Terpilih: " + daftar_kendaraan[kendaraan_terpilih].jenis);
            info_terpilih.setFillColor(sf::Color(0, 100, 0)); // Hijau tua
        } else {
            info_terpilih.setString("Belum memilih kendaraan");
            info_terpilih.setFillColor(sf::Color::Red);
        }
        info_terpilih.setCharacterSize(14);
        info_terpilih.setPosition(600, 250);
        jendela.draw(info_terpilih);
        
        // Gambar tombol
        for (size_t i = 0; i < tombol.size(); ++i) {
            jendela.draw(tombol[i]);
            jendela.draw(teks_tombol[i]);
        }
        
        // Gambar hasil
        if (tampilkan_hasil) {
            teks_hasil.setString(pesan_hasil);
            jendela.draw(teks_hasil);
        }
        
        // Tampilkan petunjuk bantuan
        sf::Text petunjuk;
        petunjuk.setFont(font);
        petunjuk.setString("Petunjuk: Klik bidang input untuk mengetik. Gunakan Enter untuk pindah ke field berikutnya.\nKota yang Tersedia: Jakarta, Surabaya, Bandung, Medan, Semarang, Makassar, Palembang, Tangerang, Depok, Bekasi");
        petunjuk.setCharacterSize(11);
        petunjuk.setFillColor(sf::Color(60, 60, 60));
        petunjuk.setPosition(50, 600);
        jendela.draw(petunjuk);
        
        jendela.display();
    }
    
    void run() {
        sf::Clock clock;
        while (jendela.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            handleEvents();
            update(deltaTime);
            render();
        }
    }
    
    // Destruktor untuk membersihkan memori
    ~SistemTransportasi() {
        for (auto& pasangan : daftar_kota) {
            delete pasangan.second; // Bersihkan objek kota yang dialokasikan secara dinamis
        }
    }
};

// ==================================================
// FUNGSI UTAMA
// ==================================================

int main() {
    try {
        SistemTransportasi sistem;
        sistem.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}