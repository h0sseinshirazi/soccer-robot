# Soccer robot

Autonomous robot-soccer firmware for an STM32/Arduino build, written 2023–2024.

Nine sketches survive. They are **not** successive drafts of one program — they are
competing tactics, saved under different names and scattered across three drives until
this repo was assembled on 2026-09-22. Commit dates are the original file timestamps.

## Hardware it drives

- **TSOP infrared receivers** — ball tracking (`tsop`)
- **Three LDRs** — white boundary line, right / left / back (`ldr_R`, `ldr_L`, `ldr_B`,
  threshold `ldrSens 400`)
- **Motors** — direction-indexed omnidirectional drive (`move`, `moveforSec`)
- **Kicker** — `shoot()`, `shootForSec()` (present in the `rote()` family only)
- **SH1106 OLED** — live state via `print_all()`

## The two strategies

Orientation was solved twice and the lines never merged:

| sketch | orient | kicker | return-to-field | size |
|---|---|---|---|---|
| `codekarbal_DEF` | `turn()` | — | MoveIn · MoveInSide · out · out_ldr | 7.5 KB |
| `bh2` | `turn()` | — | MoveIn · MoveInSide · out · out_ldr | 7.6 KB |
| `codekarbal` | `turn()` | — | MoveIn · MoveInSide · out · out_ldr | 7.6 KB |
| `kohansal2` | `rote()` | yes | MoveIn · MoveInSide · out · out_ldr | 10.7 KB |
| `karbal24` | `rote()` | yes | MoveInSide · out_ldr | 11.3 KB |

`karbal24` is not "kohansal2 plus more" — it *removes* `MoveIn()` and `out()`, narrowing
the return-to-field logic deliberately. It is the largest and the last saved.

Three sketches carry no game logic: `robot` (movement only), `tobot2` and `bachesal2`
(sensor/motor bring-up rigs).

## Timeline

```
2023-01-24  robot            1.2 KB   movement only
2023-03-03  tobot2           4.8 KB   bring-up rig
2024-02-29  bachesal2        4.3 KB   bring-up + movement
2024-03-13  codekarbal_DEF   7.5 KB   turn() line begins
2024-03-14  bh2              7.6 KB   ← 14:11
2024-03-14  codekarbal       7.6 KB   ← 14:48, 37 minutes later
2024-04-16  final_final      1.6 KB   ← 22:01, not final
2024-04-16  kohansal2       10.7 KB   ← 22:20, rote() + kicker
2024-04-17  karbal24        11.3 KB   last saved
```

## Known unknown

**Nothing on disk records which tactic won on the field.** `turn()` versus `rote()`,
kicker versus no kicker — the result of that comparison lived only in memory. Filename
order misleads: `final_final` is the *smallest* file and was superseded within 19
minutes.

## Where these came from

| sketch | original location |
|---|---|
| `robot`, `tobot2`, `codekarbal`, `codekarbal_DEF` | Windows `Documents/Arduino` sketchbook |
| `bachesal2`, `kohansal2`, `final_final`, `karbal24` | sda2 (`New Volume`), some under `downd/` |
| `bh2` | sda3 (`New Volume1`), alone on the games drive |

`lib/` holds the Adafruit SH1106 driver and its STM32 port (third-party), vendored so
the sketches still compile.
