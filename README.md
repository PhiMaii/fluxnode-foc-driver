# FluxNode-FOC

FluxNode-FOC is a compact, general-purpose BLDC motor controller designed for field-oriented control in robotics, actuators, experimental drives, and motor-integrated applications.

The project combines a high-performance STM32G4 microcontroller, a three-phase MOSFET power stage, three-phase current sensing, rotor position sensing, phase-voltage measurement, CAN-FD communication, USB, temperature monitoring, and an optional brake chopper.

The design is intended to be understandable, repairable, and extensible rather than optimized only for minimum cost or minimum PCB area.

> **Project status:** Hardware and firmware architecture are still under development. Electrical values, component selections, interfaces, and PCB details may change before the first validated hardware revision.

---

## 1. Main design targets

| Parameter | Target |
|---|---:|
| Battery voltage | Up to 8S Li-ion/LiPo |
| Maximum normal battery voltage | 33.6 V |
| Design measurement range | Up to approximately 40 V |
| Nominal development voltage | 24 V |
| Continuous phase current target | Approximately 8 A RMS |
| Peak current target | Approximately 20 A |
| PWM frequency | 25 kHz |
| Control method | Field-oriented control |
| Power MOSFET voltage rating | 60 V |
| Bulk capacitor voltage rating | 50–63 V |
| Main communication interface | CAN-FD |
| MCU | STM32G474RE |
| Gate driver | DRV8323SRTAR |
| Main rotor encoder | AS5047P |
| PCB stackup | 6 layers preferred |

The controller is designed around **8S operation**, not 10S. A fully charged 8S battery reaches 33.6 V. Additional margin is required for wiring inductance, regenerative braking, switching transients, and load-disconnect events.

The system may be operated from lower voltages, including a 24 V laboratory supply, as long as the supply is compatible with regenerative energy or appropriate braking and overvoltage protection are provided.

---

## 2. Design philosophy

The main design priorities are:

1. Reliable and accurately timed current measurement for FOC.
2. Low-inductance power-stage layout.
3. Sufficient voltage margin for an 8S battery system.
4. Good observability through current, voltage, temperature, and diagnostic measurements.
5. Expandability for different motors, sensors, and actuator configurations.
6. Firmware that is understandable and not tightly coupled to a single framework.
7. Practical assembly and debugging with commonly available tools.

The design intentionally includes features that are not strictly required for basic FOC, such as phase-voltage sensing, several temperature channels, CAN-FD, an auxiliary sensor interface, and an optional brake chopper. These features make the controller more useful as a development platform and provide additional diagnostic and protection options.

---

## 3. System architecture

The controller is divided into the following main functional blocks:

- Power input and protection
- Auxiliary power supplies
- STM32G474 control section
- DRV8323 gate driver
- Three-phase MOSFET bridge
- Three-phase current measurement
- Phase-voltage and DC-bus voltage measurement
- Rotor-position sensing
- Temperature monitoring
- CAN-FD and USB communication
- Optional external sensor interfaces
- Optional brake chopper
- Fan and debug outputs

The KiCad schematic should remain split into logical hierarchical sheets rather than placing the entire design on one sheet.

Suggested schematic sheets:

```text
Power Input
Power Supplies
MCU Core
Gate Driver
Power Stage
Current Sense
Voltage Sense
Encoders and Sensors
CAN and USB
Temperature and Fan
Brake Chopper
Debug and Expansion
```

---

## 4. Microcontroller

The selected MCU is the **STM32G474RE**, based on the STM32G4 family.

Reasons for selecting this MCU include:

- High-performance Cortex-M4 core
- Floating-point unit
- Advanced motor-control timers
- Multiple fast ADCs
- Internal operational amplifiers and comparators
- CORDIC and FMAC accelerators
- Multiple SPI peripherals
- FDCAN support
- USB device support
- DAC outputs
- Good support through STM32CubeMX and STM32Cube firmware packages

The STM32G474xB/xC/xE datasheet is the reference MCU datasheet for this project.

### Motor-control timer

TIM1 is reserved for the three-phase complementary PWM outputs.

