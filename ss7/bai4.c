#include <stdio.h>
#include <string.h>

// 1. Dinh nghia cac kieu du lieu cau truc (Data Model)

// Cau truc quan ly thiet bi dien thong minh
struct SmartDevice {
    int deviceID;
    char deviceName[50];
    int status;         // 1: Bat, 0: Tat
    float powerWatt;    // Cong suat tieu thu (Watt)
};

// Cau truc cam bien giam sat dien nang & hien dien
struct EnergySensor {
    float voltage;              // Dien ap van hanh (Volt)
    float current;              // Dong dien do duoc (Amper)
    int unoccupancyMinutes;     // Thoi gian phong trong khong co nguoi (Phut)
};

// Cau truc bao cao tong hop nang luong & an toan
struct PowerReport {
    float totalPowerWatt;   // Tong cong suat tai (Watt)
    float totalCurrent;     // Tong dong dien (Amper)
    float energyKWh;        // Dien nang tieu thu uoc tinh (kWh)
    int isOverloaded;       // 1: Qua tai (>30A), 0: An toan
};

int main() {
    // 2. Khoi tao du lieu mo phong cho thiet bi & cam bien
    struct SmartDevice ac;
    ac.deviceID = 101;
    strcpy(ac.deviceName, "Dieu hoa Inverter Living Room");
    ac.status = 1;              // Ban dau dang bat
    ac.powerWatt = 2200.0f;     // Cong suat 2200W

    struct SmartDevice light;
    light.deviceID = 102;
    strcpy(light.deviceName, "He thong Den Chieu Sang LED");
    light.status = 1;           // Dang bat
    light.powerWatt = 150.0f;   // Cong suat 150W

    struct EnergySensor sensor;
    sensor.voltage = 220.0f;            // Dien ap luoi 220V
    sensor.current = 10.68f;            // Dong do thuc te
    sensor.unoccupancyMinutes = 25;     // Khong co nguoi trong 25 phut (> 15 phut)

    struct PowerReport report;
    report.totalPowerWatt = 0.0f;
    report.totalCurrent = 0.0f;
    report.energyKWh = 0.0f;
    report.isOverloaded = 0;

    // Kiem tra du lieu dau vao phong thu (Defensive Validation)
    if (sensor.voltage <= 0.0f || sensor.unoccupancyMinutes < 0) {
        printf("[LOI HE THONG] Thong so cam bien bat thuong! Huy xu ly de dam bao an toan.\n");
        return 1;
    }

    // 3. Thuc thi kich ban tu dong hoa (Automation Scenario)
    int autoTurnOffTriggered = 0;
    if (sensor.unoccupancyMinutes > 15) {
        if (ac.status == 1) {
            ac.status = 0;          // Tu dong ngat nguon dieu hoa
            ac.powerWatt = 0.0f;    // Cong suat cap nhat lap tuc ve 0W
            autoTurnOffTriggered = 1;
        }
    }

    // Dam bao tinh nhat quan cua thiet bi den
    if (light.status == 0) {
        light.powerWatt = 0.0f;
    }

    // 4. Tong hop du lieu vao PowerReport
    report.totalPowerWatt = ac.powerWatt + light.powerWatt;
    
    // Tinh tong dong dien I = P / U
    report.totalCurrent = report.totalPowerWatt / sensor.voltage;

    // Tinh dien nang tieu thu trong ca giam sat (gia dinh 4 gio hoat dong lien tuc)
    float shiftHours = 4.0f;
    report.energyKWh = (report.totalPowerWatt * shiftHours) / 1000.0f;

    // Kiem tra an toan qua tai he thong (> 30.0A)
    if (report.totalCurrent > 30.0f) {
        report.isOverloaded = 1;
    } else {
        report.isOverloaded = 0;
    }

    // Uoc tinh chi phi dien nang theo bieu gia dien sinh hoat co ban EVN
    float estimatedCost = 0.0f;
    if (report.energyKWh <= 50.0f) {
        estimatedCost = report.energyKWh * 1806.0f;
    } else {
        estimatedCost = (50.0f * 1806.0f) + ((report.energyKWh - 50.0f) * 1866.0f);
    }

    // 5. Hien thi Dashboard giam sat Smart Home Central Gateway
    printf("======================================================================\n");
    printf("        RKHOME SMART HOME EDGE CONTROLLER - LIVING ROOM GATEWAY       \n");
    printf("======================================================================\n");
    printf("1. THONG SO CAM BIEN REAL-TIME:\n");
    printf("   + Dien ap luoi do duoc     : %.1f V\n", sensor.voltage);
    printf("   + Dong dien tong hien tai  : %.2f A\n", sensor.current);
    printf("   + Thoi gian phong trong    : %d phut\n", sensor.unoccupancyMinutes);
    printf("----------------------------------------------------------------------\n");
    printf("2. TRANG THAI CHI TIET TUNG THIET BI:\n");
    printf("   [#%d] %-32s | %-8s | Cong suat: %7.1f W\n", 
           ac.deviceID, ac.deviceName, ac.status ? "ON" : "OFF", ac.powerWatt);
    printf("   [#%d] %-32s | %-8s | Cong suat: %7.1f W\n", 
           light.deviceID, light.deviceName, light.status ? "ON" : "OFF", light.powerWatt);

    if (autoTurnOffTriggered) {
        printf("   >> [EVENT LOG]: Phong trong %d phut (>15p) -> Tu dong ngat Dieu hoa de tiet kiem dien!\n", 
               sensor.unoccupancyMinutes);
    }
    printf("----------------------------------------------------------------------\n");
    printf("3. TONG HOP NANG LUONG VA CANH BAO (POWER REPORT):\n");
    printf("   + Tong cong suat tai phong : %.1f W\n", report.totalPowerWatt);
    printf("   + Tong tai dong dien       : %.2f A\n", report.totalCurrent);
    printf("   + Dien nang uoc tinh (ca 4h): %.4f kWh\n", report.energyKWh);
    printf("   + Chi phi du kien (tam tinh): %.0f VND\n", estimatedCost);
    printf("   + Trang thai qua tai (>30A): %s\n", 
           report.isOverloaded ? "[NGUY HIEM] CO QUÁ TẢI DÒNG!" : "[AN TOAN] BÌNH THƯỜNG");
    
    if (report.isOverloaded) {
        printf("   [!] CANH BAO NGUY HIEM: Tong dong dien vuot nguong cho phep 30A!\n");
        printf("       Yeu cau ha aptomat hoac ngat bot thiet bi cong suat lon ngay lap tuc.\n");
    }
    printf("======================================================================\n");

    return 0;
}