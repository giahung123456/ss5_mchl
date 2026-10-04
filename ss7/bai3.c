#include <stdio.h>

// 1. Struct chua toi uu (thu tu khai bao gay nhieu padding bytes)
struct UnoptimizedDevice {
    char flag;             // 1 byte  + 3 bytes padding
    double totalEnergy;    // 8 bytes
    int deviceID;          // 4 bytes
    char status;           // 1 byte  + 3 bytes padding
    float voltage;         // 4 bytes
    short sensorErrorCode; // 2 bytes + 2 bytes padding
    float current;         // 4 bytes
    float measuredPower;   // 4 bytes
    int operatingMinutes;  // 4 bytes + 4 bytes padding cuoi
}; // Tong cong tren kien truc 64-bit: 40 bytes

// 2. Struct toi uu phang (sap xep giam dan kich thuoc: 8 -> 4 -> 2 -> 1)
struct OptimizedDevice {
    double totalEnergy;    // 8 bytes (offset 0)
    int deviceID;          // 4 bytes (offset 8)
    int operatingMinutes;  // 4 bytes (offset 12)
    float voltage;         // 4 bytes (offset 16)
    float current;         // 4 bytes (offset 20)
    float measuredPower;   // 4 bytes (offset 24)
    short sensorErrorCode; // 2 bytes (offset 28)
    char status;           // 1 byte  (offset 30)
    char flag;             // 1 byte  (offset 31)
}; // Tong cong tren kien truc 64-bit: 32 bytes (Tiet kiem 20% bo nho RAM)