The current pin allocation uses:

- TIM1 channel 2 and channel 2N
- TIM1 channel 3 and channel 3N
- TIM1 channel 4 and channel 4N

TIM1 channel 4 on the selected STM32G474 package has a complementary output and can therefore be used as one of the three half-bridge PWM channels.

TIM1 should be configured for:

- Center-aligned PWM
- Complementary outputs
- Programmable dead time
- Break input support
- 25 kHz switching frequency
- ADC trigger generation at defined points in the PWM cycle

The exact output polarity and idle states must match the DRV8323 input configuration.

---

## 5. Gate driver

The selected gate driver is the **DRV8323SRTAR**.

It provides:

- Three half-bridge gate-drive channels
- SPI configuration and diagnostics
- Adjustable gate-drive strength
- Current-shunt amplifier support
- Fault reporting
- Undervoltage and overcurrent protection features

The gate driver is connected to the STM32 through:

- Six PWM signals
- SPI clock, COPI, CIPO, and chip select
- Enable signal
- nFAULT signal
- Optional additional diagnostic signals
- Analog VREF generated by an MCU DAC

### Gate-driver SPI

The gate driver shares SPI1 with the onboard magnetic encoder. Each device has a separate chip-select signal and is accessed sequentially.

This sharing is acceptable because neither device requires continuous full-bandwidth SPI traffic. Motor-control timing must not depend on slow or blocking SPI transactions.

The external auxiliary SPI connector uses SPI3 so that optional external hardware does not disturb the main gate-driver and encoder bus.

### nFAULT

The gate-driver fault output should be connected to an interrupt-capable GPIO input.

Software should log and decode the driver fault registers, but hardware protection must not rely only on software response time. Where practical, the timer break input should be used for immediate PWM shutdown.

### VREF

The DRV8323 current-sense amplifier reference is generated by a DAC output from the STM32 rather than being permanently tied to 3.3 V.

This allows the current-sense zero point to be adjusted in firmware. The DAC output must be properly decoupled and kept quiet because noise on VREF directly affects all current measurements.

---

## 6. Power stage

The three-phase inverter uses six external N-channel MOSFETs.

The currently selected phase MOSFET is:

- **TPW1R306PL**

The design uses 60 V MOSFETs to provide useful margin above the 33.6 V maximum battery voltage. This margin is still not sufficient by itself to tolerate uncontrolled regenerative voltage rise or poor PCB layout, so transient suppression and bus-voltage monitoring remain necessary.

### Power-stage layout priorities

The following loops must be kept as small as possible:

- High-side MOSFET, low-side MOSFET, and local ceramic/bulk capacitor commutation loop
- Gate-driver source and sink loops
- Bootstrap capacitor loops
- Current-shunt measurement loops

The gate driver and gate resistors must be placed close to the MOSFETs. Gate traces should be short, direct, and separated from high-current switching nodes.

Kelvin source connections should be used where practical for gate-drive return and current measurement.

The phase nodes are high-dV/dt switching nodes and should have limited copper area. They should not run beneath sensitive analog circuits, the encoder, the crystal, USB, or communication interfaces.

---

## 7. Power input and voltage protection

The input stage is designed for an 8S battery or a suitable DC supply.

Required or planned protection includes:

- Reverse-polarity protection where practical
- Input fuse or external fused supply
- TVS diode or equivalent transient suppression
- Local ceramic decoupling close to the bridge
- 50–63 V rated bulk capacitors
- DC-bus voltage measurement
- Firmware overvoltage and undervoltage limits
- Optional brake chopper
- Consideration of regenerative current into the source

The exact TVS voltage must be selected together with the maximum allowed bus voltage, MOSFET rating, gate-driver rating, and expected transient energy. It must not clamp during normal 8S operation but must act before the power stage is exposed to unsafe voltage.

A bench supply may not be able to absorb regenerative current. Motor deceleration can therefore raise the bus voltage even when the controller is powered from a regulated supply.

---

## 8. Current sensing

The design measures all three phase currents.

Each phase has its own shunt measurement path, allowing:

