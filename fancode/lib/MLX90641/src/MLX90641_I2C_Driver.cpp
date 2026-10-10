// Arduino (Wire) I2C driver for the Melexis MLX90641 API.
// Replaces the mbed driver shipped in github.com/melexis/mlx90641-library.
#include <Arduino.h>
#include <Wire.h>
#include "MLX90641_I2C_Driver.h"

// ESP32 Wire buffer is 128 bytes, so read in chunks of 32 words (64 bytes).
#define MLX90641_I2C_CHUNK_WORDS 32

void MLX90641_I2CInit(void)
{
    Wire.begin();
}

int MLX90641_I2CGeneralReset(void)
{
    Wire.beginTransmission(0x00);
    Wire.write(0x06);
    if (Wire.endTransmission() != 0)
    {
        return -1;
    }
    delayMicroseconds(50);
    return 0;
}

int MLX90641_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    uint16_t done = 0;

    while (done < nMemAddressRead)
    {
        uint16_t words = nMemAddressRead - done;
        if (words > MLX90641_I2C_CHUNK_WORDS)
        {
            words = MLX90641_I2C_CHUNK_WORDS;
        }
        uint16_t addr = startAddress + done;

        Wire.beginTransmission(slaveAddr);
        Wire.write(addr >> 8);
        Wire.write(addr & 0xFF);
        if (Wire.endTransmission(false) != 0)
        {
            return -1;
        }

        uint8_t bytes = words * 2;
        if (Wire.requestFrom(slaveAddr, bytes) != bytes)
        {
            return -1;
        }
        for (uint16_t i = 0; i < words; i++)
        {
            uint8_t msb = Wire.read();
            uint8_t lsb = Wire.read();
            data[done + i] = ((uint16_t)msb << 8) | lsb;
        }
        done += words;
    }

    return 0;
}

int MLX90641_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    Wire.beginTransmission(slaveAddr);
    Wire.write(writeAddress >> 8);
    Wire.write(writeAddress & 0xFF);
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
    if (Wire.endTransmission() != 0)
    {
        return -1;
    }

    uint16_t dataCheck;
    if (MLX90641_I2CRead(slaveAddr, writeAddress, 1, &dataCheck) != 0)
    {
        return -1;
    }
    if (dataCheck != data)
    {
        return -2;
    }

    return 0;
}

void MLX90641_I2CFreqSet(int freq)
{
    // freq in kHz, same as the original Melexis driver
    Wire.setClock((uint32_t)freq * 1000);
}
