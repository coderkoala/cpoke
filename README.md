# cpoke
A Pokemon Go PvP Simulator written in C. The goal is to overcome many of the limitations imposed on PvPoke by JavaScript's memory management system, as well as eliminate the need for a Web Stack. Ultimately this codebase should aim to produce a variety of small re-usable functions and API which can easily be extended/used for other projects.

For example, `cpoke` may implement a GUI/TUI system, but the core code should not depend on these systems; which would limit the ability of future developers to use this simulator as a piece of their own project, with it's own GUI/TUI.


One of the core issues in PvPoke was it's memory management. While C is significantly more flexible in this regard, still be mindful to keep allocations to a minimum, and be as efficient as possible. The more lightweight the codebase can remain, the easier it will be to expand simulation detail. For example, simulating all pokemon with all movesets in all shield scenarios is simply not feasible if we are bogged down by poor MM. It is a slippery slope to start sacrificing movesets and pokemon selections to make up for slow simulations.
Having said that you will notice that in some cases (ex move encoding) space is sacrificed to avoid dereferencing data. This allows pokemon to hold their move definitions, and leaves the opportunity to calculate battles with GPGPU.


API/headers should operate in such a generalized that data sources ( whether by SQL, JSON Parsing, etc ) are abstracted away from the Simulator. Similarly the codebase should remain in separate modules such that shared objects may be compiled and wrapped by other languages, so that parts of this system might be used by Python, C++, etc without needing to modify the codebase extensively. Our own compilation can of course flatten these modules, optimizing out separations into a single binary for direct usage. See the headers `store.h` and `gm_store.h` for an example of an API/module definition and an implementation of that module.


## COMPLETED
- Abstract interfaces for AI modules and Data Stores.
- Parsing and storage of `GAME_MASTER.json`.
- Functional Data Store for Pokemon (Pokedex) and PvP move data.
- Object definitions for most everything required by the PvP Simulator.
- Calculators for simulations. ( CP, Damage, etc... )


## TODO
See "Projects" tab for full details.
- Battle processing and logging are the most pressing.
- Parsing of CalcyIV rosters.
- Data stores for rosters.
- Battle processing ( most of the calculation is done, it's just a matter of looping and logging ).


## Notes
- "ext" files are those which are not authored by devs here, but have been included to lock versions/ease build process.
- Many "util" files are authored by alexameen for use across projects, and should not be explicitly targeted for `cpoke`. ( If you extend them, keep things genearalized for use in "any project" ).
- I am particularly picky about code formatting; but don't let that stop you from putting in PRs. Just don't be surprised if I swoop in and reformat after the fact. <3