- Three-phase current reconstruction
- More flexible sampling
- Better diagnostics
- Detection of measurement inconsistencies
- Improved operation in modulation regions where two-shunt measurement becomes difficult

The phase-current channels are distributed across separate ADCs where possible so that they can be sampled simultaneously or with minimal timing skew.

The current measurements are configured as injected ADC conversions triggered by TIM1. Sampling should occur at a defined low-noise point in the PWM cycle.

Important requirements:

- Use differential/Kelvin routing from each shunt.
- Keep current-sense traces away from phase nodes and gate traces.
- Place input filtering near the receiving amplifier or ADC input as appropriate.
- Keep the three channels electrically symmetrical.
- Calibrate current offsets before enabling the power stage.
- Recheck offsets when temperature or VREF changes significantly.
- Detect ADC saturation and implausible current combinations.

Phase-current ADCs should remain the highest-priority analog measurements. Slower signals such as temperature and battery voltage should use regular conversions or lower-priority ADC resources.

---

## 9. Phase-voltage sensing

All three motor phases are measured through resistor dividers.

Phase-voltage sensing is not required for basic encoder-based FOC, but it provides several useful capabilities:

- Back-EMF observation
- Sensorless experimentation
- Motor-state diagnostics
- Phase-disconnection detection
- Verification of switching behavior
- Improved startup and fault analysis
- Potential observer development

The chosen divider concept uses two series high-side resistors and one low-side resistor to distribute voltage stress and improve routing.

The currently considered common value set is:

```text
High side: 2 × 51 kΩ
Low side: 6.8 kΩ
```

This ratio provides margin for phase and bus voltage excursions near 40 V while keeping the ADC voltage within the 3.3 V domain.

The final divider values must be verified against:

- ADC absolute maximum voltage
- Resistor tolerance
- Maximum expected transient voltage
- ADC input leakage
- Sampling-time requirements
- Desired measurement bandwidth

A small capacitor at the ADC-side divider node provides anti-aliasing and switching-noise filtering. A value around **1 nF** is currently considered for the phase-voltage channels, subject to final simulation and measurement.

The divider resistors should be placed close to the phase-sense pickup, while the final filter capacitor and any small series protection resistor should be placed close to the MCU ADC input. Routing between the divider and MCU must avoid switching nodes.

The phase voltages should be sampled from a timer-derived trigger at a repeatable point in the PWM cycle. They do not necessarily need the same timing priority as the phase currents.

---

## 10. DC-bus voltage sensing

The battery/DC-bus voltage is measured by an external resistor divider connected to an ADC channel.

The MCU VBAT pin is not used for measuring the motor supply. The external motor bus can reach approximately 40 V during transients and requires a dedicated divider.

The bus-voltage channel is used for:

- Undervoltage protection
- Overvoltage protection
- Brake-chopper control
- Regenerative-braking management
- Power and energy estimation
- Supply diagnostics

The same general divider ratio as the phase-voltage measurement may be used to simplify the bill of materials:

```text
High side: 2 × 51 kΩ
Low side: 6.8 kΩ
```

The bus voltage changes much more slowly than the phase-node voltage, so it should use stronger filtering than the phase-voltage channels. The final capacitor value should be chosen based on the desired brake-chopper response and ADC sampling rate.

The divider must be connected directly to the actual power-stage DC bus rather than to a remote or lightly loaded supply node.

---

## 11. Rotor-position sensing

The main onboard rotor encoder is the **AS5047P**.

It is connected by SPI and is intended to provide the rotor angle used by the FOC algorithm.

Reasons for choosing the AS5047P include:

- 14-bit absolute angle output
- High-speed SPI interface
- Suitability for motor-control applications
- On-chip dynamic angle error compensation
- Diagnostic information
- Common use in compact BLDC actuators

The encoder shares SPI1 with the DRV8323, using a separate chip-select signal.

Encoder transactions should be:

- Short
- Deterministic
- Checked for communication and parity errors
- Scheduled so they do not interfere with the current-control interrupt

DMA may be added later if it measurably improves timing or CPU load, but it is not required for initial firmware bring-up.

