# Startup Sequence

> Working draft. Use this document to define the firmware startup, self-test, calibration, enable, fault, and shutdown state flow for FluxNode-FOC.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| State-machine version | TBD |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: boot order, hardware defaults, peripheral initialization, self-tests, calibration gates, communication readiness, motor-enable sequence, fault transitions, and shutdown.
- Out of scope: Full FOC algorithm details and external host application behavior.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Firmware workflow and priorities | `README.md` section 21 | Generated CubeMX init plus application code. |
| Bring-up stages | `README.md` section 24 and `docs/bring-up-checklist.md` | Keep startup gates compatible with bench bring-up. |
| ADC, timer, interrupt maps | `docs/adc-map.md`, `docs/timer-map.md`, `docs/interrupt-priorities.md` | Startup must configure these consistently. |

## Boot Principles

- Power-stage outputs default safe.
- Brake chopper defaults off.
- Gate driver remains disabled until preconditions pass.
- Fault inputs and voltage measurements are available before torque can be enabled.
- Calibration required for current sensing before motor PWM is enabled.
- External commands cannot bypass safety gates.

## State Overview

| State | Purpose | Entry conditions | Exit conditions | Fault exits |
|---|---|---|---|---|
| Reset | Hardware reset and default pin states | Power-on, reset, watchdog | MCU starts firmware | Hardware fault TBD |
| Boot | Clock and basic runtime setup | Reset complete | System init complete | Clock/init fault |
| Safe IO | Force safe GPIO/PWM defaults | Boot | Outputs confirmed safe | GPIO init fault |
| Peripheral init | ADC, timers, SPI, CAN, USB, GPIO | Safe IO | Required peripherals ready | Init fault |
| Self-test | Validate essential hardware | Peripheral init | Self-tests pass | Self-test fault |
| Calibration | Current/voltage/encoder prep | Self-test pass | Required calibration valid | Calibration fault |
| Idle | Ready but torque disabled | Calibration pass | Enable command | Any monitored fault |
| Run | Motor control active | Enable command and limits OK | Disable command / fault | Any critical fault |
| Fault | Torque disabled, fault latched | Fault detected | Clear command and condition gone | Persistent fault |
| Shutdown | Controlled disable / reset prep | Disable/reset/power-down | Reset or idle | TBD |

## Detailed Startup Steps

### 1. Reset Defaults

- [ ] Confirm gate-driver enable pin default safe.
- [ ] Confirm PWM pins default safe.
- [ ] Confirm brake PWM default off.
- [ ] Confirm fan default behavior.
- [ ] Confirm chip-select pins default inactive.

### 2. Clock And Runtime

- [ ] Configure system clock.
- [ ] Configure SysTick or timebase.
- [ ] Enable fault handlers.
- [ ] Initialize minimal fault logging.

### 3. GPIO And Safety Inputs

- [ ] Configure `DRV_EN`.
- [ ] Configure `DRV_CAL`.
- [ ] Configure `DRV_nFAULT` interrupt input.
- [ ] Configure debug LEDs.
- [ ] Configure chip-select outputs.
- [ ] Read initial fault pin state.

### 4. Measurement Peripherals

- [ ] Initialize ADCs.
- [ ] Initialize ADC trigger timer source.
- [ ] Validate ADC reference / internal temperature if used.
- [ ] Start required regular conversions.
- [ ] Confirm bus voltage is within startup range.

### 5. Timer And PWM Setup

- [ ] Initialize TIM1 in center-aligned mode.
- [ ] Configure complementary outputs and dead time.
- [ ] Keep PWM outputs disabled until enable gate.
- [ ] Configure TIM1 TRGO2 for ADC timing.
- [ ] Initialize fan and brake timers.

### 6. Communication Peripherals

- [ ] Initialize SPI1 for DRV8323 and AS5047P.
- [ ] Initialize SPI3 auxiliary bus if used.
- [ ] Initialize FDCAN2.
- [ ] Initialize USB CDC.
- [ ] Publish identity/status when safe.

### 7. Gate Driver And Encoder Checks

- [ ] Read DRV8323 status registers.
- [ ] Configure DRV8323 registers.
- [ ] Verify `DRV_nFAULT` is inactive.
- [ ] Read AS5047P angle.
- [ ] Check encoder diagnostics.

### 8. Calibration Gate

- [ ] Run or load current offsets.
- [ ] Validate current readings near zero.
- [ ] Validate bus voltage scaling enough for protection.
- [ ] Load encoder offset and phase order.
- [ ] Mark calibration status.

### 9. Enable Sequence

1. TBD: Receive valid enable command.
2. TBD: Check supply voltage and temperature limits.
3. TBD: Check gate-driver and encoder status.
4. TBD: Enable gate driver.
5. TBD: Start PWM with zero torque / safe duty.
6. TBD: Enter run state after measurements are stable.

## Fault Transition Rules

| Fault | Detection source | Immediate action | Latch? | Clear condition |
|---|---|---|---|---|
| Gate-driver fault | `DRV_nFAULT`, SPI status | Disable PWM/torque | TBD | Fault pin inactive and registers reviewed |
| Overcurrent | ADC / hardware | Disable PWM/torque | TBD | TBD |
| Bus overvoltage | ADC | Brake/PWM policy TBD | TBD | Bus below threshold |
| Bus undervoltage | ADC | Disable torque | TBD | Bus above threshold |
| Overtemperature | ADC | Derate or disable | TBD | Temperature below threshold |
| Encoder fault | SPI/diagnostic | Disable closed-loop torque | TBD | Encoder valid |
| Command timeout | Protocol watchdog | Disable torque | TBD | Fresh valid command |

## Shutdown Sequence

- [ ] Command torque to zero.
- [ ] Disable PWM outputs.
- [ ] Disable gate driver when appropriate.
- [ ] Turn brake chopper off unless overvoltage logic requires otherwise.
- [ ] Stop high-rate telemetry or mark inactive.
- [ ] Store fault/calibration state if required.

## Validation Checklist

- [ ] Scope confirms PWM pins stay safe during reset and boot.
- [ ] Fault input before enable prevents gate-driver enable.
- [ ] Invalid calibration prevents run state.
- [ ] Command timeout exits run state.
- [ ] Disable command exits run state cleanly.
- [ ] Startup behavior is repeatable after power cycle and reset.

## Open Questions

- TBD: Exact persistent calibration validity checks.
- TBD: Whether USB/CAN can command enable before full diagnostics are ready.
- TBD: Gate-driver register configuration sequence.
- TBD: Final brake-chopper behavior during startup overvoltage.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
