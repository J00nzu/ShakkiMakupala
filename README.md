# ShakkiMakupala

The winning chess bot in Metropolia games major chess tournament 2018.

The code is not very well documented since it was a school project but you're welcome to take a look and fork it for yourself. Most of the techniques used here have been researched from the https://www.chessprogramming.org site, with some modifications.

## Main features
- Bitboard engine with "magic number" sliding piece generator
- Alpha-Beta or Minimax pruning
- Multi-threading acceleration
- "Intelligent" board state evaluator
- Board state hashing

## Known bugs
- The AI likes to attack with the king for some reason. This didn't hinder it in the tournament tho.

## Remarks
The project is made with visual studio 2015 and has those project files ready. But all the code should be compilable in most standard compilers.
