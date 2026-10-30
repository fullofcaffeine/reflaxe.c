# Date and time on the C target

<!-- hxrt-feature:date-time -->

The C target supports ordinary Haxe `Date` values for portable UTC work. A hosted adapter supplies local time, wall time, and monotonic time.

`Date` stores one Unix timestamp as `Float` milliseconds. Each call to `Date.fromTime` creates a new object, so aliases retain normal Haxe identity.

## Supported API

The target supports these `Date` operations:

- the six-field local constructor;
- `Date.fromTime` and `Date.now`;
- `getTime`;
- all local and UTC field getters;
- `getTimezoneOffset`;
- `toString` with the `YYYY-MM-DD HH:MM:SS` local format.

`Date.fromString` is not supported. `haxe.Timer.stamp` is supported, but timer scheduling is not supported.

The compiler reports these gaps at the Haxe source position. It does not emit partial C for them.

## UTC and local time

UTC field getters use Gregorian calendar arithmetic from the target-owned Haxe `Date` module. This work does not use the host timezone database.

The UTC projection accepts finite timestamps from `-8.64e15` through `8.64e15` milliseconds. It rejects values outside this range before calendar conversion.

`getTime` preserves fractional milliseconds. Calendar fields truncate the complete timestamp toward zero, which matches the pinned Haxe behavior.

Local operations use the timezone rules of the host process. On POSIX hosts, an explicit `TZ` value makes this behavior repeatable in tests.

The local constructor uses the host `mktime` rules. These rules normalize missing daylight-saving times and select one value for repeated local times.

For example, the test rule `CST6CDT,M3.2.0/2,M11.1.0/2` gives these results:

| Local input or instant | Result |
| --- | --- |
| `2024-03-10 02:30:00` | Normalizes to `03:30:00` after the spring gap. |
| `2024-11-03 01:30:00` | The host selects either occurrence; the offset must match the selected timestamp. |
| `2024-03-10 07:59:59Z` | Reports `360` minutes for UTC minus local time. |
| `2024-03-10 08:00:00Z` | Reports `300` minutes for UTC minus local time. |

The pinned Eval target reports `300` for the pre-spring-transition instant while it renders standard time. The C target reports `360` to keep the offset consistent with the rendered local time.

On Linux, Eval also reports a standard-time offset for the first fall-back instant. The C target reports its correct daylight-time offset of `300`. A local constructor in the repeated hour may select either valid instant, because the host owns that choice. The focused test checks each native timestamp together with its offset and compares all other output exactly.

## Wall time and monotonic time

Wall time and monotonic time have different jobs:

| API | Unit | Use |
| --- | --- | --- |
| `Date.now().getTime()` | Unix-epoch milliseconds | Timestamps that users can compare with calendar time. |
| `haxe.Timer.stamp()` | Seconds from an unspecified host epoch | Elapsed duration inside one process. |

The operating system can adjust wall time. Thus, wall time can move backward or forward.

Monotonic time does not use the calendar clock. Use `haxe.Timer.stamp()` to measure elapsed duration.

## Hosted boundary and range

The private runtime boundary uses status results and output parameters. It allocates no memory and stores no timezone state.

The host `time_t` range can be smaller than the portable UTC range. Local operations reject timestamps that the host cannot represent.

The `date-time` runtime feature is available only in the `hosted` environment. `freestanding` and `wasi` builds reject reachable local or clock operations with `HXC2000`.

Portable UTC access does not select the hosted `date-time` feature. A `Date` object still needs the collector, which currently supports `hosted` and `freestanding` builds.

Thus, the current `wasi` environment rejects `Date` objects even when the program uses only UTC fields. This limit is separate from timezone support.

The runtime symbols are private implementation details. They are not a stable application ABI.

## Evidence

Run the focused acceptance command:

```sh
npm run test:date-time
```

The command compares native results with the pinned Eval target in three timezones. It also compiles strict C11, C++17 headers, and sanitizer probes.

The same command checks generated C, runtime selection, overflow errors, daylight transitions, and unsupported environments.
