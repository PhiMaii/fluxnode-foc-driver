# Pinout

> Working draft. Use this document as the human-readable MCU, connector, and test-point pinout for FluxNode-FOC. Keep it synchronized with CubeMX and the KiCad schematic before each hardware release.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| MCU | STM32G474RETx, LQFP64 |
| Source files | `firmware/flux-node-foc.ioc`, KiCad schematics |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: MCU pin assignments, signal labels, peripheral functions, connector pinouts, debug pins, power pins, and board-level notes.
- Out of scope: Electrical design calculations and firmware driver details.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| MCU alternate functions and labels | `firmware/flux-node-foc.ioc` | Current generated configuration. |
| Connector pin numbers | KiCad schematic and PCB | Must be filled from hardware design. |
| External interface policy | `README.md` sections 15-18 | CAN-FD, USB, auxiliary SPI/I2C, SWD, LEDs. |

## MCU Pin Map

| MCU pin | Signal label | Peripheral / function | Direction | Schematic sheet | Connector / test point | Notes |
|---|---|---|---|---|---|---|
| PA0 | `SENSE_U_C` | ADC2_IN1 | Analog in | TBD | TBD | Phase voltage C. |
| PA1 | `SENSE_U_B` | ADC1_IN2 | Analog in | TBD | TBD | Phase voltage B. |
| PA2 | `DRV_CAL` | GPIO output | Out | TBD | TBD | Gate-driver calibration control. |
| PA3 | `DRV_EN` | GPIO output | Out | TBD | TBD | Gate-driver enable. |
| PA4 | `SPI1_DRV_CS` | GPIO output | Out | TBD | TBD | DRV8323 chip select. |
| PA5 | `SPI1_SCK` | SPI1 SCK | Out | TBD | TBD | Shared DRV8323 / AS5047P SPI bus. |
| PA6 | `SPI1_MISO` | SPI1 MISO | In | TBD | TBD | Shared SPI bus. |
| PA7 | `SPI1_MOSI` | SPI1 MOSI | Out | TBD | TBD | Shared SPI bus. |
| PA8 | `SENSE_TEMP_FET` | ADC5_IN1 | Analog in | TBD | TBD | MOSFET temperature. |
| PA9 | `SENSE_TEMP_MOT` | ADC5_IN2 | Analog in | TBD | TBD | Motor temperature. |
| PA11 | USB_DM | USB FS | Bidirectional | TBD | USB-C | TBD |
| PA12 | USB_DP | USB FS | Bidirectional | TBD | USB-C | TBD |
| PA13 | SWDIO | SWD | Bidirectional | TBD | SWD | Debug/programming. |
| PA14 | SWCLK | SWD | In | TBD | SWD | Debug/programming. |
| PB0 | `DRV_SOA` | ADC3_IN12 | Analog in | TBD | TBD | Phase current A. |
| PB1 | `DRV_SOB` | ADC1_IN12 | Analog in | TBD | TBD | Phase current B. |
| PB2 | `DRV_SOC` | ADC2_IN12 | Analog in | TBD | TBD | Phase current C. |
| PB3 | SWO | Trace | Out | TBD | SWD | Optional debug trace. |
| PB4 | `LED_PWR` | GPIO output | Out | TBD | LED | TBD |
| PB5 | FDCAN2_RX | FDCAN2 | In | TBD | CAN | TBD |
| PB6 | FDCAN2_TX | FDCAN2 | Out | TBD | CAN | TBD |
| PB7 | I2C1_SDA | I2C1 | Bidirectional | TBD | Auxiliary | TBD |
| PB8 | I2C1_SCL / BOOT0 | I2C1 / boot | Out / boot | TBD | Auxiliary | Check boot strapping. |
| PB9 | `DRV_PWM_LA` | TIM1_CH3N | Out | TBD | Gate driver | Low-side phase A TBD. |
| PB11 | `EXT_FAN` | TIM2_CH4 | Out | TBD | Fan | PWM fan output. |
| PB12 | `SENSE_TEMP_BRAKE` | ADC4_IN3 | Analog in | TBD | TBD | Brake temperature. |
| PB13 | `SENSE_VBAT` | ADC3_IN5 | Analog in | TBD | TBD | DC bus voltage. |
| PB14 | `DRV_PWM_LB` | TIM1_CH2N | Out | TBD | Gate driver | Low-side phase B TBD. |
| PB15 | `SENSE_USB_VBUS` | ADC4_IN5 | Analog in | TBD | USB-C | USB VBUS sense. |
| PC0 | `DRV_PWM_HC` | TIM1_CH1 | Out | TBD | Gate driver | High-side phase C TBD. |
| PC1 | `DRV_PWM_HB` | TIM1_CH2 | Out | TBD | Gate driver | High-side phase B TBD. |
| PC2 | `PWM_DRV_HA` | TIM1_CH3 | Out | TBD | Gate driver | High-side phase A TBD. |
| PC3 | `SENSE_U_A` | ADC2_IN9 | Analog in | TBD | TBD | Phase voltage A. |
| PC4 | `DRV_nFAULT` | EXTI4 | In | TBD | Gate driver | Fault interrupt. |
| PC5 | `SPI1_MAG_CS` | GPIO output | Out | TBD | Encoder | AS5047P chip select. |
| PC9 | `BRAKE_PWM` | TIM3_CH4 | Out | TBD | Brake chopper | PWM output. |
| PC10 | SPI3_SCK | SPI3 | Out | TBD | Auxiliary SPI | TBD |
| PC11 | SPI3_MISO | SPI3 | In | TBD | Auxiliary SPI | TBD |
| PC12 | SPI3_MOSI | SPI3 | Out | TBD | Auxiliary SPI | TBD |
| PC13 | `DRV_PWM_LC` | TIM1_CH1N | Out | TBD | Gate driver | Low-side phase C TBD. |
| PC14 | `SPI1_AUX1_CS` | GPIO output | Out | TBD | TBD | Uses OSC32_IN pin. |
| PD2 | `LED_DBG` | GPIO output | Out | TBD | LED | Debug LED. |
| PF0 | OSC_IN | HSE | In | TBD | Crystal/oscillator | TBD |
| PF1 | OSC_OUT | HSE | Out | TBD | Crystal/oscillator | TBD |
| PG10 | NRST | Reset | In | TBD | SWD/reset | TBD |