The PCB must respect the encoder manufacturer's magnet alignment, air-gap, magnetic-field, and keepout recommendations.

An additional external sensor interface is available through SPI3. This may be used for:

- A second encoder
- An off-axis sensor
- An external actuator sensor
- Experimental sensors

The external sensor is optional and not required for initial operation.

---

## 12. ADC allocation philosophy

The STM32G474 provides multiple ADCs, which are used to separate timing-critical and slow measurements.

The allocation philosophy is:

- Distribute the three phase-current measurements across ADC1, ADC2, and ADC3 where possible.
- Use injected conversions triggered by TIM1 for phase currents.
- Place phase-voltage measurements on available ADC channels, even when one ADC must contain two phase-voltage channels.
- Use regular conversions for bus voltage and temperature measurements.
- Use ADC4 and ADC5 for lower-priority auxiliary measurements where convenient.
- Avoid sacrificing correct phase-current timing merely to make the ADC channel allocation look symmetrical.

Current and phase-voltage measurements may share an ADC if the injected and regular sequences are configured carefully. The timing-critical current conversion always has priority.

---

## 13. Temperature monitoring

Planned temperature measurements include:

- Power MOSFET temperature
- Motor temperature
- Brake-chopper resistor or chopper-stage temperature
- MCU internal temperature

External temperatures are measured with NTC resistor dividers connected to ADC inputs.

Temperature monitoring is used for:

- Warning thresholds
- Current derating
- Fan control
- Controlled shutdown
- Fault logging

The MCU internal temperature sensor can be sampled in addition to the external channels. It is useful for board-level thermal monitoring but does not replace a sensor close to the MOSFETs.

NTC channels are slow signals and do not require timer-synchronous sampling.

---

## 14. Brake chopper

An optional brake chopper is included to prevent excessive DC-bus voltage during regenerative braking.

The brake chopper consists of:

- A power MOSFET
- Gate-drive circuitry
- An external or onboard braking resistor connection
- Bus-voltage feedback
- Temperature monitoring where practical
- Firmware control and fault limits

The braking resistor may be implemented as an external add-on connected through a dedicated power connector. This avoids placing a large, hot resistor on the compact controller PCB.

The chopper is controlled from a timer PWM output. It is currently assigned separately from the fan PWM so that both functions can operate independently.

The chopper is activated based on the measured DC-bus voltage. Typical firmware behavior will include:

1. Chopper remains off below the activation threshold.
2. PWM duty increases when the bus exceeds the target voltage.
3. Duty is reduced as the bus voltage falls.
4. A higher emergency threshold disables motor PWM if the chopper cannot control the bus.
5. Temperature and maximum duty limits protect the MOSFET and resistor.

The exact voltage thresholds depend on the final TVS selection, capacitor rating, MOSFET margin, and maximum permitted bus voltage.

The brake chopper must default to off during reset and startup.

---

## 15. Communication interfaces

### CAN-FD

CAN-FD is the main external real-time communication interface.

It is intended for:

- Commands and setpoints
- Telemetry
- Fault reporting
- Parameter configuration
- Multi-axis systems
- Daisy-chain-style wiring between actuator nodes

Electrical CAN wiring is a bus, not a logical point-to-point daisy chain. Connectors may provide CAN in and CAN out for convenient physical chaining, but all nodes share the same differential bus.

The controller should include:

- CAN transceiver
- CANH and CANL routing as a differential pair
- Optional switchable termination
- ESD protection near the connector
- Ground reference
- Suitable connector pinout for repeated nodes

Termination should only be enabled at the two physical ends of the CAN bus.

### USB-C

USB-C provides a local interface for:

- Firmware development
- Configuration
- Diagnostics
- Bootloader or device firmware update
- High-rate local telemetry where useful

USB is not intended to power the motor stage.

The USB section includes:

- USB 2.0 full-speed D+ and D−
- 22 Ω series resistors close to the MCU
- ESD protection close to the connector
- Correct USB-C CC resistors for a device
- VBUS sensing
- Protection against unwanted back-powering when USB and the motor battery are connected simultaneously

