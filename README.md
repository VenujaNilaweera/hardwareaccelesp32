# hardwareaccelesp32

FPGA projects targeting an Intel MAX II (EPM240T100C5) CPLD, built with Quartus Prime.

## Projects

- **ledblink/** (root) — basic LED blink, block-diagram (`.bdf`) design.
- **test1/** — top-level entity `ledblibk` (`.bdf`), including:
  - `delay500ms.v` / `500msdelay.vhd` — 500ms delay module (Verilog + VHDL versions)
  - `spiled.v` (`spi_led_control`) — SPI-driven LED control module

## Toolchain

- Quartus Prime 25.1 (Lite/Standard)
- Device family: MAX II, EPM240T100C5

## Notes

Generated/compiled artifacts (`db/`, `incremental_db/`, `output_files/`, `simulation/`, `.qws`, programming files, reports) are excluded via `.gitignore` and regenerate on compile — only design sources and project files (`.qpf`/`.qsf`/`.bdf`/`.bsf`/`.v`/`.vhd`) are tracked.
