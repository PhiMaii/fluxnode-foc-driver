# Calibration

> Working draft. Use this document to define repeatable calibration procedures and storage format for FluxNode-FOC current, voltage, temperature, and rotor-position measurements.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| Calibration data format | TBD |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: current offsets, current gain, voltage-divider gain, temperature conversion, encoder zero/electrical offset, phase order, and calibration persistence.
- Out of scope: Motor parameter identification, controller gain tuning, and production-line automation details unless they directly affect calibration data.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Analog channel map | `docs/adc-map.md` | Keep signal names consistent. |
| Startup usage | `docs/startup-sequence.md` | Define what must be calibrated before enabling PWM. |
| Hardware values | KiCad schematic / BOM | Fill exact shunt, divider, and NTC values. |

## Calibration Data Model

| Field | Unit | Type / range | Default | Required before run | Notes |
|---|---|---|---|---|---|
| Current offset A | ADC counts or A | TBD | TBD | Yes | Phase A / `DRV_SOA`. |
| Current offset B | ADC counts or A | TBD | TBD | Yes | Phase B / `DRV_SOB`. |
| Current offset C | ADC counts or A | TBD | TBD | Yes | Phase C / `DRV_SOC`. |
| Current gain A | A/count | TBD | TBD | TBD | Optional per-phase gain correction. |
| Current gain B | A/count | TBD | TBD | TBD | Optional per-phase gain correction. |
| Current gain C | A/count | TBD | TBD | TBD | Optional per-phase gain correction. |
| Bus-voltage gain | V/count | TBD | TBD | Yes | Used for protection. |
| Phase-voltage gains | V/count | TBD | TBD | TBD | One value per phase if needed. |
| Temperature coefficients | TBD | TBD | TBD | TBD | NTC conversion data. |
| Encoder zero | mechanical/electrical angle | TBD | TBD | Yes | Define convention. |
| Phase order | enum | TBD | TBD | Yes | Define motor wiring convention. |

## Current Offset Calibration

### Preconditions

- [ ] Motor phase outputs disabled.
- [ ] Gate driver disabled or high impedance as required.
- [ ] No commanded current.
- [ ] ADC reference and VREF stable.
- [ ] Board temperature recorded.

### Procedure

1. TBD: Configure ADC injected conversions.
2. TBD: Collect samples per phase.
3. TBD: Reject outliers or saturated samples.
4. TBD: Calculate mean offset for each phase.
5. TBD: Store offsets and timestamp.
6. TBD: Verify corrected current is near zero.

### Acceptance Criteria

| Check | Limit | Result |
|---|---:|---|
| Offset noise RMS | TBD | TBD |
| Corrected zero-current error | TBD | TBD |
| Phase-to-phase offset consistency | TBD | TBD |

## Current Gain Calibration

| Step | Description | Expected result | Evidence |
|---|---|---|---|
| 1 | Apply known current or use calibrated load. | TBD | TBD |
| 2 | Record ADC readings. | TBD | TBD |
| 3 | Calculate gain correction. | TBD | TBD |
| 4 | Verify corrected current. | TBD | TBD |

## Voltage Calibration

### DC Bus Voltage

- Divider values: TBD.
- ADC channel: see `docs/adc-map.md`.
- Test voltages: TBD.
- Fit method: TBD.

| Applied voltage | Raw ADC | Calculated voltage | Error | Pass |
|---:|---:|---:|---:|---|
| TBD | TBD | TBD | TBD | [ ] |

### Phase Voltages

- Divider values: TBD.
- Filter values: TBD.
- Test method with PWM disabled: TBD.
- Test method with PWM enabled: TBD.

## Temperature Calibration

| Channel | Sensor type | Divider values | Conversion equation | Validation points |
|---|---|---|---|---|
| FET temperature | TBD | TBD | TBD | TBD |
| Motor temperature | TBD | TBD | TBD | TBD |
| Brake temperature | TBD | TBD | TBD | TBD |
| MCU internal temperature | Internal | N/A | TBD | TBD |

## Encoder And Phase Calibration

### Encoder Communication Check

- [ ] SPI transaction format confirmed.
- [ ] AS5047P angle read confirmed.
- [ ] Diagnostics read and decoded.
- [ ] Parity/error handling confirmed.

### Electrical Offset

1. TBD: Lock rotor or command alignment current.
2. TBD: Read encoder angle.
3. TBD: Determine electrical offset.
4. TBD: Validate with low-current rotation.
5. TBD: Store offset with motor identity.

### Phase Order And Direction

| Test | Expected | Actual | Result |
|---|---|---|---|
| Open-loop positive angle command | TBD | TBD | TBD |
| Encoder angle direction | TBD | TBD | TBD |
| Corrected FOC direction | TBD | TBD | TBD |

## Persistence And Versioning

- Storage location: TBD.
- Data version field: TBD.
- CRC/checksum: TBD.
- Factory reset behavior: TBD.
- Per-board versus per-motor data separation: TBD.

## Recalibration Triggers

- [ ] New PCB assembly.
- [ ] Current-sense component replacement.
- [ ] Encoder or magnet moved.
- [ ] Motor changed.
- [ ] Firmware calibration format changed.
- [ ] Temperature drift exceeds TBD.
- [ ] Validation check fails during startup.

## Open Questions

- TBD: Where calibration data is stored in nonvolatile memory.
- TBD: Whether current gain is calibrated per board or derived from BOM values.
- TBD: How motor-specific calibration is associated with a controller.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
