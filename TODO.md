# TODO's

## {Month} {Year} ------------- {version}

## Bugs
- [ ]

## Features
- [ ] 

## Cleanup
- [ ] 

<hr></hr>

## September 2027 ------------- 1.0.0

## Bugs
- [ ] More to come for sure

## Features
- [ ] Clean up user data and how it is accessed throughout the application 
- [ ] Create a more meaningful and robust `checkForExistingAccount()` method. `configManager.h`
- [x] ***9/15*** Build local encryption/decryption methods to "safely" store passwords and other sensitive info `helpers.h`
- [x] ***9/14*** Change all calls to method `updateConfig(...)` in `configManager.h` to instead call `writeToConfigFile()`. `configManager.h`
- [x] ***9/14*** Establish note creation and initialization in recursive folder loop `configManager`
- [x] ***9/12*** Finish off users note and folder creation after successful server data query `configManager.h`
- [x] ***9/12*** Decide whether or not `void writeToConfigFile()` should return a boolean to make sure continuation of the program haults? Or maybe I could throw. (**Decided to throw**)

## Cleanup
- [x] ***9/14*** Check multiple chars with std::remove_if instead of std::remove in helpers.h to make `inline static string mainDirStringCleanup(const string &mainDir)` more versitile `helpers.h`
- [x] ***9/14*** Add section comments in configManager and organize methods `configManager.h`
- [x] ***9/14*** Research into why all files are header files and no cpp exists other than main, haha. WTF was I doing? (**Keeping the same for now**)