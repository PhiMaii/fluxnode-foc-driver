# Timer Map

> Working draft. Use this document to record all STM32 timer assignments, PWM frequencies, trigger outputs, synchronization points, and validation measurements for FluxNode-FOC.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| MCU timer clock | 170 MHz in current IOC |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: TIM1 motor PWM, ADC trigger generation, brake-chopper PWM, fan PWM, timer-derived interrupts, dead time, update timing, and measurement evidence.
- Out of scope: Full control-loop math, except where timing affects execution.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Timer peripheral setup | `firmware/flux-node-foc.ioc` | Update after CubeMX regeneration. |
| Motor-control target | `README.md` sections 1 and 4 | PWM target is 25 kHz. |
| ADC trigger relationship | `docs/adc-map.md` | Keep TIM1 TRGO2 and ADC injected timing aligned. |

## Timer Allocation Summary

| Timer | Channel | Signal | MCU pin | Purpose | Frequency / period | Notes |
|---|---|---|---|---|---:|---|
| TIM1 | CH1 | `DRV_PWM_HC` | PC0 | Motor PWM high-side phase C TBD | 25 kHz target | Complementary pair with CH1N. |
| TIM1 | CH1N | `DRV_PWM_LC` | PC13 | Motor PWM low-side phase C TBD | 25 kHz target | Verify phase naming. |
| TIM1 | CH2 | `DRV_PWM_HB` | PC1 | Motor PWM high-side phase B TBD | 25 kHz target | Complementary pair with CH2N. |
| TIM1 | CH2N | `DRV_PWM_LB` | PB14 | Motor PWM low-side phase B TBD | 25 kHz target | Verify polarity. |
| TIM1 | CH3 | `PWM_DRV_HA` | PC2 | Motor PWM high-side phase A TBD | 25 kHz target | Complementary pair with CH3N. |
| TIM1 | CH3N | `DRV_PWM_LA` | PB9 | Motor PWM low-side phase A TBD | 25 kHz target | Verify naming. |
| TIM1 | CH4 no output | Internal | N/A | ADC trigger compare | TBD | Current IOC uses OC4REF as TRGO2. |
| TIM2 | CH4 | `EXT_FAN` | PB11 | Fan PWM | TBD | IOC period currently 6799. |
| TIM3 | CH4 | `BRAKE_PWM` | PC9 | Brake-chopper PWM | TBD | IOC period currently 6799. |

## TIM1 Motor PWM

| Setting | Current IOC value | Intended value | Notes |
|---|---:|---:|---|
| Counter mode | Center-aligned 1 | TBD | Required for symmetric PWM sampling. |
| Timer clock | 170 MHz | TBD | Confirm generated clock tree. |
| Period | 3399 | TBD | Yields 25 kHz with center-aligned counting at 170 MHz. |
| Dead time | 34 | TBD | Convert to ns and validate at gate. |
| TRGO2 | OC4REF | TBD | Used for injected ADC trigger. |
| OC4 compare | 3299 | TBD | Define exact sample point. |
| PWM idle states | TBD | TBD | Must match DRV8323 input mode. |
| Break input | TBD | TBD | Prefer hardware shutdown where practical. |

## ADC Trigger Timing

| Trigger | Source | Target sample point | Measurement evidence |
|---|---|---|---|
| Injected phase currents | TIM1 TRGO2 / OC4REF | TBD | TBD |
| Phase voltage sampling | TBD | TBD | TBD |
| Bus voltage sampling | TBD | TBD | TBD |

## Fan PWM

| Setting | Value | Notes |
|---|---:|---|
| Timer/channel | TIM2_CH4 | `EXT_FAN` on PB11. |
| Timer clock | 170 MHz current IOC | TBD |
| Period | 6799 current IOC | Calculate final PWM frequency. |
| Duty range | TBD | Define electrical output stage expectations. |
| Default state | TBD | Fan off or fail-safe behavior. |

## Brake-Chopper PWM

| Setting | Value | Notes |
|---|---:|---|
| Timer/channel | TIM3_CH4 | `BRAKE_PWM` on PC9. |
| Timer clock | 170 MHz current IOC | TBD |
| Period | 6799 current IOC | Calculate final PWM frequency. |
| Activation threshold | TBD | See protection design. |
| Default state | Off | README requires brake chopper default off during reset/startup. |
| Maximum duty | TBD | Protect resistor and MOSFET. |

## Timer Interrupt Ownership

| Timer event | ISR owner | Priority | Work allowed in ISR | Notes |
|---|---|---:|---|---|
| TIM1 update / compare | Motor control TBD | TBD | TBD | Fast path only. |
| TIM1 break | Fault manager TBD | TBD | Disable/log fault | TBD |
| TIM2 update | Fan control TBD | TBD | Usually none | Use hardware PWM. |
| TIM3 update | Brake control TBD | TBD | Usually none | Duty updated by slower loop. |

## Validation Checklist

- [ ] Verify TIM1 PWM frequency at MCU pin.
- [ ] Verify complementary polarity at MCU pin.
- [ ] Verify dead time at gate-driver inputs or gates.
- [ ] Verify ADC trigger point relative to PWM waveform.
- [ ] Verify PWM outputs remain safe during reset and startup.
- [ ] Verify fan PWM frequency and duty range.
- [ ] Verify brake PWM frequency, polarity, and default-off behavior.

## Open Questions

- TBD: Final phase naming between TIM1 channels and board phase outputs.
- TBD: Final dead-time value after MOSFET/gate-driver measurement.
- TBD: Whether TIM1 break input is routed and configured.
- TBD: Final fan and brake PWM frequencies.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
