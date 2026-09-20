# fastwalk

Fat iterator reductions for Lua tables — one C call per traversal.

Pure-Lua `pairs()` crosses the Lua?C boundary once per element. This library
walks the table entirely in C, eliminating those crossings.

## Results

On a 1M-element array (Lua 5.5, Windows, gcc -O2):

| Function | Time | vs pairs |
|----------|------|----------|
| pairs    | 0.050s | 1.0x |
| ipairs   | 0.050s | 1.0x |
| fatsum   | 0.017s | 2.9x |
| max      | 0.018s | 2.8x |
| min      | 0.018s | 2.8x |

## API

- `fw.sum_values(t)` — sum of all numeric values
- `fw.count(t)` — number of pairs
- `fw.max_value(t)` — maximum numeric value
- `fw.min_value(t)` — minimum numeric value

## Build (Windows, Lua 5.5 via Scoop)
