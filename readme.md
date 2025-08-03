# 🚚 Sistem Informasi Transportasi & Pengiriman Nasional

📦 **Aplikasi simulasi pengiriman barang antar kota di Indonesia** berbasis **GUI** menggunakan **C++ & SFML v2.6.0**. Sistem ini mensimulasikan alur pengiriman barang dari kota asal ke kota tujuan, dengan berbagai fitur interaktif, validasi data, dan perhitungan biaya otomatis berdasarkan algoritma graf.

---

## ✨ Fitur Unggulan

✅ **Form GUI Interaktif**  
Masukkan data pengirim, barang, berat, kota asal-tujuan, dan pilih kendaraan dengan navigasi GUI yang nyaman.

✅ **Perhitungan Otomatis**  
Menggunakan algoritma **Dijkstra** untuk menghitung rute pengiriman terpendek dan menentukan biaya berdasar jarak & kapasitas kendaraan.

✅ **Validasi Data Real-time**  
Nama kota dicek dengan **searching** dan **sorting** sebelum dikirim. Berat barang divalidasi sesuai batas maksimal kendaraan.

✅ **Pilihan Armada Logistik**:
| Kendaraan | Kapasitas |
|----------|-----------|
| 🚗 APV        | 500 kg     |
| 🚚 Pickup     | 1.000 kg   |
| 🚛 Box        | 2.000 kg   |
| 🏗️ Truck      | 5.000 kg   |

✅ **Struktur Data & Algoritma**:
- 🧱 **Stack**: Riwayat pengiriman  
- ⏳ **Queue**: Daftar antrean pesanan  
- 🧭 **Graph & Dijkstra**: Navigasi kota  
- 🧠 **Pointer & Struct**: Representasi data dinamis  
- 🔎 **Searching & Sorting**: Validasi & pencarian kota

---

## 🗺️ Kota yang Didukung

`Jakarta`, `Bandung`, `Surabaya`, `Medan`, `Makassar`, `Semarang`, `Palembang`, `Tangerang`, `Depok`, `Bekasi`

---

## ⚙️ Cara Menjalankan

1. **Instal SFML v2.6.0**
   - 📥 [Unduh SFML](https://www.sfml-dev.org/download.php)
   - Pastikan `lib`, `include`, dan `.dll` SFML telah disambungkan

2. **Kompilasi Program**:
   ```bash
   g++ main.cpp -o transportasi -lsfml-graphics -lsfml-window -lsfml-system
