<?php
header("Content-Type: application/json");

// Koneksi database
$conn = new mysqli("localhost", "aquacareethernet", "3DL9LaidDxGxm2ukfutR", "ethernet");

// Cek koneksi
if ($conn->connect_error) {
    echo json_encode([
        "status" => "error",
        "message" => "Koneksi gagal"
    ]);
    exit;
}

// Ambil data terbaru
$sql = "SELECT * FROM monitoring_air ORDER BY id DESC LIMIT 1";
$result = $conn->query($sql);

if ($result && $result->num_rows > 0) {
    $row = $result->fetch_assoc();

    echo json_encode([
        "status" => "success",
        "suhu" => (float)$row['suhu'],
        "tds" => (float)$row['tds'],
        "ph" => (float)$row['ph'],
        "kekeruhan" => (float)$row['kekeruhan'],
        "wifi_signal" => (int)$row['wifi_signal'],
        "time" => $row['created_at']
      
    ]);
} else {
    echo json_encode([
        "status" => "empty",
        "message" => "Belum ada data"
    ]);
}

$conn->close();
?>