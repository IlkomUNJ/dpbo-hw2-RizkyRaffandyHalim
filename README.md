[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/SCVt0OYF)
Name: Rizky Raffandy Halim

NIM : 1313624067

# Instruction
1. Fill in your details
2. Clone this repo
3. Move your source to this repo
4. Fill in the required section in the Readme
5. This assignment is due October 20th, 23:55 AM

# Features Implemented (list all completed)
1. **Login Pengguna**	(Memungkinkan user untuk masuk sebagai Buyer (ID 101+) atau Seller (ID 2001+))
2. **Registrasi Akun** (Memungkinkan pendaftaran Buyer atau Seller baru, secara otomatis membuat dan menghubungkan dengan Akun Bank baru)
3. **Login Administrator**	(Akses terpisah untuk admin sistem (root/toor) guna melihat laporan analitik)
4. **Data Dummy**	(Inisialisasi data awal (akun bank, buyer, seller, dan item) untuk mempercepat pengujian)
5. **Purchase Item**	(Logika inti pembelian: validasi stok, transfer dana dari akun Buyer ke Seller melalui Bank, dan pengurangan stok item)
6. **View Orders**	(Melihat riwayat pesanan yang pernah dilakukan, dilengkapi opsi filter status (ALL, PAID, COMPLETED, CANCELED)
7. **Topup Saldo**	(Menambah saldo akun bank yang terhubung dengan Buyer)
8. **Withdraw** (Saldo	Menarik saldo dari akun bank Buyer (dengan cek ketersediaan dana))
9. **List Cash Flow**	(Melihat riwayat transaksi bank (Topup/Withdraw) dalam periode Hari Ini atau Bulan Lalu)
10. **Check Spending**	(Menghitung total pengeluaran Buyer untuk pembelian di toko selama K hari terakhir)
11. **Item Management**	(Seller dapat menambahkan item baru dengan atribut ID, Nama, Harga, Kuantitas (Stok), dan status Visibility)
12. **Stock Control**	(Stok item otomatis berkurang saat terjadi pembelian yang berhasil)
13. **Last Week Transactions**	(Menampilkan semua transaksi bank (Topup/Withdraw/Transfer) yang terjadi dalam 7 hari terakhir)
14. **List All Customers**	(Menampilkan daftar lengkap semua akun bank beserta detail saldo)
15. **Dormant Accounts**	(Melaporkan akun bank yang tidak aktif (tidak ada transaksi) selama satu bulan terakhir)
16. **Top N Users Today**	(Menampilkan N pelanggan bank teratas berdasarkan jumlah transaksi hari ini)
17. **Last K Days Transactions**	9Menampilkan riwayat order toko selama K hari terakhir)
18. **Paid but Not Completed**	(Melaporkan pesanan yang sudah dibayar tetapi statusnya belum COMPLETED)
19. **Most M Frequent Items**	(Menampilkan M item yang paling banyak diorder)
20. **Most Active Buyers/Sellers**	(Menampilkan M pengguna (Buyer dan Seller) yang paling aktif bertransaksi)
21. **Layanan Bank (Transfer)**	(Logika untuk transfer dana yang aman dan tercatat antara dua akun bank (digunakan saat pembelian))
22. **Pencatatan Transaksi**	(Semua aktivitas bank dan toko dicatat secara real-time ke dalam objek BankTransaction dan StoreTransaction)
23. **Penyimpanan Riwayat File**	(Fitur persistensi data: Saat program ditutup (Exit), semua riwayat transaksi bank dan toko secara otomatis disimpan ke dalam file riwayat_transaksi.txt)

# Collaborators (The person you learned from)
1. Tidak ada

# Students (the person you taught to complete the assignments)
1. Sayyied Ridho Arhami

# AI Assistance
AI Tools: Gemini AI

Paid AI Plugin (state none, if not subscribed): Tidak Berlangganan

Total Prompts used (int estimate): Perkiraan 3 kali menggunakan prompt

Dominant prompt used for (seek solution / implement ideas) : Implementasi ide
