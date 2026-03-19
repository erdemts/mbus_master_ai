# mbus_master — M-Bus Master Protocol Library

M-Bus (EN 13757) master protocol implementation for reading utility meters.

## Key Features

- **Zero firmware dependency**: No `in223Gateway.h`, no `HARDWARE_TYPE` switches
- **Port interface**: `mbus_master_port_t` with send, configure_uart, delay_ms, get_tick_ms
- **Firmware-owned buffers**: RX data buffer provided via config
- **Optional NT750**: MBUS converter module enabled via CMake flag
- **Works on M031 and M2354**: Single codebase, port handles UART differences

## RAM Usage

| Component | RAM |
|-----------|-----|
| `mbus_lib_t` context | ~48 B (state + config pointer) |
| RX data buffer | 0 B (firmware-owned, typically 528 B) |

## Integration

```c
#include "mbus_master/mbus_master.h"

static uint8_t s_mbus_rx_buf[528];

static const mbus_master_port_t s_port = {
    .send           = my_uart_send,
    .configure_uart = my_uart_configure,
    .delay_ms       = my_delay,
    .get_tick_ms    = my_get_tick,
};

static const mbus_master_config_t s_config = {
    .buf  = { .rx_data = s_mbus_rx_buf, .rx_data_size = sizeof(s_mbus_rx_buf) },
    .port = &s_port,
    .baudrate = 2400,
};

static mbus_lib_t mbus;

void init(void) {
    mbus_master_init(&mbus, &s_config);
}

/* In UART RX thread: */
mbus_master_input_process(&mbus, uart_data, uart_len);

/* To read a meter: */
mbus.master.mbus_id = 12345678;
mbus.master.read_type = 0;  /* SND_NKE Network, 0x53, wait ACK */
mbus_master_res_t res = mbus_master_read(&mbus);
```

## Build

```bash
cmake -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=../../tools/cmake-toolchains/arm-none-eabi-gcc.cmake \
  -DCMAKE_C_FLAGS="-mcpu=cortex-m0 -mthumb" \
  -DCMAKE_BUILD_TYPE=Release \
  -DMBUS_MASTER_NT750=ON
```
