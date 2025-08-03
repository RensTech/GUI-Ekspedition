# 🚚 Sistem Informasi Transportasi Nasional - GUI SFML

![GUI Preview](https://via.placeholder.com/800x500/2d3748/ffffff?text=Transportation+System+GUI) *(Preview gambar akan ditambahkan)*

## 🌟 Fitur Utama

```diff
+ 📦 Kalkulator biaya pengiriman otomatis
+ 🗺️ Jaringan 10 kota besar di Indonesia
+ 🚛 4 jenis kendaraan dengan kapasitas berbeda
+ 📊 Riwayat transaksi dan antrian pengiriman
+ 🛣️ Algoritma Dijkstra untuk rute terpendek

🛠️ Teknologi
<div align="center">
Komponen	Teknologi
GUI Framework	SFML 2.6.0
Struktur Data	Vector, Map, Queue, Stack
Algoritma	Dijkstra, Binary Search
Bahasa	C++11
</div>

pie
    title Sektor Penggunaan
    "Logistik" : 45
    "E-commerce" : 30
    "Pemerintahan" : 15
    "Pendidikan" : 10

    # Alasan spesifik versi 2.6.0:
+ Stabil dan teruji
+ Kompatibilitas lintas platform
+ Dukungan OpenGL yang baik
! Versi lebih baru mungkin ada breaking changes
- Versi lama kurang fitur

/src
│
├── core/               # Logika utama
│   ├── Graph.cpp       # Algoritma Dijkstra
│   └── Calculator.cpp  # Hitung biaya
│
├── gui/                # Tampilan
│   ├── MainWindow.cpp  # Window utama
│   └── Components.cpp  # Button, Input
│
└── data/               # Data referensi
    ├── Cities.json     # Daftar kota
    └── Vehicles.json   # Spesifikasi kendaraan