## Power And Motor Connectors

| Connector | Pin | Signal | Direction | Voltage/current | Notes |
|---|---:|---|---|---|---|
| Battery input | TBD | `VBUS` / battery + | In | TBD | XT connector TBD. |
| Battery input | TBD | GND | In | TBD | TBD |
| Motor phase | TBD | Phase A | Out | TBD | TBD |
| Motor phase | TBD | Phase B | Out | TBD | TBD |
| Motor phase | TBD | Phase C | Out | TBD | TBD |
| Brake resistor | TBD | TBD | Out | TBD | Optional external resistor. |

## Communication And Debug Connectors

| Connector | Pin | Signal | Direction | Notes |
|---|---:|---|---|---|
| CAN | TBD | CANH | Bidirectional | Termination policy TBD. |
| CAN | TBD | CANL | Bidirectional | TBD |
| CAN | TBD | GND | Reference | TBD |
| USB-C | TBD | D+ | Bidirectional | USB FS. |
| USB-C | TBD | D- | Bidirectional | USB FS. |
| SWD | TBD | SWDIO | Bidirectional | Debug/programming. |
| SWD | TBD | SWCLK | In | Debug/programming. |
| SWD | TBD | NRST | In | Reset. |
| SWD | TBD | SWO | Out | Optional trace. |

## Auxiliary Interfaces

| Interface | Signal | MCU pin | Connector pin | Pull-up / termination | Notes |
|---|---|---|---:|---|---|
| SPI3 auxiliary | SCK | PC10 | TBD | TBD | External sensor bus. |
| SPI3 auxiliary | MISO | PC11 | TBD | TBD | TBD |
| SPI3 auxiliary | MOSI | PC12 | TBD | TBD | TBD |
| I2C1 auxiliary | SDA | PB7 | TBD | TBD | Pull-up policy TBD. |
| I2C1 auxiliary | SCL | PB8 | TBD | TBD | BOOT0 interaction to verify. |
| Fan | `EXT_FAN` | PB11 | TBD | TBD | Requires external driver stage. |

## Pin Review Checklist

- [ ] Every CubeMX label matches KiCad net name.
- [ ] Every connector pin number is filled from schematic.
- [ ] High-current and high-voltage connector ratings confirmed.
- [ ] SWD and reset accessible on assembled board.
- [ ] BOOT0 behavior checked with I2C1_SCL use.
- [ ] CAN termination option documented.
- [ ] USB-C CC and VBUS behavior documented.
- [ ] Gate-driver PWM polarity verified against DRV8323 mode.

## Open Questions

- TBD: Final connector part numbers and pin numbers.
- TBD: Final phase naming convention A/B/C versus U/V/W.
- TBD: Whether `SPI1_AUX1_CS` remains on PC14.
- TBD: Whether TIM1 phase/channel assignment should be renamed for clarity.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