int main() {
    printf("============================================================\n");
    printf("     PHAN TICH DUNG LUONG BO NHO STRUCT (MEMORY PADDING)    \n");
    printf("============================================================\n");
    printf("Kich thuoc Struct CHUA toi uu : %lu bytes\n", sizeof(struct UnoptimizedDevice));
    printf("Kich thuoc Struct DA toi uu   : %lu bytes\n", sizeof(struct OptimizedDevice));
    printf("RAM tiet kiem duoc moi thiet bi: %lu bytes (%.1f%%)\n",
           sizeof(struct UnoptimizedDevice) - sizeof(struct OptimizedDevice),
           ((float)(sizeof(struct UnoptimizedDevice) - sizeof(struct OptimizedDevice)) / sizeof(struct UnoptimizedDevice)) * 100.0f);
    printf("------------------------------------------------------------\n");

    // Khai bao bien struct da toi uu de luu tru thong so
    struct OptimizedDevice device;
    device.totalEnergy = 0.0;
    device.sensorErrorCode = 0;
    device.flag = 1;

    // Nhap thong so thiet bi tu ban phim
    printf("Nhap ma thiet bi (ID): ");
    if (scanf("%d", &device.deviceID) != 1) {
        printf("[LOI HE THONG] Ma thiet bi khong hop le!\n");
        return 1;
    }

    printf("Nhap dien ap (Volt): ");
    if (scanf("%f", &device.voltage) != 1) {
        printf("[LOI HE THONG] Du lieu dien ap khong hop le!\n");
        return 1;
    }

    printf("Nhap dong dien (Ampere): ");
    if (scanf("%f", &device.current) != 1) {
        printf("[LOI HE THONG] Du lieu dong dien khong hop le!\n");
        return 1;
    }

    printf("Nhap cong suat do duoc tu cam bien (Watt): ");
    if (scanf("%f", &device.measuredPower) != 1) {
        printf("[LOI HE THONG] Du lieu cong suat khong hop le!\n");
        return 1;
    }

    printf("Nhap trang thai thiet bi (1: Bat, 0: Tat): ");
    if (scanf("%hhd", &device.status) != 1) {
        printf("[LOI HE THONG] Du lieu trang thai khong hop le!\n");
        return 1;
    }

    printf("Nhap thoi gian hoat dong (phut): ");
    if (scanf("%d", &device.operatingMinutes) != 1) {
        printf("[LOI HE THONG] Du lieu thoi gian khong hop le!\n");
        return 1;
    }

    // Kiem tra du lieu dau vao bat hop le & phong thu loi cam bien
    if (device.voltage <= 0.0f || device.current < 0.0f || device.measuredPower < 0.0f || device.operatingMinutes < 0) {
        printf("\n[CANH BAO NGHIEP VU] Phat hien thong so am hoac dien ap <= 0! Huy xu ly do loi phan cung.\n");
        return 1;
    }

    if (device.status != 0 && device.status != 1) {
        printf("\n[CANH BAO NGHIEP VU] Trang thai thiet bi chi chap nhan 0 hoac 1!\n");
        return 1;
    }

    // Tinh toan cong suat ly thuyet P_calc = U * I
    float pCalc = device.voltage * device.current;

    // Kiem tra do sai lech cam bien cong suat (> 5% so voi P_calc)
    float diff = device.measuredPower - pCalc;
    if (diff < 0.0f) {
        diff = -diff; // Tinh tri tuyet doi thu cong tranh include math.h
    }

    int sensorFault = 0;
    if (pCalc > 0.0f && (diff / pCalc) > 0.05f) {
        sensorFault = 1;
    }

    // Kiem tra canh bao qua tai nguy hiem (I > 30A hoac P > 6600W)
    int overloadAlert = 0;
    if (device.current > 30.0f || device.measuredPower > 6600.0f) {
        overloadAlert = 1;
    }

    // Kiem tra ro ri dien (status == 0 nhung I > 0.05A)
    int leakageAlert = 0;
    if (device.status == 0 && device.current > 0.05f) {
        leakageAlert = 1;
    }

    // Tinh dien nang tieu thu (kWh) - ep kieu so thuc 60.0f va 1000.0f tranh loi phep chia nguyen
    float kWh = (device.measuredPower * ((float)device.operatingMinutes / 60.0f)) / 1000.0f;
    device.totalEnergy = (double)kWh;

    // Xuat ket qua bao cao giam sat
    printf("\n============================================================\n");
    printf("         BAO CAO GIAM SAT SMART HOME IOT GATEWAY            \n");
    printf("============================================================\n");
    printf("Thiet bi             : #%d\n", device.deviceID);
    printf("Trang thai           : %s\n", device.status == 1 ? "BAT (Active)" : "TAT (Inactive)");
    printf("Dien ap / Dong dien  : %.1f V  |  %.2f A\n", device.voltage, device.current);
    printf("P tinh toan (U x I)  : %.2f W\n", pCalc);
    printf("P do tu cam bien     : %.2f W\n", device.measuredPower);
    printf("Thoi gian hoat dong  : %d phut (%.2f gio)\n", device.operatingMinutes, (float)device.operatingMinutes / 60.0f);
    printf("Dien nang tieu thu   : %.4f kWh\n", device.totalEnergy);
    printf("------------------------------------------------------------\n");
    printf("CANH BAO AN TOAN & HE THONG:\n");

    if (sensorFault) {
        printf(" [!] CANH BAO: Cam bien cong suat bao sai lech (> 5%% so voi U x I)!\n");
    } else {
        printf(" [OK] Cam bien cong suat hoat dong chuan xac.\n");
    }

    if (overloadAlert) {
        printf(" [CRITICAL] QUA TAI NGUY HIEM: Dong > 30A hoac P > 6600W! Can ngat aptomat ngay!\n");
    }

    if (leakageAlert) {
        printf(" [WARNING] RO RI DIEN HOAC TAI AN: Thiet bi TAT nhung I = %.3f A (> 0.05A)!\n", device.current);
    }

    if (!overloadAlert && !leakageAlert) {
        printf(" [OK] Tai dien va he thong cach dien nam trong gioi han an toan.\n");
    }
    printf("============================================================\n");

    return 0;
}