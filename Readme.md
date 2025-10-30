### Setup

run `make` --> `cmd_catan.exe`

Optional commands (only use 1. In order of precedence):
- `-seed` to set RNG seed
- `-load [file]` to load old game (game is autosaved to backup.sv)
- `-board [file]` to use board layout specified in [file]
- `-random-board` to generate random board arrangement (otherwise uses default layout.txt)
- Can set a dice value with "load", or return to default fair dice with "fair" during roll turn

<img width="1485" height="755" alt="image" src="https://github.com/user-attachments/assets/529244df-a7de-4127-adeb-7cbea05edef2" />

### Game Info
- Regular Catan game rules (Turn order: Blue -> Red -> Orange -> Yellow)
- Vertices and edges are numbers (0-71) and (0-53) respectively.
- Resources are [brick, grain, lumber, ore, wool]
- Development cards are ["k", "vp", "rb", "yp", "m"]
- Agree with "yes" or "no"
- Begin turns with "roll"

Valid commands (after initial):
- board (print the board)
- status (print everyone's resources and public victory points)
- settlements (get a list of your own buildings)
- build-road <edge#>
- build-res <housing#>
- improve <housing#>
- trade <colour> <give> <give amount> <take> <take amount>
- buydev
- usedev <card>
- next (end turn)
- save <file>
- help