# FluxNode-FOC

FluxNode-FOC is a compact, universal BLDC/FOC motor driver based on an STM32G4 microcontroller. The first hardware revision is intended as a robust development ESC for small to medium BLDC motors, with the 5010 350KV motor used only as an initial test motor.

## Project Goal

The goal is to design a reusable 3-phase FOC ESC that can drive different BLDC motors, not just one specific motor. The design should be suitable for experiments with:

- FOC torque control
- robot actuators
- gimbal-style and drone-style BLDC motors
- compact motor-mounted driver boards
- future daisy-chained power and data distribution

The first revision prioritizes robustness, debugability, and electrical flexibility over absolute minimum size.

## Target Electrical Specification

| Parameter | Target |
|---|---:|
| Nominal DC bus voltage | 24 V |
| Operating voltage range | 8-28 V DC |
| Initial supply source | Bench power supply |
| MOSFET voltage class | 60 V |
| Capacitor voltage rating | preferably 50 V on VBUS |
| Continuous phase current | approx. 5-8 A RMS |
| Peak phase current | approx. 15-20 A |
| PWM frequency | 25 kHz default |
| Optional PWM range | approx. 20-40 kHz |
| PWM mode | 6-PWM |
| Current sensing | 3x low-side shunt |
| Default shunt value | 3 mOhm |
| Optional shunt values | 2 mOhm / 3 mOhm / 5 mOhm |
| PCB stackup | 4-layer PCB recommended |

## Motor Compatibility

The ESC is not designed only for the 5010 350KV test motor. It should be usable with a wider range of small and medium 3-phase BLDC motors, as long as they stay within the voltage, current, and thermal limits of the board.

The 5010 350KV motor is the initial validation motor because it is physically suitable for a compact motor-backpack layout and useful for FOC actuator experiments.

## PWM Frequency

The default PWM frequency should be 25 kHz.

Reasoning:

- 25 kHz is above the typical audible range and should reduce motor whining.
- It is a good compromise between acoustic behavior and switching losses.
- Higher PWM frequencies such as 32 kHz or 40 kHz may be supported in firmware, but they increase MOSFET switching losses and thermal stress.

The hardware should therefore be designed with MOSFETs and a gate driver that can reasonably support 25-40 kHz operation.

## Power Stage

The power stage should use external N-channel MOSFETs driven by a 3-phase gate driver.

Target MOSFET characteristics:

- 60 V VDS rating
- low RDS(on), preferably below roughly 5 mOhm if practical
- moderate gate charge for efficient 25-40 kHz PWM operation
- thermally capable package, preferably 5x6 mm power package such as LFPAK56, PowerPAK SO-8, PDFN 5x6, or similar
- good availability from common distributors

The design should avoid choosing MOSFETs only by lowest RDS(on). Gate charge, switching losses, package thermals, and layout practicality are also important.

## Gate Driver

The preferred architecture is a 3-phase smart gate driver for external MOSFETs.

Required or strongly preferred features:

- support for 3-phase BLDC power stage
- support for 6-PWM control from the STM32
- suitable for a 24 V nominal / 60 V class design
- bootstrap high-side drive
- fault output to the MCU
- undervoltage lockout
- overcurrent or short-circuit protection
- compatibility with 3-shunt low-side current sensing
- operation at 25 kHz PWM, with margin for higher PWM frequencies

## Current Sensing

The ESC should use three low-side shunts, one per phase leg.

The shunts should be treated as configurable assembly options:

| Shunt value | Use case |
|---:|---|
| 5 mOhm | smaller motors, better current resolution |
| 3 mOhm | default universal value |
| 2 mOhm | higher current range, lower loss |

Recommended default:

- 3 mOhm
- 1% tolerance
- current-sense type resistor
- preferably Kelvin / 4-terminal style
- 2512 or comparable high-power package
- at least 1 W, preferably 2 W depending on selected part

Sense routing should use Kelvin connections from each shunt to the current-sense amplifier or MCU analog front-end.

Suggested signal names:

