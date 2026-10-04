#include <stdio.h>

// Khai bao cau truc quan ly thiet bi thong minh
struct SmartDevice {
    int id;
    float voltage;      // Dien ap (V)
    float current;      // Dong dien (A)
    float power_watt;   // Cong suat tieu thu (W)
    int is_active;      // Trang thai thiet bi: 1 - Bat, 0 - Tat
    int is_overloaded;  // Canh bao qua tai: 1 - Qua tai, 0 - An toan
};

int main() {
    // Khoi tao thong tin thiet bi Binh Nong Lanh
    struct SmartDevice heater;
    heater.id = 101;
    heater.voltage = 220.0f;
    heater.current = 35.0f;     // Cam bien ghi nhan dong dien 35A (vuot nguong 30A)
    heater.is_active = 1;       // Nguoi dung ban dau bat thiet bi
    heater.is_overloaded = 0;   // Mac dinh ban dau an toan
    heater.power_watt = 0.0f;   // Khoi tao cong suat ve 0

    // 1. Kiem tra quy tac an toan qua tai dong dien (> 30A)
    if (heater.current > 30.0f) {
        heater.is_overloaded = 1;
        heater.is_active = 0;   // Tu dong ngat dien thiet bi de bao ve aptomat
    }

    // 2. Tinh toan cong suat tieu thu dua tren trang thai hoat dong sau kiem tra
    if (heater.is_active == 1) {
        heater.power_watt = heater.voltage * heater.current;
    } else {
        heater.power_watt = 0.0f; // Neu thiet bi TAT (hoac da bi ngat), cong suat bat buoc bang 0
    }

    // 3. In thong so thiet bi ra man hinh ung dung IoT
    printf("=== HETHONG GIAM SAT THIET BI SMART HOME ===\n");
    printf("Ma thiet bi: %d\n", heater.id);
    printf("Trang thai hoat dong: %s\n", heater.is_active ? "BAT" : "TAT");
    printf("Cong suat tieu thu: %.2f W\n", heater.power_watt);
    printf("Canh bao qua tai (>30A): %s\n", heater.is_overloaded ? "CO (DA NGAT DIEN)" : "KHONG");

    return 0;
}