USB 5 V and the board's main supply must not be directly tied together without a deliberate power-path design.

---

## 16. Auxiliary interfaces

The board includes optional external interfaces for future sensors and accessories.

Planned interfaces include:

- SPI3 auxiliary sensor port
- I2C sensor port
- Fan connector
- External brake-resistor connector
- SWD debugging connector
- Debug LEDs
- Optional spare GPIO or timer signals where routing permits

The external SPI port is separate from the main SPI1 bus. This prevents an optional cable-connected device from adding excessive capacitance or noise to the gate-driver and onboard-encoder interface.

I2C is intended for relatively slow optional sensors. Pull-up resistors should be configurable or removable so that multiple boards or sensor modules do not unintentionally place too many pull-ups in parallel.

---

## 17. Fan control

An external fan can be controlled through a timer PWM output.

The fan output is independent of the brake-chopper PWM.

Firmware may control fan speed based on:

- MOSFET temperature
- Motor current
- Board temperature
- Brake-chopper activity
- User configuration

The MCU pin must not directly drive a fan motor. A suitable transistor or MOSFET stage, flyback protection where required, and connector decoupling must be provided.

---

## 18. Debugging and programming

The primary programming and debugging interface is SWD.

Required signals include:

- SWDIO
- SWCLK
- NRST
- GND
- Target reference voltage
- Optional SWO

SWO should be routed if pin availability allows it, because it can provide low-overhead debug output and timing information.

Boot configuration and reset access should remain available during bring-up.

At least two debug LEDs are planned for basic status and fault indication.

---

## 19. PCB stackup and layout

A **6-layer PCB** is preferred.

Although a 4-layer design is technically possible, the small additional fabrication cost of six layers is justified by the complexity of the mixed-signal motor-control board.

A possible six-layer stackup is:

```text
L1: Components, short signals, power-stage copper
L2: Solid ground plane
L3: Power distribution
L4: Secondary power and slower signals
L5: Solid ground plane
L6: Signals and low-current components
```

The exact stackup must be selected from the PCB manufacturer's controlled stackup options.

### Reasons for using six layers

- Better uninterrupted ground references
- Easier separation of power and sensitive analog routing
- Improved return-current paths
- More practical routing around the LQFP MCU
- Better EMI performance
- Easier CAN, USB, SPI, and ADC routing
- Reduced need to split or perforate the main ground plane

### Ground strategy

The design should use a continuous ground system rather than creating isolated analog and digital ground islands.

Sensitive analog circuits should be separated by placement and return-current control, not by cutting the ground plane.

High-current power returns must not share narrow copper paths with:

- ADC reference returns
- Current-sense returns
- Encoder ground
- USB ground
- MCU decoupling returns

Use stitching vias around noisy power areas, board edges where appropriate, and layer transitions.

### Placement order

Recommended placement order:

1. MOSFET bridge and power connectors
2. DC-link capacitors
3. Gate driver and gate components
4. Current shunts and sense paths
5. MCU and decoupling
6. Encoder
7. Power supplies
8. CAN and USB
9. Auxiliary connectors
10. Debug and low-priority circuitry

The power-stage layout should be completed before general digital routing.

---

## 20. Component sizes and assembly

The board uses a mixture of 0402 and 0603 passives.

General preference:

- Use 0402 where density or very low parasitic inductance matters.
- Use 0603 for resistor dividers, manually reworked parts, and components exposed to higher voltage.
- Use appropriately larger packages for high-power, high-voltage, or high-capacitance components.
- Do not select 0402 only to reduce board area if it makes probing and rework unnecessarily difficult.

For voltage dividers connected to the motor bus or phase nodes, 0603 resistors are preferred. Splitting the high-side resistance between two components reduces the voltage across each resistor and improves layout flexibility.

The PCB is intended to be assembled with solder paste, hot plate or reflow, hot air, flux, and hand rework where necessary. 0402 assembly is possible but requires careful paste control and inspection.

---

## 21. Firmware workflow

The firmware is generated initially with **STM32CubeMX** and developed in **Visual Studio Code**.

