<?php
$conn = new mysqli("localhost", "aquacareethernet", "3DL9LaidDxGxm2ukfutR", "ethernet");

if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

echo "Koneksi OK";

$json = file_get_contents("php://input");

if (!$json) {
    die("JSON KOSONG");
}

$data = json_decode($json, true);

if (json_last_error() !== JSON_ERROR_NONE) {
    die("JSON ERROR: " . json_last_error_msg() . " | RAW: " . $json);
}

// Ambil data
$suhu = $data['suhu'] ?? null;
$tds = $data['tds'] ?? null;
$ph = $data['ph'] ?? null;
$kekeruhan = $data['kekeruhan'] ?? null;
$signal = $data['signal'] ?? null;

// Validasi
if ($suhu === null || $tds === null || $ph === null || $kekeruhan === null || $signal === null) {
    die("DATA TIDAK LENGKAP");
}

// Insert
$stmt = $conn->prepare("INSERT INTO monitoring_air (suhu, tds, ph, kekeruhan, wifi_signal) VALUES (?, ?, ?, ?, ?)");

if (!$stmt) {
    die("Prepare failed: " . $conn->error);
}

$stmt->bind_param("ddddd", $suhu, $tds, $ph, $kekeruhan, $signal);

if ($stmt->execute()) {
    echo "OK";
} else {
    echo "EXEC ERROR: " . $stmt->error;
}

$conn->close();
?>
