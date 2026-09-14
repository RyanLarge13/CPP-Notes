# TODO's

## {Month} {Year} -------------

## Bugs
- [ ]

## Features
- [ ] 

## Cleanup
- [ ] 

## Done
- [ ] 

<hr></hr>

## September 2027 -------------

## Bugs
- [ ] More to come for sure

## Features
- [ ] Build a local serialization method to safely store passwords and other sensitive info `helpers.h`
- [ ] Create a more meaningful and robust `checkForExistingAccount()` method. `configManager.h`
- [x] ***9/14*** Change all calls to method `updateConfig(...)` in `configManager.h` to instead call `writeToConfigFile()`. `configManager.h`
- [x] ***9/14*** Establish note creation and initialization in recursive folder loop `configManager`
- [x] ***9/12*** Finish off users note and folder creation after successful server data query `configManager.h`


## Cleanup
- [ ] Check multiple chars with std::remove_if instead of std::remove in helpers.h to make `inline static string mainDirStringCleanup(const string &mainDir)` more versitile `helpers.h`
- [x] *9/14* Add section comments in configManager and organize methods `configManager.h`
- [x] *9/14* Research into why all files are header files and no cpp exists other than main, haha. WTF was I doing?

## Done
- [x] Create a global user variable and update configuration based on what global user is at that time
- [x] Decide whether or not `void writeToConfigFile()` should return a boolean to make sure continuation of the program haults? Or maybe I could throw. (**Decided to throw**)
- [x] Check configManager for globalUser and writeToConfig calls to make sure they are up to date with our new globalUser implementation `configManager.h`
- [x] Finish off users note and folder creation after successful server data query `configManager.h`
- [x] Establish note creation and initialization in recursive folder loop `configManager`
- [x] Add section comments in configManager and organize methods `configManager.h`
- [x] Research into why all files are header files and no cpp exists other than main, haha. WTF was I doing? (**Keeping the same for now**)
- [x] Change all calls to method `updateConfig(...)` in `configManager.h` to instead call `writeToConfigFile()`. `configManager.h`