#include <stdio.h>
#include <string.h>

// Cau truc quan ly thiet bi thong minh
struct SmartDevice {
    int deviceID;
    char deviceName[50];
    int status;             // 1: Dang bat, 0: Da tat
    float ratedPowerKW;     // Cong suat dinh muc (kW)
};

// Cau truc cam bien dien nang va hien dien
struct EnergySensor {
    float voltageVolts;         // Dien ap do duoc (V)
    float currentAmperes;       // Dong dien do duoc (A)
    int unoccupancyMinutes;     // Thoi gian vang nguoi (phut)
};

int main() {
    // 1. Khoi tao du lieu mau cho he thong
    struct SmartDevice acDevice;
    acDevice.deviceID = 201;
    strcpy(acDevice.deviceName, "Dieu hoa Inverter 2.5 HP");
    acDevice.status = 1;            // Thiet bi dang bat
    acDevice.ratedPowerKW = 2.0f;   // Cong suat 2.0 kW

    struct EnergySensor mainSensor;
    mainSensor.voltageVolts = 220.0f;
    mainSensor.currentAmperes = 12.5f;     // Gia su dong van hanh binh thuong
    mainSensor.unoccupancyMinutes = 20;    // Phong khong co nguoi 20 phut

    float operatingHours = 180.0f; // Tong gio van hanh du kien trong thang

    // 2. Kiem tra rang buoc & loi thuong gap ve du lieu
    // Kiem tra loi phan cung cam bien (gia tri am)
    if (mainSensor.voltageVolts < 0.0f || mainSensor.currentAmperes < 0.0f || mainSensor.unoccupancyMinutes < 0) {
        printf("[LOI HE THONG] Du lieu cam bien bat thuong (chi so am)! Huy thao tac de bao tri phan cung.\n");
        return 1;
    }

    // Kiem tra gio van hanh hop le trong thang (0 < operatingHours <= 744)
    if (operatingHours <= 0.0f || operatingHours > 744.0f) {
        printf("[LOI HE THONG] So gio van hanh thang khong hop le (phai > 0 va <= 744 gio)!\n");
        return 1;
    }

    // 3. Xu ly logic kiem soat an toan & tu dong hoa
    int emergencyCutoff = 0;
    int energySavingCutoff = 0;

    // Quy tac Uu tien 1: Kiem soat an toan dong dien (> 30.0A)
    if (mainSensor.currentAmperes > 30.0f) {
        acDevice.status = 0;
        emergencyCutoff = 1;
    } 
    // Quy tac Uu tien 2: Tu dong hoa tiet kiem nang luong (vang nguoi >= 15 phut)
    else if (acDevice.status == 1 && mainSensor.unoccupancyMinutes >= 15) {
        acDevice.status = 0;
        energySavingCutoff = 1;
    }

    // 4. Tinh toan dien nang tieu thu va hoa don EVN theo bac thang
    float totalKWh = acDevice.ratedPowerKW * operatingHours;
    float tempKWh = totalKWh;

    float kwhBac1 = 0.0f, kwhBac2 = 0.0f, kwhBac3 = 0.0f, kwhBac4 = 0.0f;
    float costBac1 = 0.0f, costBac2 = 0.0f, costBac3 = 0.0f, costBac4 = 0.0f;

    // Bac 1: Cho 50 kWh dau tien (1.806 VND/kWh)
    if (tempKWh > 50.0f) {
        kwhBac1 = 50.0f;
    } else {
        kwhBac1 = tempKWh;
    }
    costBac1 = kwhBac1 * 1806.0f;
    tempKWh = tempKWh - kwhBac1;

    // Bac 2: Cho kWh tu 51 den 100 (1.866 VND/kWh)
    if (tempKWh > 50.0f) {
        kwhBac2 = 50.0f;
    } else {
        kwhBac2 = tempKWh;
    }
    costBac2 = kwhBac2 * 1866.0f;
    tempKWh = tempKWh - kwhBac2;

    // Bac 3: Cho kWh tu 101 den 200 (2.167 VND/kWh)
    if (tempKWh > 100.0f) {
        kwhBac3 = 100.0f;
    } else {
        kwhBac3 = tempKWh;
    }
    costBac3 = kwhBac3 * 2167.0f;
    tempKWh = tempKWh - kwhBac3;

    // Bac 4: Cho kWh tu 201 tro len (2.729 VND/kWh)
    if (tempKWh > 0.0f) {
        kwhBac4 = tempKWh;
        costBac4 = kwhBac4 * 2729.0f;
    }

    float totalBillVND = costBac1 + costBac2 + costBac3 + costBac4;

    // 5. Xuat bao cao giam sat len Smart Gateway
    printf("============================================================\n");
    printf("           RIKKEI SMART HOME - CENTRAL GATEWAY              \n");
    printf("============================================================\n");
    printf("Ma thiet bi         : #%d\n", acDevice.deviceID);
    printf("Ten thiet bi        : %s\n", acDevice.deviceName);
    printf("Cong suat dinh muc  : %.2f kW\n", acDevice.ratedPowerKW);
    printf("Thong so cam bien   : %.1f V | %.2f A | Vang nguoi: %d phut\n",
           mainSensor.voltageVolts, mainSensor.currentAmperes, mainSensor.unoccupancyMinutes);
    printf("Trang thai hien tai : %s\n", acDevice.status ? "DANG BAT" : "DA NGAT/TAT");
    printf("------------------------------------------------------------\n");
    printf("CANH BAO & DIEU KHIEN HE THONG:\n");

    if (emergencyCutoff) {
        printf(" [!] NGUY CAP: Dong dien %.2fA vuot nguong an toan 30.0A!\n", mainSensor.currentAmperes);
        printf("     -> DA KICH HOAT NGAT KHAN CAP PHONG CHONG CHAY NO.\n");
    } else if (energySavingCutoff) {
        printf(" [*] TIET KIEM DIEN: Khong co nguoi trong phong >= 15 phut.\n");
        printf("     -> HE THONG TU DONG TAT THIET BI.\n");
    } else {
        printf(" [OK] He thong hoat dong on dinh trong gioi han an toan.\n");
    }

    printf("------------------------------------------------------------\n");
    printf("TINH TOAN HOA DON TIEN DIEN (CHUA VAT):\n");
    printf("Tong thoi gian van hanh : %.1f gio/thang\n", operatingHours);
    printf("Tong dien nang tieu thu : %.2f kWh\n", totalKWh);
    printf(" + Bac 1 (1 - 50 kWh)   : %6.1f kWh  x 1.806 = %10.0f VND\n", kwhBac1, costBac1);
    printf(" + Bac 2 (51 - 100 kWh) : %6.1f kWh  x 1.866 = %10.0f VND\n", kwhBac2, costBac2);
    printf(" + Bac 3 (101 - 200 kWh): %6.1f kWh  x 2.167 = %10.0f VND\n", kwhBac3, costBac3);
    printf(" + Bac 4 (> 200 kWh)    : %6.1f kWh  x 2.729 = %10.0f VND\n", kwhBac4, costBac4);
    printf("------------------------------------------------------------\n");
    printf("TONG TIEN DIEN DU KIEN  : %.0f VND\n", totalBillVND);
    printf("============================================================\n");

    return 0;
}