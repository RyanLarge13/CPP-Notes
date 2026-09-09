# TODO

## Bugs
- [ ] More to come for sure

## Features
- [ ] Check multiple chars with std::remove_if instead of std::remove in helpers.h to make `inline static string mainDirStringCleanup(const string &mainDir)` more versitile `helpers.h`
- [ ] Build a local serialization method to safely store passwords and other sensitive info `helpers.h`
- [ ] Create a more meaningful and robust `checkForExistingAccount()` method. `configManager.h`
- [ ] Finish off users note and folder creation after successful server data query `configManager.h`


## Cleanup
- [ ] Add section comments in configManager and organize methods `configManager.h`
- [ ] Research into why all files are header files and no cpp exists other than main, haha. WTF was I doing?
- [ ] Check configManager for globalUser and writeToConfig calls to make sure they are up to date with our new globalUser implementation `configManager.h`

## Done
- [x] Create a global user variable and update configuration based on what global user is at that time
- [x] Decide whether or not `void writeToConfigFile()` should return a boolean to make sure continuation of the program haults? Or maybe I could throw. **Decided to throw**