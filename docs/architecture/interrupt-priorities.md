# Interrupt Priorities

> Working draft. Use this document to define the real-time interrupt policy for FluxNode-FOC, especially PWM/current-control timing, ADC completion, gate-driver faults, communication, and low-priority diagnostics.

| Field | Value |
|---|---|
| Project | FluxNode-FOC |
| Hardware revision | TBD |
| Firmware revision / commit | TBD |
| MCU | STM32G474RE |
| Priority grouping | `NVIC_PRIORITYGROUP_4` in current IOC |
| Status | Draft / fill in |
| Last reviewed | TBD |

## Scope

- In scope: NVIC priorities, preemption rules, ISR execution budget, interrupt ownership, deferred work, and fault-response policy.
- Out of scope: Full control-loop design, except where it determines interrupt timing or allowed work inside an ISR.

## Source Of Truth

| Item | Source | Notes |
|---|---|---|
| Configured NVIC entries | `firmware/flux-node-foc.ioc` | Update after CubeMX changes. |
| Real-time policy | `README.md` section 21 | PWM, current sampling, current loop, and hardware faults have highest priority. |
| Timer and ADC timing | `docs/timer-map.md`, `docs/adc-map.md` | Keep trigger assumptions aligned. |

## Priority Policy

- Hardware shutdown paths must not depend only on firmware latency.
- Fast control interrupts must never perform blocking SPI, CAN, USB, logging, or heap allocation.
- ISR work should be bounded, measured, and moved to deferred tasks where possible.
- Communication interrupts must not delay current sampling or PWM update work.
- Slow measurement processing should run outside the fast control path.

## Interrupt Map

| Interrupt / event | Source | Current IOC priority | Intended priority | Owner module | ISR budget | Deferred work |
|---|---|---:|---:|---|---:|---|
| TIM1 update / compare | TIM1 PWM/control loop | TBD | TBD | Motor control | TBD us | TBD |
| ADC injected conversion complete | ADC1/2/3 current samples | `0` currently in IOC for ADC1_2 | TBD | Motor control | TBD us | Current processing/control loop TBD |
| `DRV_nFAULT` | EXTI4 on PC4 | `0` currently in IOC | TBD | Fault manager | TBD us | SPI fault-register read outside hard fault path |
| FDCAN2 IT0 | CAN-FD | `0` currently in IOC | TBD | Communication | TBD us | Message decode / app dispatch |
| USB LP | USB CDC | `0` currently in IOC | TBD | Diagnostics | TBD us | Console/protocol handling |
| SysTick | System tick | `15` currently in IOC | TBD | System | TBD us | Scheduler/timebase |
| HardFault / system faults | Cortex-M | Fixed | Fixed | System | N/A | Fault log if possible |

## Real-Time Execution Budget

| Task | Frequency / trigger | Max duration | Measurement method | Notes |
|---|---:|---:|---|---|
| Current sampling ISR entry to data ready | 25 kHz PWM related | TBD | GPIO toggle / SWO / timer capture | TBD |
| Current-control computation | TBD | TBD | TBD | TBD |
| PWM register update | TBD | TBD | TBD | TBD |
| Fault input to PWM disabled | Event | TBD | Scope measurement | Prefer hardware break where practical. |
| CAN RX ISR | Event / bus rate | TBD | TBD | Should enqueue only. |
| USB ISR | Event | TBD | TBD | Lowest practical urgency. |

## Fault Response Paths

| Fault source | Hardware immediate action | ISR action | Deferred action | Recovery policy |
|---|---|---|---|---|
| Gate-driver `nFAULT` | TBD | Disable torque/PWM if not already disabled | Read DRV8323 registers | TBD |
| Timer break input | TBD | Record event | Decode source | TBD |
| ADC overcurrent | TBD | TBD | Fault log | TBD |
| Bus overvoltage | TBD | Brake/PWM policy TBD | Fault log | TBD |
| Encoder communication fault | None | TBD | Diagnostics and state transition | TBD |
| Command timeout | None | TBD | Fault or safe state | TBD |

## Deferred Work Queues

| Queue / mechanism | Producer ISR | Consumer | Max depth | Overflow behavior |
|---|---|---|---:|---|
| CAN RX queue | FDCAN2 | Protocol task/main loop | TBD | TBD |
| Fault event queue | EXTI / ADC / timer | Fault manager | TBD | TBD |
| Telemetry buffer | Control loop / slow loop | CAN/USB transport | TBD | Drop or overwrite TBD |
| Log buffer | Any allowed source | USB/SWO logger | TBD | Non-blocking required |

## Prohibited ISR Operations

- Blocking SPI transactions.
- Blocking USB or CAN writes.
- Dynamic allocation.
- Long formatted printing.
- Flash erase/write.
- Waiting for ADC, timer, or communication flags in a polling loop.

## Validation Checklist

- [ ] Confirm generated NVIC priorities after CubeMX regeneration.
- [ ] Measure control ISR duration at target PWM frequency.
- [ ] Measure fault input to PWM-disabled latency.
- [ ] Confirm CAN RX load cannot starve control loop.
- [ ] Confirm USB traffic cannot starve control loop.
- [ ] Record ISR timing evidence in `docs/measurements/` or linked location.

## Open Questions

- TBD: Exact TIM1 interrupt source used for control-loop execution.
- TBD: Whether ADC ISR or TIM1 ISR owns current-control computation.
- TBD: Whether timer break input is routed and enabled.
- TBD: Final priority numbers for communication versus diagnostics.

## Revision Notes

| Date | Change | Evidence |
|---|---|---|
| TBD | Initial fill-in | TBD |
