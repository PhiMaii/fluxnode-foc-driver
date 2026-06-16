# ADC Map

> Working draft. Use this document to record every analog channel, its timing class, scaling, calibration data, and validation evidence for the FluxNode-FOC hardware revision being documented.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| Source files | `README.md`, `firmware/flux-node-foc.ioc`, KiCad schematics |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: ADC channel allocation, injected and regular conversion groups, trigger sources, sampling times, scaling equations, filtering, calibration constants, and validation measurements.
- Out of scope: Final protection thresholds and control-loop behavior, except where they depend directly on ADC measurements.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| MCU ADC pin assignment | `firmware/flux-node-foc.ioc` | Update after CubeMX pin changes. |
| Signal names and routing | KiCad schematic sheets | Cross-check net names before release. |
| Measurement intent | `README.md` sections 8-13 | Current, phase voltage, bus voltage, and temperature philosophy. |

## Current ADC Allocation Summary

| ADC | Conversion type | Channel | MCU pin | Signal label | Measurement | Timing class | Notes |
|---|---|---:|---|---|---|---|---|
| ADC1 | Injected | 12 | PB1 | `DRV_SOB` | Phase current B | PWM synchronous | TBD |
| ADC2 | Injected | 12 | PB2 | `DRV_SOC` | Phase current C | PWM synchronous | TBD |
| ADC3 | Injected | 12 | PB0 | `DRV_SOA` | Phase current A | PWM synchronous | TBD |
| ADC1 | Regular | 2 | PA1 | `SENSE_U_B` | Phase voltage B | TBD | TBD |
| ADC2 | Regular | 1 | PA0 | `SENSE_U_C` | Phase voltage C | TBD | TBD |
| ADC2 | Regular | 9 | PC3 | `SENSE_U_A` | Phase voltage A | TBD | TBD |
| ADC3 | Regular | 5 | PB13 | `SENSE_VBAT` | DC bus voltage | Slow / protection | TBD |
| ADC4 | Regular | 3 | PB12 | `SENSE_TEMP_BRAKE` | Brake temperature | Slow | TBD |
| ADC4 | Regular | 5 | PB15 | `SENSE_USB_VBUS` | USB VBUS sense | Slow | TBD |
| ADC5 | Regular | 1 | PA8 | `SENSE_TEMP_FET` | MOSFET temperature | Slow | TBD |
| ADC5 | Regular | 2 | PA9 | `SENSE_TEMP_MOT` | Motor temperature | Slow | TBD |
| ADC5 | Regular | Internal | Internal | MCU temperature | MCU temperature | Slow | TBD |

## Injected Conversion Timing

| Item | Value | Notes / evidence |
|---|---|---|
| Trigger source | TIM1 TRGO2 | IOC currently uses `ADC_EXTERNALTRIGINJEC_T1_TRGO2`. |
| Trigger point | TBD | Define relative to center-aligned PWM carrier. |
| Sampling time | TBD | IOC currently uses 12.5 cycles for injected current channels. |
| Conversion order | TBD | Confirm generated code and simultaneous ADC behavior. |
| Offset handling | TBD | Define startup offset capture and runtime drift policy. |

## Regular Conversion Timing

| Measurement group | Channels | Rate target | Trigger / scheduling | Notes |
|---|---|---:|---|---|
| Phase voltage | TBD | TBD | TBD | Sample at repeatable PWM point if used for observers. |
| DC bus voltage | TBD | TBD | TBD | Must support overvoltage and brake-chopper decisions. |
| Temperatures | TBD | TBD | TBD | Slow signals; include filtering strategy. |
| USB VBUS | TBD | TBD | TBD | Define use in USB/power-path logic. |

## Scaling And Units

| Signal | Raw ADC unit | Engineering unit | Equation | Component values | Calibration constants |
|---|---|---|---|---|---|
| `DRV_SOA` | counts | A | TBD | shunt/gain TBD | TBD |
| `DRV_SOB` | counts | A | TBD | shunt/gain TBD | TBD |
| `DRV_SOC` | counts | A | TBD | shunt/gain TBD | TBD |
| `SENSE_U_A` | counts | V | TBD | divider/filter TBD | TBD |
| `SENSE_U_B` | counts | V | TBD | divider/filter TBD | TBD |
| `SENSE_U_C` | counts | V | TBD | divider/filter TBD | TBD |
| `SENSE_VBAT` | counts | V | TBD | divider/filter TBD | TBD |
| Temperature channels | counts | deg C | TBD | NTC network TBD | TBD |

## Filtering

- Hardware filters: TBD.
- Firmware filters: TBD.
- Latency budget for protection decisions: TBD.
- Noise/ripple acceptance criteria: TBD.

## Calibration Hooks

- Current-sense offset calibration: see `docs/calibration.md`.
- Voltage-divider gain calibration: see `docs/calibration.md`.
- Temperature calibration: see `docs/calibration.md`.

## Validation Checklist

- [ ] Confirm every ADC channel against KiCad net labels.
- [ ] Confirm CubeMX generated code matches this map.
- [ ] Measure ADC reference voltage.
- [ ] Verify current zero offsets with power stage disabled.
- [ ] Verify current scaling with a known current.
- [ ] Verify bus-voltage scaling at several known input voltages.
- [ ] Verify phase-voltage scaling with PWM disabled and enabled.
- [ ] Verify temperature readings against known temperatures.
- [ ] Record oscilloscope or logic-analyzer evidence for ADC trigger timing.

## Open Questions

- TBD: Final shunt value and current-sense amplifier gain.
- TBD: Final phase-voltage and bus-voltage divider capacitor values.
- TBD: Whether phase-voltage channels use timer-triggered regular conversions or software scheduling.
- TBD: DMA usage for regular ADC groups.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
