#include <Arduino.h>
#include <Wire.h>
#include "MLX90641_API.h"
#include "MLX90641_I2C_Driver.h"

#define MLX90641_ADDR 0x33
#define I2C_SDA       21
#define I2C_SCL       22
#define TA_SHIFT      5     // ambient offset for open-air use (Melexis default)
#define EMISSIVITY    0.95f

#define ROWS 12
#define COLS 16

static uint16_t eeData[832];
static uint16_t frameData[242];
static paramsMLX90641 mlxParams;
static float temps[ROWS * COLS];

void setup()
{
    Serial.begin(115200);
    delay(500);

    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(400000);

    Wire.beginTransmission(MLX90641_ADDR);
    if (Wire.endTransmission() != 0)
    {
        Serial.println("MLX90641 not found, check wiring");
        while (true) delay(1000);
    }

    if (MLX90641_DumpEE(MLX90641_ADDR, eeData) != 0)
    {
        Serial.println("Failed to read MLX90641 EEPROM");
        while (true) delay(1000);
    }
    if (MLX90641_ExtractParameters(eeData, &mlxParams) != 0)
    {
        Serial.println("Failed to extract MLX90641 parameters");
        while (true) delay(1000);
    }

    // 0x00=0.5Hz 0x01=1Hz 0x02=2Hz 0x03=4Hz 0x04=8Hz 0x05=16Hz
    MLX90641_SetRefreshRate(MLX90641_ADDR, 0x03);

    Serial.println("MLX90641 ready");
}

void loop()
{
    if (MLX90641_GetFrameData(MLX90641_ADDR, frameData) < 0)
    {
        Serial.println("Frame read error");
        return;
    }

    float ta = MLX90641_GetTa(frameData, &mlxParams);
    float tr = ta - TA_SHIFT;
    MLX90641_CalculateTo(frameData, &mlxParams, EMISSIVITY, tr, temps);

    if (mlxParams.brokenPixel != 0xFFFF)
    {
        MLX90641_BadPixelsCorrection(mlxParams.brokenPixel, temps);
    }

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Serial.printf("%5.1f ", temps[r * COLS + c]);
        }
        Serial.println();
    }
    Serial.println();
}