PlatformIO is not used for this project.

The intended workflow is:

1. Configure pins, clocks, ADCs, timers, DMA, communication peripherals, and middleware in STM32CubeMX.
2. Generate the STM32 project and initialization code.
3. Open and develop the generated project in Visual Studio Code.
4. Keep application code outside generated sections where possible.
5. Regenerate from CubeMX without overwriting project logic.
6. Build and debug using the selected ARM GCC/CMake or STM32-compatible toolchain.

The firmware will initially use STM32 HAL and LL drivers where appropriate. Time-critical motor-control code may use LL access or direct register operations after the basic system is validated.

### Firmware language

CubeMX generates C initialization code. The main firmware may remain primarily C, with carefully introduced C++ modules if they offer a clear architectural benefit.

The project should not depend on CubeMX generating native C++ code. C-generated peripheral initialization can be called from C++ through a compatible interface.

### Real-time priorities

Highest-priority operations include:

- PWM update
- Current sampling
- Current-control loop
- Hardware fault response

Lower-priority operations include:

- Encoder communication
- CAN processing
- USB communication
- Temperature conversion
- Logging
- Configuration storage
- User-interface functions

Blocking operations must not be used inside the fast control interrupt.

FreeRTOS is not required for the initial motor-control implementation. It may be evaluated later if the communication and application layers become complex enough to justify it.

---

## 22. Suggested repository structure

The project uses a single repository containing hardware, firmware, documentation, and tools.

```text
FluxNode-FOC/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── architecture/
│   ├── calculations/
│   ├── bring-up/
│   ├── measurements/
│   └── images/
├── hardware/
│   ├── kicad/
│   │   ├── FluxNode-FOC.kicad_pro
│   │   ├── FluxNode-FOC.kicad_sch
│   │   ├── FluxNode-FOC.kicad_pcb
│   │   └── sheets/
│   ├── libraries/
│   │   ├── symbols/
│   │   ├── footprints/
│   │   └── 3dmodels/
│   ├── production/
│   │   ├── gerbers/
│   │   ├── bom/
│   │   ├── pick-and-place/
│   │   └── assembly/
│   └── datasheets/
├── firmware/
│   ├── cubemx/
│   ├── Core/
│   ├── Drivers/
│   ├── App/
│   ├── MotorControl/
│   ├── Communication/
│   ├── BSP/
│   ├── Tests/
│   └── tools/
├── mechanical/
│   ├── cad/
│   ├── drawings/
│   └── exports/
└── scripts/
```

A monorepo is preferred because hardware and firmware revisions are tightly coupled. A tag or release can therefore identify the exact PCB, schematic, firmware, configuration, and documentation that belong together.

Generated production files should be placed in clearly versioned release folders or attached to Git releases rather than continuously overwriting unversioned files.

Large vendor libraries should not be copied into the repository unless their license permits it and they are required for reproducible builds.

---

## 23. Naming and revision conventions

Suggested conventions:

- Repository: `FluxNode-FOC`
- Firmware modules: lowercase or project-standard snake_case
- PCB revision: `REV_A`, `REV_B`, and so on
- Firmware semantic version: `v0.1.0`, `v0.2.0`, and so on
- Hardware-specific firmware configuration: explicitly tied to a PCB revision
- Nets: descriptive uppercase names such as `VBUS`, `PHASE_A`, `ISENSE_A`, and `DRV_NFAULT`

Each manufactured PCB revision should have a visible revision marking on the silkscreen.

Firmware should check or store the expected hardware revision where practical.

---

## 24. Bring-up strategy

Initial bring-up should be performed in controlled stages.

### Stage 1: Unpowered checks

- Visual inspection
- Resistance checks between all power rails and ground
- Verify MOSFET orientation
- Verify gate-driver and MCU pin orientation
- Check for solder bridges
- Check shunt and divider values
- Confirm connector polarity

### Stage 2: Low-voltage control power

- Use a current-limited supply
- Verify all regulated rails
- Confirm MCU programming and SWD
- Check reset and clock operation
- Test debug LEDs
- Verify USB without motor power

### Stage 3: Gate driver without motor operation