```text
R_SHUNT_U
R_SHUNT_V
R_SHUNT_W

ISENSE_U_P
ISENSE_U_N
ISENSE_V_P
ISENSE_V_N
ISENSE_W_P
ISENSE_W_N
```

## Brake Chopper Option

The PCB should include an optional brake chopper circuit as a DNP / optional assembly feature.

A brake chopper protects the DC bus when the motor regenerates energy during braking or external motion. This is especially important when using a bench power supply, because many bench supplies cannot absorb reverse current. In that case, regenerated energy can raise VBUS and damage the MOSFETs, capacitors, gate driver, regulator, or other parts.

The brake chopper should dump excess energy into an external braking resistor when VBUS rises above a firmware-defined threshold.

Preferred topology:

```text
VBUS+ ---- external braking resistor ---- BRK-
                                             |
                                      Drain Q_BRK
                                      Source Q_BRK
                                             |
                                            GND
```

When the brake MOSFET turns on, current flows:

```text
VBUS+ -> braking resistor -> brake MOSFET -> GND
```

Recommended implementation:

- low-side N-MOSFET brake switch
- external braking resistor connector
- gate driven by STM32 GPIO or PWM output
- gate resistor, e.g. 10-47 ohm
- gate pulldown, e.g. 100 kOhm
- VBUS measurement through ADC
- optional hardware comparator in a later revision
- TVS diode on VBUS still required for fast transients

Suggested signal and part names:

```text
VBUS_SENSE
BRAKE_GATE
BRK+
BRK-

Q_BRK
R_BRK_G
R_BRK_PD
J_BRK
D_TVS
```

The braking resistor does not need to be placed on the PCB. A connector for an external resistor is preferred because braking resistors can dissipate significant heat.

Example braking resistor values for testing at approximately 30 V bus voltage:

| Resistor | Current | Power |
|---:|---:|---:|
| 100 ohm | 0.3 A | 9 W |
| 47 ohm | 0.64 A | 19 W |
| 22 ohm | 1.36 A | 41 W |
| 10 ohm | 3 A | 90 W |

For early tests, an external resistor around 47-100 ohm with sufficient power rating can be used.

## Protection Features

The design should include or reserve support for:

- gate driver fault signal to MCU
- VBUS voltage measurement
- undervoltage handling in firmware
- overvoltage detection in firmware
- overcurrent protection
- TVS diode across VBUS and GND
- NTC temperature sensor near the MOSFETs
- sufficient VBUS bulk capacitance close to the power stage
- careful startup current limiting in firmware

The TVS diode is not a replacement for the brake chopper. The TVS handles short voltage spikes, while the brake chopper handles longer regenerative braking events.

## Bench Power Supply Testing Notes

Initial testing should be done conservatively:

1. Start at 12 V, not 24 V.
2. Use a low current limit, for example 1-2 A.
3. Test first without load.
4. Verify gate signals before enabling high current operation.
5. Verify current-sense readings before closed-loop FOC.
6. Increase voltage and current limits gradually.
7. Be careful with braking or externally spinning the motor, because the bench supply may not absorb regenerated energy.

## PCB Design Direction

The board should use a 4-layer stackup.

Recommended direction:

- Layer 1: signal and power components
- Layer 2: solid GND plane
- Layer 3: power distribution, VBUS, possibly additional GND areas
- Layer 4: signals and secondary routing

Layout priorities:

- compact MOSFET half-bridge layout
- short gate traces
- tight high-current loops
- low-inductance VBUS decoupling near MOSFETs
- Kelvin routing for shunts
- separation between noisy power stage and sensitive analog/current-sense signals
- large copper areas and thermal vias for MOSFET heat spreading
- NTC located close to the MOSFETs

## Current Design Decision Summary

The current baseline specification is:

> Universal 24 V class FOC ESC, 8-28 V input, 8 A RMS continuous phase current target, 20 A peak phase current target, 60 V MOSFETs, 25 kHz default PWM, 6-PWM control, three low-side configurable shunts, optional brake chopper, VBUS sensing, TVS protection, and MOSFET temperature sensing.

