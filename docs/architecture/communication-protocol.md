# Communication Protocol

> Working draft. Use this document to define the external command, telemetry, configuration, and fault protocol for FluxNode-FOC over CAN-FD and USB diagnostic links.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| Protocol version | TBD |
| Primary transport | CAN-FD |
| Secondary transport | USB CDC diagnostics / configuration |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: CAN-FD identifiers, message layout, command semantics, telemetry messages, configuration access, fault reporting, versioning, and USB diagnostic mapping.
- Out of scope: Low-level FDCAN driver implementation details except when required to preserve timing, safety, or compatibility.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Communication intent | `README.md` section 15 | CAN-FD is the main external real-time interface. |
| FDCAN peripheral setup | `firmware/flux-node-foc.ioc` | Current IOC uses FDCAN2 with CAN-FD BRS enabled. |
| USB device class | `firmware/flux-node-foc.ioc` | Current IOC uses USB CDC FS. |

## Transport Summary

| Transport | Purpose | Current IOC / design note | Status |
|---|---|---|---|
| CAN-FD | Commands, setpoints, telemetry, faults, configuration | FDCAN2, nominal rate currently about 1 Mbit/s, FD with BRS enabled | TBD |
| USB CDC | Local configuration, diagnostics, firmware development | USB full-speed CDC product string `FluxNode FOC` | TBD |

## Node Identity

| Field | Size / type | Source | Notes |
|---|---|---|---|
| Node ID | TBD | TBD | Must be unique on a bus. |
| Hardware revision | TBD | TBD | Include in identity response. |
| Firmware version | TBD | Build metadata | Include commit if practical. |
| Serial number | TBD | TBD | Optional for early bring-up. |
| Motor profile ID | TBD | Calibration/config storage | TBD |

## CAN-FD Frame Layout

| Field | Bits / bytes | Description |
|---|---:|---|
| CAN identifier | TBD | Standard or extended ID policy. |
| DLC | TBD | Payload length. |
| Message type | TBD | Command, telemetry, config, fault, response. |
| Sequence / counter | TBD | Optional missed-frame detection. |
| Payload | TBD | Message-specific data. |
| Checksum | TBD | Usually not needed beyond CAN CRC unless payload requires one. |

## Identifier Allocation

| ID range / base | Direction | Message class | Priority | Notes |
|---|---|---|---|---|
| TBD | Host -> node | Control setpoints | Highest | Must be deterministic. |
| TBD | Node -> host | Fast telemetry | High | Current, voltage, state. |
| TBD | Node -> host | Faults/events | High | Should transmit immediately. |
| TBD | Host <-> node | Configuration | Medium | Rate-limited. |
| TBD | Node -> host | Slow telemetry | Low | Temperature, diagnostics. |
| TBD | Host <-> node | Bootloader/service | TBD | Define later if used. |

## Command Messages

| Command | Direction | Rate | Payload | Valid states | Safety behavior |
|---|---|---:|---|---|---|
| Enable | Host -> node | Event | TBD | TBD | TBD |
| Disable | Host -> node | Event | TBD | Any | Must shut down PWM safely. |
| Current setpoint | Host -> node | TBD | TBD | Run | Timeout must disable or hold safe state. |
| Velocity setpoint | Host -> node | TBD | TBD | TBD | TBD |
| Position setpoint | Host -> node | TBD | TBD | TBD | TBD |
| Clear fault | Host -> node | Event | TBD | Fault | Only if fault condition is gone. |

## Telemetry Messages

| Message | Direction | Rate | Payload fields | Notes |
|---|---|---:|---|---|
| Fast electrical telemetry | Node -> host | TBD | currents, bus voltage, PWM/state | Keep compact. |
| Encoder telemetry | Node -> host | TBD | angle, velocity, status | TBD |
| Temperature telemetry | Node -> host | TBD | FET, motor, brake, MCU | Slow. |
| Fault telemetry | Node -> host | Event + repeat TBD | fault code, raw driver status, state | Must be logged. |
| Identity | Node -> host | On request | versions, IDs | TBD |

## Configuration Access

| Parameter group | Access | Persistence | Notes |
|---|---|---|---|
| Limits | Read/write | TBD | Current, voltage, temperature limits. |
| Calibration | Read/write guarded | TBD | See `docs/calibration.md`. |
| CAN settings | Read/write guarded | TBD | Changing these can disconnect the node. |
| Motor profile | Read/write | TBD | TBD |
| Debug settings | Read/write | Volatile or persistent TBD | TBD |

## Fault And State Reporting

| Fault code | Source | Severity | Latching | Recovery |
|---|---|---|---|---|
| Gate-driver fault | DRV8323 / `DRV_nFAULT` | Critical | TBD | Decode SPI registers before clear. |
| Overcurrent | ADC / hardware | Critical | TBD | TBD |
| Bus overvoltage | ADC / brake logic | Critical | TBD | TBD |
| Bus undervoltage | ADC | TBD | TBD | TBD |
| Overtemperature | NTC/internal | TBD | TBD | TBD |
| Encoder fault | AS5047P/SPI | TBD | TBD | TBD |
| Command timeout | Protocol watchdog | Critical | TBD | Disable torque command. |

## Timing And Watchdogs

- Command timeout: TBD.
- Fast telemetry rate: TBD.
- Slow telemetry rate: TBD.
- Configuration rate limit: TBD.
- Fault repeat interval: TBD.
- Behavior on bus-off: TBD.

## USB CDC Diagnostic Mapping

| Function | Format | Notes |
|---|---|---|
| Console log | TBD | Do not block real-time control. |
| Command shell | TBD | Optional. |
| Binary diagnostics | TBD | Optional. |
| Firmware update | TBD | Define if USB bootloader is used. |

## Versioning And Compatibility

- Protocol version field: TBD.
- Backward compatibility policy: TBD.
- Unknown message behavior: TBD.
- Endianness: TBD.
- Numeric units and scaling: TBD.

## Test Cases

- [ ] Node identity request/response.
- [ ] Enable/disable sequence.
- [ ] Command timeout behavior.
- [ ] Fault message after forced `DRV_nFAULT`.
- [ ] Bus-off or disconnected CAN behavior.
- [ ] USB CDC enumeration and basic diagnostic exchange.
- [ ] High-rate telemetry does not disturb current-control timing.

## Open Questions

- TBD: Standard versus extended CAN identifiers.
- TBD: Binary payload schema and endianness.
- TBD: Whether USB uses the same protocol frames or a separate diagnostic shell.
- TBD: Bootloader and firmware-update strategy.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
