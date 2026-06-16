# Bring-Up Checklist

> Working draft. Use this document as the controlled hardware and firmware bring-up record for each FluxNode-FOC board revision and assembled unit.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Board serial / unit ID | TBD |
| Firmware revision / commit | TBD |
| Operator | TBD |
| Bench supply / limits | TBD |
| Status | Not started / in progress / passed / blocked |
| Last reviewed | TBD |

## Scope

- In scope: staged bench bring-up, safety setup, required equipment, measurements, pass/fail records, deviations, and release criteria for an assembled board.
- Out of scope: Detailed calibration math, communication protocol definitions, and control-loop implementation details except where they are required to pass a bring-up stage.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Bring-up stages | `README.md` section 24 | This checklist expands the staged strategy into a board-level record. |
| Pin, ADC, and timer mappings | `docs/pinout.md`, `docs/adc-map.md`, `docs/timer-map.md` | Use these when checking signals and measurements. |
| Hardware details | KiCad schematic and PCB | Fill connector, test-point, and measurement expectations from the current revision. |

## Safety Setup

- [ ] Current-limited supply configured.
- [ ] Fuse or current-limited source in series with motor supply.
- [ ] Emergency disconnect reachable.
- [ ] Motor mechanically secured.
- [ ] Scope probing plan reviewed for ground safety.
- [ ] Thermal camera or temperature probe available for powered tests.
- [ ] Fire-safe bench area prepared for high-power tests.

## Required Equipment

| Equipment | Model / settings | Purpose | Ready |
|---|---|---|---|
| Bench supply | TBD | Low-voltage and motor-bus power | [ ] |
| Multimeter | TBD | Resistance and voltage checks | [ ] |
| Oscilloscope | TBD | PWM, gate, phase, and bus measurements | [ ] |
| Differential probe | TBD | Switching-node measurements | [ ] |
| SWD probe | TBD | Programming and debug | [ ] |
| CAN-FD adapter | TBD | Communication validation | [ ] |
| USB cable | TBD | USB CDC validation | [ ] |
| Test motor | TBD | First rotation | [ ] |

## Stage 1: Unpowered Checks

- [ ] Visual inspection complete.
- [ ] Component orientation checked.
- [ ] MOSFET orientation checked.
- [ ] Gate driver orientation checked.
- [ ] MCU orientation checked.
- [ ] Connector polarity checked.
- [ ] Power rails resistance to ground recorded.
- [ ] Shunt values checked.
- [ ] Voltage-divider values checked.
- [ ] NTC networks checked.
- [ ] Solder bridges corrected.

| Measurement | Expected | Actual | Pass | Notes |
|---|---:|---:|---|---|
| Battery input to GND resistance | TBD | TBD | [ ] | TBD |
| 5 V rail to GND resistance | TBD | TBD | [ ] | TBD |
| 3.3 V rail to GND resistance | TBD | TBD | [ ] | TBD |
| Phase U to GND resistance | TBD | TBD | [ ] | TBD |
| Phase V to GND resistance | TBD | TBD | [ ] | TBD |
| Phase W to GND resistance | TBD | TBD | [ ] | TBD |

## Stage 2: Low-Voltage Control Power

- [ ] Power supply current limit set to TBD.
- [ ] 3.3 V rail measured.
- [ ] Other auxiliary rails measured.
- [ ] MCU reset behavior verified.
- [ ] HSE clock verified.
- [ ] SWD connection works.
- [ ] Firmware can be flashed.
- [ ] Debug LEDs can be controlled.
- [ ] USB enumerates without motor power, if intended.

## Stage 3: Gate Driver Without Motor Operation

- [ ] `DRV_EN` default state verified.
- [ ] `DRV_CAL` behavior verified.
- [ ] SPI communication with DRV8323 verified.
- [ ] Gate-driver fault registers read and logged.
- [ ] `DRV_nFAULT` interrupt behavior verified.
- [ ] DAC / VREF behavior verified, if applicable.
- [ ] TIM1 PWM observed with bridge disabled.
- [ ] Complementary output polarity verified.
- [ ] Dead time measured.

## Stage 4: Power-Stage Switching

- [ ] Low DC-bus voltage selected: TBD V.
- [ ] Strict current limit selected: TBD A.
- [ ] No motor connected for first switching tests, if applicable.
- [ ] Half-bridge switching tested one phase at a time.
- [ ] Gate voltage measured.
- [ ] Phase-node ringing measured.
- [ ] Bootstrap behavior verified.
- [ ] MOSFET temperature checked.
- [ ] Gate resistors reviewed after measurement.

## Stage 5: Sensor Validation

- [ ] Current offset calibration complete.
- [ ] Known-current test complete.
- [ ] Bus-voltage scaling verified.
- [ ] Phase-voltage scaling verified.
- [ ] FET temperature channel verified.
- [ ] Motor temperature channel verified.
- [ ] Brake temperature channel verified.
- [ ] AS5047P communication verified.
- [ ] Encoder direction verified.
- [ ] Encoder diagnostics checked.

## Stage 6: First Motor Operation

- [ ] Small test motor selected and mounted.
- [ ] Low voltage selected: TBD V.
- [ ] Conservative current limit selected: TBD A.
- [ ] Phase order test complete.
- [ ] Encoder electrical offset calibration complete.
- [ ] Open-loop rotation verified.
- [ ] Closed-loop current control enabled at low current.
- [ ] Fault shutdown tested.

## Stage 7: Higher-Power Validation

- [ ] Voltage increased gradually.
- [ ] Current increased gradually.
- [ ] Sustained thermal test completed.
- [ ] Bus transients measured.
- [ ] Regenerative braking tested.
- [ ] Brake chopper tested.
- [ ] CAN-FD command and telemetry tested under load.
- [ ] USB diagnostics tested under load.

## Issues And Deviations

| ID | Stage | Finding | Severity | Resolution | Linked evidence |
|---|---|---|---|---|---|
| TBD | TBD | TBD | TBD | TBD | TBD |

## Release Criteria

- [ ] All required bring-up stages passed or explicitly waived.
- [ ] Known deviations documented.
- [ ] Calibration data stored.
- [ ] Firmware revision tagged or recorded.
- [ ] Hardware revision and unit ID recorded.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
