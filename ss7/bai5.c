#include <stdio.h>

// Dinh nghia cau truc quan ly cam bien moi truong
struct EnvironmentSensor {
    int sensor_id;
    char room_name[51];
    float temperature;
    float humidity;
    int battery_level;
};

int main() {
    struct EnvironmentSensor sensor;

    // Nhap du lieu cho cac thanh phan cua struct qua toan tu cham (.)
    if (scanf("%d", &sensor.sensor_id) != 1) {
        return 1;
    }
    if (scanf("%50s", sensor.room_name) != 1) {
        return 1;
    }
    if (scanf("%f", &sensor.temperature) != 1) {
        return 1;
    }
    if (scanf("%f", &sensor.humidity) != 1) {
        return 1;
    }
    if (scanf("%d", &sensor.battery_level) != 1) {
        return 1;
    }

    // 1. Kiem tra tinh hop le cua dung luong pin [0, 100]
    if (sensor.battery_level < 0 || sensor.battery_level > 100) {
        printf("Loi: Dung luong pin khong hop le!\n");
        return 0;
    }

    // 2. Danh gia trang thai an toan dua tren cac nguong nghiep vu
    const char *status_str = "SAFE";

    if (sensor.battery_level < 10 || sensor.temperature > 60.0f) {
        status_str = "CRITICAL";
    } else if ((sensor.battery_level >= 10 && sensor.battery_level <= 20) ||
               (sensor.temperature >= 40.0f && sensor.temperature <= 60.0f) ||
               (sensor.humidity > 85.0f)) {
        status_str = "WARNING";
    } else {
        status_str = "SAFE";
    }

    // 3. Xuat bao cao giam sat chuan he thong SMART_HOME_IOT
    printf("--- HE THONG MONITORING SMART_HOME_IOT ---\n");
    printf("Ma cam bien: %d\n", sensor.sensor_id);
    printf("Phong: %s\n", sensor.room_name);
    printf("Nhiet do: %.2f C\n", sensor.temperature);
    printf("Do am: %.2f %%\n", sensor.humidity);
    printf("Dung luong pin: %d %%\n", sensor.battery_level);
    printf("Trang thai: %s\n", status_str);

    return 0;
}