- Read and write SPI registers
- Confirm nFAULT behavior
- Verify DAC-generated VREF
- Observe PWM outputs with the bridge disabled
- Verify dead time and complementary polarity

### Stage 4: Power-stage switching

- Use a low DC-bus voltage
- Use strict current limiting
- Test one half bridge at a time where practical
- Observe gate voltage, switch node, and ringing
- Verify bootstrap operation
- Tune gate resistors only after measurements

### Stage 5: Sensor validation

- Calibrate current offsets
- Inject known current where possible
- Verify bus and phase-voltage scaling
- Check all NTC channels
- Verify encoder direction, zero, and diagnostics

### Stage 6: First motor operation

- Use a small motor and low bus voltage
- Start with limited voltage and current commands
- Validate phase order
- Validate encoder direction and electrical offset
- Test open-loop rotation before closed-loop current control
- Keep overcurrent and overvoltage limits conservative

### Stage 7: Full-power validation

- Increase voltage and current gradually
- Measure MOSFET and gate-driver temperature
- Validate bus transients
- Test regenerative braking
- Validate brake-chopper behavior
- Perform sustained thermal tests

---

## 25. Safety

This project controls motors from battery voltages and currents capable of causing burns, fire, component explosion, mechanical injury, and battery damage.

Development precautions include:

- Use a current-limited power source during bring-up.
- Add an appropriate fuse.
- Keep a physical emergency disconnect accessible.
- Secure the motor mechanically.
- Do not touch the board while powered.
- Use differential or isolated probing techniques for switching-node measurements.
- Do not connect an oscilloscope ground clip to a non-ground switching node.
- Treat lithium batteries as high-energy sources.
- Verify braking behavior before high-speed operation.
- Test overvoltage protection before relying on regenerative braking.

The controller is an experimental design and is not certified for safety-critical use.

---

## 26. Open design decisions

The following items are not yet final and must be confirmed before release:

- Final CAN transceiver and connector pinout
- Final auxiliary power-supply topology
- Final TVS diode and overvoltage thresholds
- Exact current-shunt resistance and power rating
- Current-sense amplifier gain and filtering
- Final voltage-divider capacitor values
- Brake-chopper MOSFET, gate driver, resistor rating, and connector
- Final fan voltage and output stage
- Exact ADC channel allocation
- Final SPI clock rates
- DMA usage for encoder and communication peripherals
- USB device class
- Final board dimensions and mounting pattern
- Final six-layer manufacturer stackup
- Exact thermal limits and current-derating curve
- Production test-point set
- Bootloader and firmware-update strategy

These items should be moved out of this section once they are validated and formally adopted.

---

## 27. Documentation policy

Important design decisions should be documented in the repository rather than existing only in chat logs or schematic comments.

For every major decision, record:

- What was selected
- Why it was selected
- Alternatives considered
- Important calculations
- Validation measurements
- Any revision-specific limitations

Hardware calculations, oscilloscope captures, thermal tests, and bring-up notes should be stored under `docs/`.

Datasheets should either be linked from documentation or stored under `hardware/datasheets/` when redistribution is permitted.

---

## 28. License

A license has not yet been finalized.

Before publishing the repository, choose separate or compatible licenses for:

- Hardware design files
- Firmware
- Documentation

Common choices include CERN-OHL for hardware and MIT, BSD, GPL, or Apache-2.0 for firmware. The final choice should reflect whether commercial reuse and closed-source derivatives are intended to be allowed.

---

## 29. Current project summary

The current FluxNode-FOC concept is a 6-layer, STM32G474-based FOC motor controller for up to 8S operation. It uses a DRV8323 SPI gate driver, external 60 V MOSFETs, three phase-current measurements, three phase-voltage measurements, DC-bus sensing, an AS5047P rotor encoder, CAN-FD, USB-C, temperature monitoring, fan control, and an optional external-resistor brake chopper.

The firmware is generated with STM32CubeMX and developed in Visual Studio Code without PlatformIO.

The project remains under active development, and all high-power functionality must be validated experimentally before the design is considered complete.
